/**
 * @file rw_obj.cpp
 *
 * @author  Jason Perkins (jason@379.com)
 * @date    Copyright (C) 2001 by 379, Inc.
 * @version $Id: rw_obj.cpp,v 1.2 2002/04/22 01:51:56 jason379 Exp $
 */
#include <Core/core.h>
#include <IO/io.h>
#include <IO/reader_writer.h>
#include <Memory/memory.h>
using namespace f4;


struct rwOBJ : public ReaderWriter
{
	virtual Service* read(InputStream* stream);
	virtual bool write(Service* service, OutputStream* stream);

private:
	DECL_SERVICE;
};

IMPL_ANON_SERVICE(rwOBJ, ReaderWriter)


//---------------------------------------------------------------------------
// Message text
//---------------------------------------------------------------------------



//---------------------------------------------------------------------------

Service* rwOBJ::read(InputStream* stream)
{
	return 0;
}

//---------------------------------------------------------------------------

bool rwOBJ::write(Service* service, OutputStream* stream)
{
	return false;
}
