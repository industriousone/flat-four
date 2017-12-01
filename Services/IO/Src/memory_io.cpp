/**
 * @file memory_io.cpp
 *
 * @author  Jason Perkins (jason@379.com)
 * @date    Copyright (C) 2001 by 379, Inc.
 * @version $Id: memory_io.cpp,v 1.2 2002/04/22 01:51:56 jason379 Exp $
 *
 * @todo Implement write() method.
 *
 */
#include <Core/core.h>
#include <IO/io.h>
#include <IO/raw_stream.h>
#include <Platform/file.h>
#include "memory_io.h"
#include <Memory/memory.h>
using namespace f4;

DECLARE_SERVICE(InputStream)

IMPL_ANON_SERVICE(MemoryStream, RawStream)

//---------------------------------------------------------------------------

MemoryStream::MemoryStream()
: m_buffer(0),
  m_size(0),
  m_current(0)
{
}

//---------------------------------------------------------------------------

MemoryStream::~MemoryStream()
{
	if (m_buffer) free(m_buffer);
}

//---------------------------------------------------------------------------

bool MemoryStream::initialize(void* buffer, unsigned size)
{
	m_buffer = (UInt8*)buffer;
	m_size   = size;
	return true;
}

//---------------------------------------------------------------------------

bool MemoryStream::eof()
{
	return (m_current >= m_size);
}

//---------------------------------------------------------------------------

unsigned MemoryStream::read(void* buffer, unsigned num)
{
	if (num > m_size - m_current) 
		num = m_size - m_current;
	
	UInt8* src = &m_buffer[m_current];
	UInt8* dst = (UInt8*)buffer;
	UInt8* end = src + num;

	while (src < end)
	{
		*(dst++) = *(src++);
	}

	m_current += num;
	return num;
}

//---------------------------------------------------------------------------

unsigned MemoryStream::write(void* buffer, unsigned num)
{
	return 0;
}

//---------------------------------------------------------------------------

bool MemoryStream::flush()
{
	return true;
}

//---------------------------------------------------------------------------

bool MemoryStream::seek(int offset, int origin)
{
	unsigned newPos = m_current;

	switch (origin)
	{
	case FILE_START:
		newPos = offset;
		break;

	case FILE_CURRENT:
		newPos = m_current + offset;
		break;

	case FILE_END:
		newPos = m_size - offset;
		break;
	}

	if (newPos < 0) return false;
	if (newPos > m_size) return false;

	m_current = newPos;
	return true;
}

//---------------------------------------------------------------------------

unsigned MemoryStream::tell()
{
	return m_current;
}
