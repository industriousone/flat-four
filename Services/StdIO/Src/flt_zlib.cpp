/**
 * @file flt_zlib.cpp
 *
 * @author  Jason Perkins (jason@379.com)
 * @date    Copyright (C) 2001 by 379, Inc.
 * @version $Id: flt_zlib.cpp,v 1.2 2002/04/22 01:51:56 jason379 Exp $
 */
#include "zlib.h"

#include <Core/core.h>
#include <IO/filter.h>
#include <Memory/memory.h>
using namespace f4;

static const int BUF_SIZE = 1024;


struct ZlibFilter : public IOFilter
{
	ZlibFilter();
	virtual ~ZlibFilter();

	void setTarget(RawStream* target);

	// RawStream interface
	virtual bool eof();
	virtual unsigned read(void* buffer, unsigned num);
	virtual unsigned write(void* buffer, unsigned num);
	virtual bool flush();
	virtual bool seek(int offset, int origin);
	virtual unsigned tell();

private:
	Ref<RawStream> m_target;
	z_stream m_zlib;
	UInt8*   m_buffer;

	bool _prepareZlib();

	DECL_SERVICE;
};

IMPL_ANON_SERVICE(ZlibFilter, IOFilter)

//---------------------------------------------------------------------------

ZlibFilter::ZlibFilter()
{
	m_buffer = (UInt8*)malloc(BUF_SIZE);
	_prepareZlib();
}

//---------------------------------------------------------------------------

ZlibFilter::~ZlibFilter()
{
	inflateEnd(&m_zlib);
	free(m_buffer);
}

//---------------------------------------------------------------------------

void ZlibFilter::setTarget(RawStream* target)
{
	m_target = target;
}

//---------------------------------------------------------------------------

bool ZlibFilter::eof()
{
	return (m_target) ? m_target->eof() : true;
}

//---------------------------------------------------------------------------

unsigned ZlibFilter::read(void* buffer, unsigned num)
{
	if (!m_target) return 0;

	m_zlib.next_out  = (UInt8*)buffer;
	m_zlib.avail_out = num;
	m_zlib.total_out = 0;

	int z = Z_OK;
	while (z == Z_OK && m_zlib.avail_out > 0)
	{
		// Make sure there is data available to decompress
		if (m_zlib.avail_in == 0)
		{
			m_zlib.avail_in = m_target->read(m_buffer, BUF_SIZE);
			m_zlib.next_in  = m_buffer;
		}

		// Decompress some data
		z = inflate(&m_zlib, 0);
	}

	return m_zlib.total_out;
}

//---------------------------------------------------------------------------

unsigned ZlibFilter::write(void* buffer, unsigned num)
{
	return (m_target) ? m_target->write(buffer, num) : 0;
}

//---------------------------------------------------------------------------

bool ZlibFilter::flush()
{
	return (m_target) ? m_target->flush() : false;
}

//---------------------------------------------------------------------------

bool ZlibFilter::seek(int offset, int origin)
{
	return (m_target) ? m_target->seek(offset, origin) : false;
}

//---------------------------------------------------------------------------

unsigned ZlibFilter::tell()
{
	return (m_target) ? m_target->tell() : 0;
}

//---------------------------------------------------------------------------

bool ZlibFilter::_prepareZlib()
{
	m_zlib.next_in  = Z_NULL;
	m_zlib.avail_in = 0;
	m_zlib.total_in = 0;

	m_zlib.next_out  = Z_NULL;
	m_zlib.avail_out = 0;
	m_zlib.total_out = 0;

	m_zlib.zalloc = Z_NULL;
	m_zlib.zfree  = Z_NULL;

	m_zlib.data_type = Z_BINARY;

	return (inflateInit(&m_zlib) == Z_OK);
}

