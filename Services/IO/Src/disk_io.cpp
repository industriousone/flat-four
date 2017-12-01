/**
 * @file disk_io.cpp
 *
 * @author  Jason Perkins (jason@379.com)
 * @date    Copyright (C) 2001 by 379, Inc.
 * @version $Id: disk_io.cpp,v 1.2 2002/04/22 01:51:56 jason379 Exp $
 *
 * @todo Move core of createInputStream() and createOutputStream()
 *       to common function.
 *
 */
#include <string.h>

#include <Core/core.h>
#include <IO/io.h>
#include <IO/raw_stream.h>
#include <Platform/directory.h>
#include <Platform/file.h>
#include <Memory/memory.h>
using namespace f4;

DECLARE_SERVICE(InputStream)
DECLARE_SERVICE(OutputStream)


struct DiskDirectory : public Directory
{
	DiskDirectory();
	virtual ~DiskDirectory();

	virtual bool initialize(const Char* path);
	virtual InputStream* createInputStream(const Char* filename);
	virtual OutputStream* createOutputStream(const Char* filename);
	virtual DirectoryIterator* createIterator();
	virtual Directory* getDirectory(const Char* path);
	virtual bool pathExists(const Char* path);

	Char* m_path;
	friend struct DiskIterator;
	DECL_SERVICE;
};

IMPL_ANON_SERVICE(DiskDirectory, Directory)

//---------------------------------------------------------------------------

struct DiskIterator : public DirectoryIterator
{
	DiskIterator() { }
	DiskIterator(DiskDirectory* dir, const Char* path);
	virtual ~DiskIterator();

	virtual const Char* getName();
	virtual bool next();
	virtual bool rewind();

	Ref<DiskDirectory> m_dir;
	DirIdx m_idx;
	DECL_SERVICE;
};

IMPL_ANON_SERVICE(DiskIterator, DirectoryIterator)

//---------------------------------------------------------------------------

struct DiskStream : public RawStream
{
	DiskStream();
	virtual ~DiskStream();

	virtual bool     eof();
	virtual unsigned read (void* buffer, unsigned num);
	virtual unsigned write(void* buffer, unsigned num);
	virtual bool     flush();
	virtual bool     seek(int offset, int origin);
	virtual unsigned tell();

	File m_file;
	DECL_SERVICE;
};

IMPL_ANON_SERVICE(DiskStream, RawStream);



//---------------------------------------------------------------------------
// DiskDirectory
//---------------------------------------------------------------------------

DiskDirectory::DiskDirectory()
: m_path(0)
{
}

//---------------------------------------------------------------------------

DiskDirectory::~DiskDirectory()
{
	if (m_path) free(m_path);
}

//---------------------------------------------------------------------------

bool DiskDirectory::initialize(const Char* path)
{
	// Make sure the directory actually exists
	if (!changeDirectory(path)) return false;

	// Get the full path to the directory and copy to my own buffer
	char buffer[512];
	getCurrentDirectory(buffer, 512);

	int len = strlen(buffer);
	m_path = (char*)malloc(len + 2);
	strcpy(m_path, buffer);

	// Make sure the path has a trailing '/'
	if (m_path[len - 1] != '/')
	{
		m_path[len++] = '/';
		m_path[len]   = 0;
	}

	return true;
}

//---------------------------------------------------------------------------

InputStream* DiskDirectory::createInputStream(const Char* filename)
{
	// Build a complete path and try to open it
	char buffer[4096];
	strcpy(buffer, m_path);
	strcat(buffer, filename);

	File file = fileOpen(buffer, "in");
	if (!file) return 0;

	// Wrap the file in a disk stream
	DiskStream* ds = new DiskStream;
	ds->m_file = file;

	// Set the directory associated with the stream. If additional
	// directory information was supplied in 'filename', then create a
	// new Directory object, else use this one.
	DiskDirectory* dir;

	char path[4096];
	splitPath(buffer, DIRECTORY, path, 4096);
	if (strcmp(path, m_path) == 0)
		dir = this;
	else
	{
		dir = new DiskDirectory;
		dir->initialize(path);
	}

	// Now create and initialize the new input stream
	InputStream* s = CREATEA_LOCAL(InputStream);
	s->initialize(ds, dir, filename);
	return s;
}

