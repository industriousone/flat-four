/**
 * @file reader_writer.cpp
 *
 * @author  Jason Perkins (jason@379.com)
 * @date    Copyright (C) 2001 by 379, Inc.
 * @version $Id: reader_writer.cpp,v 1.2 2002/04/22 01:51:56 jason379 Exp $
 */
#include <Core/core.h>
#include <IO/io.h>
#include <IO/reader_writer.h>
#include <Memory/memory.h>
using namespace f4;

struct ReaderWriterImpl : public ReaderWriter
{
	virtual Service* read(InputStream* stream);
	virtual bool write(Service* service, OutputStream* stream);

private:
	DECL_SERVICE;
};

IMPL_SERVICE(ReaderWriterImpl, ReaderWriter);

//---------------------------------------------------------------------------

Service* ReaderWriterImpl::read(InputStream* stream)
{
	Service* service;

	DISPATCH_METHOD("read", 0)
		DISPATCH_ARG_SERVICE(stream)
	DISPATCH_RETURNS_SERVICE(service);
	
	return service;
}

//---------------------------------------------------------------------------

bool ReaderWriterImpl::write(Service* service, OutputStream* stream)
{
	bool result;

	DISPATCH_METHOD("write", 0)
		DISPATCH_ARG_SERVICE(service)
		DISPATCH_ARG_SERVICE(stream)
	DISPATCH_RETURNS_BOOL(result);

	return result;
}



