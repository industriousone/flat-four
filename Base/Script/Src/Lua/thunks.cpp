/**
 * @file thunks.cpp
 *
 * @author  Jason Perkins (jason@379.com)
 * @date    Copyright (C) 2001 by 379, Inc.
 * @version $Id: thunks.cpp,v 1.3 2002/04/25 01:19:30 jason379 Exp $
 */
#include <stdio.h>
#include <string.h>

extern "C"
{
#include "lua.h"
#include "../src/lstate.h"
}

#include <Core/core.h>
#include "extensions.h"
#include <Memory/memory.h>
using namespace f4;


// These routines are used to bridge between Lua's C code and F4's C++

//---------------------------------------------------------------------------

/*
 * A replacement for Lua's use of fputs() (see Lua/src/lbaselib.c)
 */
extern "C" void _f4Printer(const char* msg)
{
	Core->print(msg);
}

//---------------------------------------------------------------------------

/*
 * A replacement for Lua's use of realloc()
 */
extern "C" void* _f4Realloc(void* ptr, size_t size)
{
	return realloc(ptr, size);
}

//---------------------------------------------------------------------------

/*
 * A replacement for Lua's use of free()
 */
extern "C" void _f4Free(void* ptr)
{
	free(ptr);
}

//---------------------------------------------------------------------------

/*
 * The app gets killed on all script errors.
 */
extern "C" void _f4Error(lua_State* L, const char* msg)
{
	// Trim off some extra text that looks bad in the console
	char* ptr = strstr(msg, " at line 1 in string \"\"");
	if (ptr) *ptr = 0;


	// Based on code from luaB__ERRORMESSAGE in lbaselib.c
	Char err[1024];

	strcpy(err, "Script error: ");
	strcat(err, msg);

	// Step through each active stack in turn until I find file/line info
	lua_Debug ar;
	for (int stack = 0; lua_getstack(L, stack, &ar); ++stack)
	{
		lua_getinfo(L, "Sl", &ar);
		if (ar.source && ar.currentline > 0)
		{
			char buff[100];
			sprintf(buff, "\n  <%.70s: line %d>", ar.short_src, ar.currentline);
			strcat(err, buff);
			break;
		}
	}

	Core->error(err);
}

//---------------------------------------------------------------------------

/*
 * A replacement for lua_dofile(). Calls Core->runFile() which loads the
 * script into a buffer, then calls Script->runBuffer() which calls 
 * lua_dobuffer().
 */
extern "C" int _f4DoFile(lua_State* L, const char* filename)
{
	if (!Core->runFile(filename, _asContext(L)))
		return LUA_ERRFILE;
	else
		return 0;
}
