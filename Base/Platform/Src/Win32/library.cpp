/**
 * @file library.cpp
 *
 * Shared library access functions.
 *
 * @author  Jason Perkins (jason@379.com)
 * @date    Copyright (C) 2001 by 379, Inc.
 * @version $Id: library.cpp,v 1.3 2002/05/22 01:41:07 jason379 Exp $
 */
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <Platform/platform.h>
#include <Platform/library.h>
using namespace f4;


SharedLibrary f4::openLibrary(const Char* libraryName)
{
	Char winLibName[512];
	strcpy(winLibName, libraryName);
	strcat(winLibName, ".dll");

	SharedLibrary hDll = (SharedLibrary)LoadLibrary(winLibName);
	return hDll;
}

//---------------------------------------------------------------------------

void f4::closeLibrary(SharedLibrary library)
{
	FreeLibrary((HMODULE)library);
}

//---------------------------------------------------------------------------

bool f4::bindSymbol(void** ptr, SharedLibrary library, const Char* symbol)
{
	*ptr = GetProcAddress((HMODULE)library, symbol);
	return (*ptr != 0);
}

//---------------------------------------------------------------------------

void f4::normalizeLinkerBehavior(char*[])
{
}
