/**
 * @file register.cpp
 *
 * @author  Jason Perkins (jason@379.com)
 * @date    Copyright (C) 2001 by 379, Inc.
 * @version $Id: register.cpp,v 1.2 2002/04/22 01:51:56 jason379 Exp $
 */
#include <Core/core.h>
#include <IO/io.h>
#include <Memory/memory.h>
using namespace f4;


/**
 * Registers all of the services contained in the package. There are
 * no methods to call, just creating an instance of this service will
 * do the trick.
 * @ingroup f4_stdio
 */
struct Register : public Service
{
	Register();
	virtual ~Register() { }

private:
	DECL_SERVICE;
};

IMPL_ANON_SERVICE(Register, Service)



//---------------------------------------------------------------------------
// Message text
//---------------------------------------------------------------------------

static const Char* START_MSG = "Registering standard set of ReaderWriters\n";
static const Char* NO_IO     = "I/O service not installed\n";


//---------------------------------------------------------------------------

Register::Register()
{
	Core->print(START_MSG);

	IOService* io = (IOService*)Core->createA("IO.IOService");
	if (!io) 
	{
		Core->print(NO_IO);
		return;
	}

	io->associate("4fnt", "StdIO.rw4FNT");
	io->associate("bmp",  "StdIO.rwBMP");
	io->associate("obj",  "StdIO.rwOBJ");
	io->associate("png",  "StdIO.rwPNG");
	io->associate("tga",  "StdIO.rwTGA");
}
