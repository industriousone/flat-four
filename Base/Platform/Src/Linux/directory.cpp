/*
 * @file directory.cpp
 *
 * Low-level management of file directories and path names.
 *
 * @author  Jason Perkins (jason@379.com)
 * @date    Copyright (C) 2001 by 379, Inc.
 * @version $Id: directory.cpp,v 1.3 2002/05/24 02:15:19 jason379 Exp $
 */
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>

#include <Platform/platform.h>
#include <Platform/directory.h>
#include <Platform/file.h>
using namespace f4;


bool f4::changeDirectory(const char* dir)
{
	return (chdir(dir) == 0);
}

//---------------------------------------------------------------------------

void f4::getCurrentDirectory(char* buffer, int bufferSize)
{
	getcwd(buffer, bufferSize);
}

//---------------------------------------------------------------------------

void f4::getExecutableDirectory(const char* argv0, Char* buffer, int bufferSize)
{
	// Check to see if the path to the file is contained in argv[0]
	if (fileExists(argv0))
	{
		// Remember the current directory...
		char curDir[256];
		getcwd(curDir, 256);

		// chdir() to the exe directory and grab the full path
		strcpy(buffer, argv0);
		char* p = strrchr(buffer, '/');
		if (p)
		{
			*p = 0;
			chdir(buffer);
		}
		getcwd(buffer, bufferSize);

		// restore the original directory
		chdir(curDir);
	}
	else
	{
		// Not in argv[0], search the path
		const char *exeName = strrchr(argv0, '/');
		if (!exeName) exeName = argv0;

		char path[1024];
		strcpy(path, getenv("PATH"));
		char* start = path;
		while (*start)
		{
			char* end = strchr(start, ':');
			if (end)
				*end = 0;
			else
				end = start + strlen(start) - 1;

			strcpy(buffer, start);
			strcat(buffer, "/");
			strcat(buffer, exeName);

			if (fileExists(buffer))
				break;

			start = end + 1;
		}

		if (start)
			strcpy(buffer, start);
		else
			*buffer = 0;
	}
}

//---------------------------------------------------------------------------

void f4::splitPath(const char* path, int part, char* buffer, int bufSize)
{
	// Assume that something will go wrong
	*buffer = 0;

	// Translate the path to the system path format. This is the only
	// way I can be sure what I'm dealing with.
	char* workPath = new char[strlen(path) + 1];
	strcpy(workPath, path);
	makeNativePath(workPath);

	// Locate the last path separator
	char* filename = strrchr(workPath, '/');
	if (filename)
		++filename;

	// Locate the last '.'
	char* extension = strrchr(workPath, '.');

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
		if (extension) strncpy(buffer, extension, bufSize);
		break;
	}

	delete workPath;
}

//---------------------------------------------------------------------------

void f4::makeNativePath(char* path)
{
}

//---------------------------------------------------------------------------

void f4::makeGenericPath(char* path)
{
}



//---------------------------------------------------------------------------
// Directory indexing
//---------------------------------------------------------------------------

struct Index
{
	DIR* dir;
	dirent* entry;
};

DirIdx f4::dirOpen(const Char* path)
{
	DIR* d = opendir(path);
	if (d)
	{
		Index* i = (Index*)malloc(sizeof(Index));
		i->dir = d;
		i->entry = readdir(i->dir);
	}

	return d;
}

//---------------------------------------------------------------------------

void f4::dirClose(DirIdx idx)
{
	Index* i = (Index*)idx;
	if (i)
	{
		closedir(i->dir);
		free(i);
	}
}

//---------------------------------------------------------------------------

bool f4::dirNextEntry(DirIdx idx)
{
	Index* i = (Index*)idx;
	i->entry = readdir(i->dir);
	return (i->entry != NULL);
}

//---------------------------------------------------------------------------

const Char* f4::dirGetEntryName(DirIdx idx)
{
	Index* i = (Index*)idx;
	return (i->entry) ? i->entry->d_name : 0;
}

