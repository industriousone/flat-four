/**
 * @file library.cpp
 *
 * Linux version of the shared library functions.
 *
 * @author  Jason Perkins (jason@379.com)
 * @date    Copyright (C) 2001 by 379, Inc.
 * @version $Id: library.cpp,v 1.3 2002/05/24 02:15:19 jason379 Exp $
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dlfcn.h>
#include <string.h>

#include <Platform/platform.h>
#include <Platform/library.h>
#include <Platform/directory.h>
using namespace f4;


extern char** environ;


SharedLibrary f4::openLibrary(const char* libraryName)
{
	char sysLibPath[512];
	char sysLibName[64];

	splitPath(libraryName, DIRECTORY, sysLibPath, 512);
	splitPath(libraryName, BASENAME, sysLibName, 64);

	strcat(sysLibPath, "lib");
	strcat(sysLibPath, sysLibName);
	strcat(sysLibPath, ".so");

	SharedLibrary hDll = (SharedLibrary)dlopen(sysLibPath, RTLD_NOW);
	return hDll;
}

//---------------------------------------------------------------------------

void f4::closeLibrary(SharedLibrary library)
{
	dlclose(library);
}

//---------------------------------------------------------------------------

bool f4::bindSymbol(void** ptr, SharedLibrary library, const char* symbol)
{
	*ptr = dlsym(library, symbol);
	return (*ptr != 0);
}

//---------------------------------------------------------------------------

void f4::normalizeLinkerBehavior(char* argv[])
{
	// I'll need the executable name and directory
	const char *exeName = strrchr(argv[0], '/');
	if (!exeName) exeName = argv[0];

	char exeDir[512];
	getExecutableDirectory(argv[0], exeDir, 512);

	if (*exeDir == 0) return;

	// Is this directory already on the path?
	char path[1024] = { 0 };
	char* ldPath = getenv("LD_LIBRARY_PATH");
	if (ldPath)
	{
		if (strstr(ldPath, exeDir)) return;
		strcpy(path, ldPath);
	}

	// Nope, tack it on the end
	if (*path) strcat(path, ":");
	strcat(path, exeDir);
	setenv("LD_LIBRARY_PATH", path, 1);

	// Restart the app so the change will take effect
	strcpy(path, exeDir);
	strcat(path, "/");
	strcat(path, exeName);

	execve(path, argv, environ);
}

