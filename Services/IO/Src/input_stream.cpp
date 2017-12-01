/**
 * @file input_stream.cpp
 *
 * @author  Jason Perkins (jason@379.com)
 * @date    Copyright (C) 2001 by 379, Inc.
 * @version $Id: input_stream.cpp,v 1.2 2002/04/22 01:51:56 jason379 Exp $
 */
#include <Core/core.h>
#include <IO/io.h>
#include <IO/filter.h>
#include <IO/raw_stream.h>
#include <Platform/endian.h>
#include <Math/math.h>
#include <list>
#include "memory_io.h"
#include <Memory/memory.h>
using namespace std;
using namespace f4;

DECLARE_SERVICE(MemoryStream)


struct InputStreamImpl : public InputStream
{
	InputStreamImpl();
	virtual ~InputStreamImpl() { }

	virtual bool initialize(RawStream* raw, Directory* dir, const Char* name);

	virtual unsigned read(void* buffer, unsigned num);
	virtual bool     seek(int offset, int origin);
	virtual unsigned tell();
	virtual bool     eof();

	virtual void   setByteOrder(int order);
	virtual UInt8  readInt8();
	virtual UInt16 readInt16();
	virtual UInt32 readInt32();
	virtual Float  readFloat();
	virtual string readString();
	virtual string readFixedString(int length);
	virtual void   readIntegers(UInt32* ints, int num);
	virtual void   readFloats(Float* floats, int num);
	virtual void   readVectors(Vector3* vectors, int num);
	
	virtual InputStream* createMemoryFile(unsigned size);
	
	virtual Directory*  getDirectory();
	virtual const Char* getSourceName();

	virtual void addFilter(IOFilter* filter);

private:
	Ref<RawStream> m_stream;
	Ref<Directory> m_directory;
	std::string    m_name;
	int m_byteOrder;

	typedef std::list< Ref<IOFilter> > Filters;
	Filters m_filters;

	DECL_SERVICE;
};

IMPL_SERVICE(InputStreamImpl, InputStream)

//---------------------------------------------------------------------------

InputStreamImpl::InputStreamImpl()
: m_byteOrder(LSB)
{
}

//---------------------------------------------------------------------------

bool InputStreamImpl::initialize(RawStream* raw, Directory* dir, const Char* name)
{
	m_stream    = raw;
	m_directory = dir;
	m_name      = name;
	return true;
}

//---------------------------------------------------------------------------

Directory* InputStreamImpl::getDirectory()
{
	return m_directory;
}

//---------------------------------------------------------------------------

const Char* InputStreamImpl::getSourceName()
{
	return m_name.c_str();
}

//---------------------------------------------------------------------------

unsigned InputStreamImpl::read(void* buffer, unsigned num)
{
	return m_stream->read(buffer, num);
}

//---------------------------------------------------------------------------

bool InputStreamImpl::seek(int offset, int origin)
{
	return m_stream->seek(offset, origin);
}

//---------------------------------------------------------------------------

unsigned InputStreamImpl::tell()
{
	return m_stream->tell();
}

//---------------------------------------------------------------------------

bool InputStreamImpl::eof()
{
	return m_stream->eof();
}

//---------------------------------------------------------------------------

void InputStreamImpl::setByteOrder(int order)
{
	m_byteOrder = order;
}

//---------------------------------------------------------------------------

UInt8 InputStreamImpl::readInt8()
{
	UInt8 x;
	m_stream->read(&x, 1);
	return x;
}

//---------------------------------------------------------------------------

UInt16 InputStreamImpl::readInt16()
{
	UInt16 x;
	m_stream->read(&x, 2);
	return (m_byteOrder == MSB) ? convertMSB(x) : convertLSB(x);
}

//---------------------------------------------------------------------------

UInt32 InputStreamImpl::readInt32()
{
	UInt32 x;
	m_stream->read(&x, 4);
	return (m_byteOrder == MSB) ? convertMSB(x) : convertLSB(x);
}

//---------------------------------------------------------------------------

Float InputStreamImpl::readFloat()
{
	Float x;
	m_stream->read(&x, 4);
	return (m_byteOrder == MSB) ? convertMSB(x) : convertLSB(x);
}

//---------------------------------------------------------------------------

string InputStreamImpl::readString()
{
	string result;

	char  buffer[1024];
	char* ptr = buffer;
	char* end = buffer + 511;

	m_stream->read(ptr, 1);
	while (*ptr && !m_stream->eof())
	{
		if (++ptr == end)
		{
			*ptr = 0;
			result += buffer;
			ptr = buffer;
		}

		m_stream->read(ptr, 1);
	}

	*ptr = 0;
	result += buffer;
	return result;
}

//---------------------------------------------------------------------------

string InputStreamImpl::readFixedString(int length)
{
	string result;
	result.resize(length + 1);

	m_stream->read((void*)result.data(), length);
	result[length] = 0;

	return result;
}

//---------------------------------------------------------------------------

void InputStreamImpl::readIntegers(UInt32* ints, int num)
{
	m_stream->read(ints, num * sizeof(UInt32));

#if defined(F4_SYSTEM_LITTLE_ENDIAN)
	if (m_byteOrder == MSB)
	{
		UInt32* end = ints + num;
		for (UInt32* ptr = ints; ptr < end; ++ptr)
			convertMSB(*ptr);
	}
#else
	if (m_byteOrder == LSB)
	{
		UInt32* end = ints + num;
		for (UInt32* ptr = ints; ptr < end; ++ptr)
			convertLSB(*ptr);
	}
#endif
}

//---------------------------------------------------------------------------

void InputStreamImpl::readFloats(Float* floats, int num)
{
	m_stream->read(floats, num * sizeof(Float));

#if defined(F4_SYSTEM_LITTLE_ENDIAN)
	if (m_byteOrder == MSB)
	{
		Float* end = floats + num;
		for (Float* ptr = floats; ptr < end; ++ptr)
			convertMSB(*ptr);
	}
#else
	if (m_byteOrder == LSB)
	{
		Float* end = floats + num;
		for (Float* ptr = floats; ptr < end; ++ptr)
			convertLSB(*ptr);
	}
#endif
}

//---------------------------------------------------------------------------

void InputStreamImpl::readVectors(Vector3* vectors, int num)
{
	m_stream->read(vectors, num * sizeof(Vector3));

#if defined(F4_SYSTEM_LITTLE_ENDIAN)
	if (m_byteOrder == MSB) 
	{
		Float* end = (Float*)(vectors + num); 
		for (Float* ptr = (Float*)vectors; ptr < end; ++ptr)
			convertMSB(*ptr);
	}
#else
	if (m_byteOrder == LSB) 
	{
		Float* end = (Float*)(vectors + num); 
		for (Float* ptr = (Float*)vectors; ptr < end; ++ptr)
			convertLSB(*ptr);
	}
#endif
}

//---------------------------------------------------------------------------

InputStream* InputStreamImpl::createMemoryFile(unsigned size)
{
	UInt8* buffer = (UInt8*)malloc(size);
	size = m_stream->read(buffer, size);

	MemoryStream* ms = CREATEA_LOCAL(MemoryStream);
	ms->initialize(buffer, size);

	InputStream* stream = new InputStreamImpl;
	stream->initialize(ms, m_directory, m_name.c_str());
	return stream;
}

//---------------------------------------------------------------------------

void InputStreamImpl::addFilter(IOFilter* filter)
{
	if (!m_filters.empty())
		filter->setTarget(m_filters.back());
	else
	{
		filter->setTarget(m_stream);
		m_stream = filter;
	}
}
