/**
 * @file file.cpp
 *
 * <stdio> version of the low-level file I/O functions.
 *
 * @author  Jason Perkins (jason@379.com)
 * @date    Copyright (C) 2001 by 379, Inc.
 * @version $Id: file.cpp,v 1.3 2002/05/22 01:41:07 jason379 Exp $
 */
#include <stdio.h>
#include <string.h>
#include <io.h>

#include <Platform/platform.h>
#include <Platform/file.h>
using namespace f4;


File f4::fileOpen(const Char* path, const Char* access)
{
	const Char* flags;
	if (!strcmp(access, "in"))
		flags = "rb";
	else if (!strcmp(access, "out"))
		flags = "wb";
	else if (!strcmp(access, "inout"))
		flags = "w+b";
	else
		return 0;

	return (File)fopen(path, flags);
}

//---------------------------------------------------------------------------

void f4::fileClose(File file)
{
	fclose((FILE*)file);
}

//---------------------------------------------------------------------------

bool f4::fileEOF(File file)
{
	return (feof((FILE*)file) != 0);
}

//---------------------------------------------------------------------------

unsigned f4::fileRead(File file, void* buffer, unsigned num)
{
	return fread(buffer, 1, num, (FILE*)file);
}

//---------------------------------------------------------------------------

unsigned f4::fileWrite(File file, void* buffer, unsigned num)
{
	return fwrite(buffer, 1, num, (FILE*)file);
}

//---------------------------------------------------------------------------

bool f4::fileFlush(File file)
{
	return (fflush((FILE*)file) == 0);
}

//---------------------------------------------------------------------------

bool f4::fileSeek(File file, int offset, int origin)
{
	int whence;
	switch (origin)
	{
	case FILE_START:
		whence = SEEK_SET;
		break;

	case FILE_CURRENT:
		whence = SEEK_CUR;
		break;

	case FILE_END:
		whence = SEEK_END;
		break;

	default:
		return false;
	};

	return (fseek((FILE*)file, offset, whence) == 0);
}

//---------------------------------------------------------------------------

unsigned f4::fileTell(File file)
{
	return ftell((FILE*)file);
}

//---------------------------------------------------------------------------

bool f4::fileExists(const Char* file)
{
	return (_access(file, 0) == 0);
}
