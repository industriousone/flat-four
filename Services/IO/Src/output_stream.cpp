/**
 * @file output_stream.cpp
 *
 * @author  Jason Perkins (jason@379.com)
 * @date    Copyright (C) 2001 by 379, Inc.
 * @version $Id: output_stream.cpp,v 1.3 2002/04/25 01:17:47 jason379 Exp $
 */
#include <Core/core.h>
#include <IO/io.h>
#include <IO/raw_stream.h>
#include <Platform/endian.h>
#include <Memory/memory.h>
using namespace f4;


struct OutputStreamImpl : public OutputStream
{
	OutputStreamImpl();
	virtual ~OutputStreamImpl() { }

	virtual bool initialize(RawStream* raw, Directory* dir, const Char* name);
	virtual unsigned write(void* buffer, unsigned num);
	virtual bool flush();

	virtual void setByteOrder(int order);
	virtual bool writeInt8(UInt8 i);
	virtual bool writeInt16(UInt16 i);
	virtual bool writeInt32(UInt32 i);
	virtual bool writeFloat(Float f);
	virtual bool writeString(const Char* s);

private:
	Ref<RawStream> m_stream;
	Ref<Directory> m_directory;
	std::string    m_name;
	int m_byteOrder;
	DECL_SERVICE;
};

IMPL_SERVICE(OutputStreamImpl, OutputStream)

//---------------------------------------------------------------------------

OutputStreamImpl::OutputStreamImpl()
: m_byteOrder(LSB)
{
}

//---------------------------------------------------------------------------

bool OutputStreamImpl::initialize(RawStream* raw, Directory* dir, const Char* name)
{
	m_stream    = raw;
	m_directory = dir;
	m_name      = name;
	return true;
}

//---------------------------------------------------------------------------

unsigned OutputStreamImpl::write(void* buffer, unsigned num)
{
	return m_stream->write(buffer, num);
}

//---------------------------------------------------------------------------

bool OutputStreamImpl::flush()
{
	return m_stream->flush();
}

//---------------------------------------------------------------------------

void OutputStreamImpl::setByteOrder(int order)
{
	m_byteOrder = order;
}

//---------------------------------------------------------------------------

bool OutputStreamImpl::writeInt8(UInt8 i)
{
	return (m_stream->write(&i, 1) == 1);
}

//---------------------------------------------------------------------------

bool OutputStreamImpl::writeInt16(UInt16 i)
{
	i = (m_byteOrder == MSB) ? convertMSB(i) : convertLSB(i);
	return (m_stream->write(&i, 2) == 2);
}

//---------------------------------------------------------------------------

bool OutputStreamImpl::writeInt32(UInt32 i)
{
	i = (m_byteOrder == MSB) ? convertMSB(i) : convertLSB(i);
	return (m_stream->write(&i, 4) == 4);
}

//---------------------------------------------------------------------------

bool OutputStreamImpl::writeFloat(Float f)
{
	f = (m_byteOrder == MSB) ? convertMSB(f) : convertLSB(f);
	return (m_stream->write(&f, sizeof(Float)) == sizeof(Float));
}

//---------------------------------------------------------------------------

bool OutputStreamImpl::writeString(const Char* s)
{
	int len = strlen(s);
	return (m_stream->write((void*)s, len) == len);
}
