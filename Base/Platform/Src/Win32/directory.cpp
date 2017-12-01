/**
 * @file directory.cpp
 *
 * Low-level management of file directories and path names.
 *
 * @author  Jason Perkins (jason@379.com)
 * @date    Copyright (C) 2001 by 379, Inc.
 * @version $Id: directory.cpp,v 1.3 2002/05/22 01:41:07 jason379 Exp $
 */
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdlib.h>
#include <direct.h>
#include <string.h>

#include <Platform/platform.h>
#include <Platform/directory.h>
using namespace f4;


bool f4::changeDirectory(const Char* dir)
{
	return (_chdir(dir) == 0);
}

//---------------------------------------------------------------------------

void f4::getCurrentDirectory(Char* buffer, int bufferSize)
{
	_getcwd(buffer, bufferSize);
}

//---------------------------------------------------------------------------

void f4::getExecutableDirectory(const Char*, Char* buffer, int bufferSize)
{
	GetModuleFileName(NULL, buffer, bufferSize);
	char* ptr = strrchr(buffer, '\\');
	if (ptr) 
		*(++ptr) = 0;
}

//---------------------------------------------------------------------------

char f4::getSystemPathSeparator()
{
	return '\\';
}

//---------------------------------------------------------------------------

void f4::splitPath(const Char* path, int part, Char* buffer, int bufSize)
{
	// Assume that something will go wrong
	*buffer = 0;

	// Translate the path to the generic path format
	Char* workPath = (char*)malloc(strlen(path) + 1);
	strcpy(workPath, path);
	makeGenericPath(workPath);

	// Locate the last path separator
	Char* filename = strrchr(workPath, '/');
	if (filename)
		++filename;

	// Locate the last '.'
	Char* extension = strrchr(workPath, '.');

	// Return the appropriate portion
	*buffer = 0;
	switch (part)
	{
	case DIRECTORY:
		if (filename)
		{
			*filename = 0;
			strncpy(buffer, workPath, bufSize);
		}
		break;

	case FILENAME:
		strncpy(buffer, (filename) ? filename : workPath, bufSize);
		break;

	case BASENAME:
		if (extension)
			*extension = 0;
		strncpy(buffer, (filename) ? filename : workPath, bufSize);
		break;

	case EXTENSION:
		if (extension)
			strncpy(buffer, extension, bufSize);
		break;
	}

	free(workPath);
}

//---------------------------------------------------------------------------

void f4::makeNativePath(Char* path)
{
	for (Char* ptr = path; *ptr; ++ptr)
		if (*ptr == '/')
			*ptr = '\\';
}

//---------------------------------------------------------------------------

void f4::makeGenericPath(Char* path)
{
	for (Char* ptr = path; *ptr; ++ptr)
		if (*ptr == '\\')
			*ptr = '/';
}



//---------------------------------------------------------------------------
// Directory indexing
//---------------------------------------------------------------------------

struct Index
{
	HANDLE handle;
	WIN32_FIND_DATA data;
};

DirIdx f4::dirOpen(const Char* path)
{
	// Build a search mask
	char mask[512];
	strcpy(mask, path);
	makeNativePath(mask);
	if (mask[strlen(mask)-1] != '\\')
		strcat(mask, "\\");
	strcat(mask, "*.*");

	Index* i = (Index*)malloc(sizeof(Index));
	i->handle = FindFirstFile(mask, &i->data);
	if (i->handle == INVALID_HANDLE_VALUE)
	{
		free(i);
		i = 0;
	}

	return (DirIdx*)i;
}

//---------------------------------------------------------------------------

void f4::dirClose(DirIdx idx)
{
	Index* i = (Index*)idx;
	if (i)
	{
		FindClose(i->handle);
		free(i);
	}
}

//---------------------------------------------------------------------------

bool f4::dirNextEntry(DirIdx idx)
{
	Index* i = (Index*)idx;
	return (FindNextFile(i->handle, &i->data) != 0);
}

//---------------------------------------------------------------------------

const Char* f4::dirGetEntryName(DirIdx idx)
{
	Index* i = (Index*)idx;
	return i->data.cFileName;
}