//---------------------------------------------------------------------------

OutputStream* DiskDirectory::createOutputStream(const Char* filename)
{
	// Build a complete path and try to open it
	char buffer[4096];
	strcpy(buffer, m_path);
	strcat(buffer, filename);

	File file = fileOpen(buffer, "out");
	if (!file) return 0;

	// Wrap the file in a disk stream
	DiskStream* ds = new DiskStream;
	ds->m_file = file;

	// Set the directory associated with the stream. If additional
	// directory information was supplied in 'filename', then create
	// a new Directory object, else use this one.
	DiskDirectory* dir;

	char path[4096];
	splitPath(buffer, DIRECTORY, path, 4096);
	if (strcmp(path, m_path) == 0)
		dir = this;
	else
	{
		dir = new DiskDirectory;
		dir->initialize(path);
	}

	// Now create and initialize the new output stream
	OutputStream* s = CREATEA_LOCAL(OutputStream);
	s->initialize(ds, dir, filename);
	return s;
}

//---------------------------------------------------------------------------

DirectoryIterator* DiskDirectory::createIterator()
{
	return new DiskIterator(this, m_path);
}

//---------------------------------------------------------------------------

Directory* DiskDirectory::getDirectory(const Char* path)
{
	// Get to the requested directory
	changeDirectory(m_path);
	if (!changeDirectory(path)) return 0;

	// Read out the path
	char buffer[512];
	getCurrentDirectory(buffer, 512);

	Directory* d = new DiskDirectory();
	d->initialize(buffer);
	return d;
}

//---------------------------------------------------------------------------

bool DiskDirectory::pathExists(const Char* path)
{
	changeDirectory(m_path);

	File f = fileOpen(path, "in");
	if (f)
	{
		fileClose(f);
		return true;
	}

	return changeDirectory(path);
}




//---------------------------------------------------------------------------
// DiskIterator
//---------------------------------------------------------------------------

DiskIterator::DiskIterator(DiskDirectory* dir, const Char* path)
{
	m_dir = dir;
	m_idx = dirOpen(path);	
}

//---------------------------------------------------------------------------

DiskIterator::~DiskIterator()
{
	dirClose(m_idx);
}

//---------------------------------------------------------------------------

const Char* DiskIterator::getName()
{
	return dirGetEntryName(m_idx);
}

//---------------------------------------------------------------------------

bool DiskIterator::next()
{
	return dirNextEntry(m_idx);
}

//---------------------------------------------------------------------------

bool DiskIterator::rewind()
{
	dirClose(m_idx);
	m_idx = dirOpen(m_dir->m_path);
	return true;
}



//---------------------------------------------------------------------------
// DiskStream
//---------------------------------------------------------------------------

DiskStream::DiskStream()
: m_file(0)
{
}

//---------------------------------------------------------------------------

DiskStream::~DiskStream()
{
	fileClose(m_file);
}

//---------------------------------------------------------------------------

bool DiskStream::eof()
{
	return fileEOF(m_file);
}

//---------------------------------------------------------------------------

unsigned DiskStream::read(void* buffer, unsigned num)
{
	return fileRead(m_file, buffer, num);
}

//---------------------------------------------------------------------------

unsigned DiskStream::write(void* buffer, unsigned num)
{
	return fileWrite(m_file, buffer, num);
}

//---------------------------------------------------------------------------

bool DiskStream::flush()
{
	return fileFlush(m_file);
}

//---------------------------------------------------------------------------

bool DiskStream::seek(int offset, int origin)
{
	return fileSeek(m_file, offset, origin);
}

//---------------------------------------------------------------------------

unsigned DiskStream::tell()
{
	return fileTell(m_file);
}
