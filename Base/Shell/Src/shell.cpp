/**
 * @file shell.cpp
 *
 * @author  Jason Perkins (jason@379.com)
 * @date    Copyright (C) 2001 by 379, Inc.
 * @version $Id: shell.cpp,v 1.3 2002/06/19 01:40:08 jason379 Exp $
 *
 * @todo Make the boot exe more generic, as part of separating core and
 *       script services. Thinking: 'f4 [-code dllname] args...'. Load
 *       DLL and call main() with args. If node dll specified, load Lua
 *       script engine. Much better than current.
 *
 * @todo Move the execution of 'core.cfg' out of the engine and place it
 *       here. Rename to 'shell.cfg'.
 *
 */
#include <Core/core.h>
#include <Platform/library.h>
#include <Platform/print_alert.h>
using namespace f4;


//---------------------------------------------------------------------------
// Message text
//---------------------------------------------------------------------------

static const Char* DLG_CAPTION    = "Missing File";
static const Char* MISSING_CORE   = "Unable to load Core service.";
static const Char* MISSING_SCRIPT = "Unable to load script '%s'";



//---------------------------------------------------------------------------

int main(int argc, char* argv[])
{
	// Make all platforms look for shared libraries in the app directory
	normalizeLinkerBehavior(argv);

	// Load and start the engine
	SharedLibrary hDll = openLibrary("Core");
	if (!hDll)
	{
		printAlert(DLG_CAPTION, MISSING_CORE);
		return -1;
	}

	CoreService* (*createCoreService)();
	bindSymbol((void**)&createCoreService, hDll, "createCoreService");
	CoreService* core = createCoreService();

	if (!core->startEngine(argc, argv))
		return -1;


	// Run any scripts specified on the command line
	for (int arg = 1; arg < argc; arg++)
	{
		if (!core->runFile(argv[arg]))
			printAlert(DLG_CAPTION, MISSING_SCRIPT, argv[arg]);
	}


	// Then run the event loop (if one was registered by the scripts)
	core->run();


	core->stopEngine();
	return 0;
}

