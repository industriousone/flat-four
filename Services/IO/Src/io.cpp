/**
 * @file io.cpp
 *
 * @author  Jason Perkins (jason@379.com)
 * @date    Copyright (C) 2001 by 379, Inc.
 * @version $Id: io.cpp,v 1.2 2002/04/22 01:51:56 jason379 Exp $
 *
 * @todo Create a constant MAX_PATH somewhere and use it.
 *
 * @todo Had to put STL headers after <core.h> to avoid dumb MSVC warnings.
 *       Standardize behavior for this case.
 *
 * @todo Add support for filesystems: ZIP files, network mounts
 *
 */
#include <string.h>

#include <Core/core.h>

#include <list>
#include <map>

#include <IO/io.h>
#include <IO/reader_writer.h>
#include <Script/script.h>
#include <Platform/directory.h>
#include <Platform/string.h>
#include <Memory/memory.h>
using namespace f4;

struct DiskDirectory;
DECLARE_SERVICE(DiskDirectory)



struct IOImpl : public IOService
{
	IOImpl();
	virtual ~IOImpl();

	virtual bool mountFilesystem(const Char* name, const Char* path);
	virtual void unmountFilesystem(const Char* name);

	virtual void associate(const Char* type, const Char* rw);

	virtual InputStream*  createInputStream(const Char* filename);
	virtual OutputStream* createOutputStream(const Char* filename);

	virtual Service* fromFile(const Char* filename);
	virtual Service* fromStream(InputStream* s, const Char* type);
	virtual bool toFile(Service* srv, const Char* filename);
	virtual bool toStream(Service* srv, OutputStream* os, const Char* type);

	virtual bool runFile(const Char* filename, Context* context);
	virtual bool runStream(InputStream* s, Context* context);

	virtual Directory* getDirectory(const Char* path);

	virtual bool pathExists(const Char* path);

private:
	// Filesystem mount points
	typedef std::map< std::string, Ref<Directory> > Mounts;
	Mounts m_mounts;

	// The "current directory stack"
	typedef std::list< Ref<Directory> > CwdStack;
	CwdStack m_cwdStack;

	// Type->ReaderWriter registry
	typedef std::map<std::string, std::string> Registry;
	Registry m_registry;

	Directory*    _findMountPoint(const Char** path);
	ReaderWriter* _findReaderWriter(const Char* type);

	ScriptService* m_scriptService;

	DECL_SERVICE;
};

IMPL_SINGLETON_SERVICE(IOImpl, IOService)



//---------------------------------------------------------------------------
// Message text
//---------------------------------------------------------------------------

static const Char* START_MSG = "Initializing I/O service\n";
static const Char* STOP_MSG  = "Closing I/O service\n";
static const Char* NO_RW     = "Warning: no ReaderWriter found for type '%s'\n";



//---------------------------------------------------------------------------

IOImpl::IOImpl()
{
	Core->print(START_MSG);

	// Push the cwd on the current stack
	char buffer[1024];
	getCurrentDirectory(buffer, 1024);
	Ref<Directory> d = (Directory*)CREATEA_LOCAL(DiskDirectory);
	if (d->initialize(buffer))
		m_cwdStack.push_front(d);

	// Add some default directories to the list of search paths
	mountFilesystem("BIN", Core->getBinDirectory());
	mountFilesystem("HOME", Core->getStartDirectory());

	// Register with CoreService so I can handle it's file requests
	m_scriptService = Core->registerFileServer(this);
}

//---------------------------------------------------------------------------

IOImpl::~IOImpl()
{
	Core->print(STOP_MSG);
	Core->registerFileServer(0);
}

//---------------------------------------------------------------------------

bool IOImpl::mountFilesystem(const Char* name, const Char* path)
{
	// I am only supporting disk directories at the moment, no .zip files
	// or anything like that yet.

	// Does 'path' refer to an existing mount point?
	const Char* ptr = path;
	Directory* mount = _findMountPoint(&ptr);
	if (mount)
	{
		Directory* d = mount->getDirectory(ptr);
		if (d)
		{
			m_mounts.insert(Mounts::value_type(name, d));
			return true;
		}
	}
	
	// Doesn't refer to an existing mount point, try to access the
	// directory as an absolute path
	Directory* d = (Directory*)CREATEA_LOCAL(DiskDirectory);
	if (!d->initialize(path))
	{
		d->release();
		return false;
	}

	m_mounts.insert(Mounts::value_type(name, d));
	return true;
}

//---------------------------------------------------------------------------

void IOImpl::unmountFilesystem(const Char* name)
{
	Mounts::iterator i = m_mounts.find(name);
	if (i != m_mounts.end())
	{
		m_mounts.erase(i);
	}
}

//---------------------------------------------------------------------------

void IOImpl::associate(const Char* type, const Char* rw)
{
	// Convert to a standard case
	char correct[64];
	strcpy(correct, type);
	stringUpper(correct);

	std::string t(correct);
	std::string r(rw);

	m_registry.insert(Registry::value_type(t,r));
}

//---------------------------------------------------------------------------

InputStream* IOImpl::createInputStream(const Char* filename)
{
	Directory* d = _findMountPoint(&filename);
	if (!d) return 0;

	return d->createInputStream(filename);
}

//---------------------------------------------------------------------------

OutputStream* IOImpl::createOutputStream(const Char* filename)
{
	Directory* d = _findMountPoint(&filename);
	if (!d) return 0;

	return d->createOutputStream(filename);
}

//---------------------------------------------------------------------------

Service* IOImpl::fromFile(const Char* filename)
{
	Ref<InputStream> s = createInputStream(filename);
	if (!s) return 0;

	// Pull off the file extension to identify the data type
	Char ext[32];
	splitPath(filename, EXTENSION, ext, 32);

	return fromStream(s, ext + 1);  // +1 to skip the leading '.'
}

//---------------------------------------------------------------------------

Service* IOImpl::fromStream(InputStream* s, const Char* type)
{
	Ref<ReaderWriter> rw = _findReaderWriter(type);
	if (rw) return rw->read(s);

	Core->print(NO_RW, type);
	return 0;
}

//---------------------------------------------------------------------------

bool IOImpl::toFile(Service* srv, const Char* filename)
{
	Ref<OutputStream> os = createOutputStream(filename);
	if (!os) return false;

	// Pull off the file extension to identify the data type
	Char ext[32];
	splitPath(filename, EXTENSION, ext, 32);

	return toStream(srv, os, ext + 1);  // +1 to skip the leading '.'
}

//---------------------------------------------------------------------------

bool IOImpl::toStream(Service* srv, OutputStream* os, const Char* type)
{
	Ref<ReaderWriter> rw = _findReaderWriter(type);
	if (rw) return rw->write(srv, os);

	Core->print(NO_RW, type);
	return 0;
}

//---------------------------------------------------------------------------

bool IOImpl::runFile(const Char* filename, Context* context)
{
	Ref<InputStream> s = createInputStream(filename);
	return (s) ? runStream(s, context) : false;
}

//---------------------------------------------------------------------------

bool IOImpl::runStream(InputStream* s, Context* context)
{
	// Push this directory to the front of the current stack. This allows
	// the script to reference files relative to itself
	m_cwdStack.push_front(s->getDirectory());

	// Perhaps a bit of a hack - load the entire script contents into
	// a memory buffer, then execute the buffer. Start by sizing up the
	// file - this may not work if I implement net streams, we'll see
	UInt32 pos = s->tell();
	s->seek(0, InputStream::END);
	UInt32 len = s->tell() - pos + 1;
	s->seek(pos, InputStream::START);

	// Create a buffer and load the code
	void* buffer = malloc(len);
	len = s->read(buffer, len);

	// Execute it
	bool result = m_scriptService->runBuffer(buffer, len, s->getSourceName(), context);

	// Clean up and exit
	free(buffer);
	m_cwdStack.pop_front();
	return result;
}

//---------------------------------------------------------------------------

Directory* IOImpl::getDirectory(const Char* path)
{
	const Char* ptr = path;
	Directory* mount = _findMountPoint(&ptr);
	if (mount)
	{
		Directory* d = mount->getDirectory(ptr);
		if (d) return d;
	}

	// Doesn't refer to an existing mount point, try to access the
	// directory as an absolute path
	Directory* d = (Directory*)CREATEA_LOCAL(DiskDirectory);
	if (!d->initialize(path))
	{
		d->release();
		d = 0;
	}

	return d;
}

//---------------------------------------------------------------------------

bool IOImpl::pathExists(const Char* path)
{
	Mounts::iterator m;
	for (m = m_mounts.begin(); m != m_mounts.end(); ++m)
		if ((*m).second->pathExists(path)) return true;

	CwdStack::iterator c;
	for (c = m_cwdStack.begin(); c != m_cwdStack.end(); ++c)
		if ((*c)->pathExists(path)) return true;

	return false;
}

//---------------------------------------------------------------------------

/*
 * Attempts to find the mount point associated with 'path'. If path starts
 * with a mount point name, the pointer is adjusted to point to path after
 * the name.
 */
Directory* IOImpl::_findMountPoint(const Char** path)
{
	// Look for a name
	const Char* ptr = strchr(*path, '/');
	if (ptr)
	{
		// Extract the mount point name from the path
		Char name[92];
		int len = ptr - (*path);
		strncpy(name, *path, len);
		name[len] = 0;

		// The wildcard '*' will search all mount points 
		if (*name == '*' && len == 1)
		{
			Mounts::iterator i;
			for (i = m_mounts.begin(); i != m_mounts.end(); ++i)
			{
				if ((*i).second->pathExists(ptr + 1))
				{
					*path = ptr + 1;
					return (*i).second;
				}
			}

			return 0;
		}

		// Not a wildcard, look up a specific mount point
		Mounts::iterator i = m_mounts.find(name);
		if (i != m_mounts.end())
		{
			*path = ptr + 1;
			return (*i).second;
		}
	}

	// No mount point found, return current directory
	if (!m_cwdStack.empty())
	{
		CwdStack::iterator i = m_cwdStack.begin();
		return (*i);
	}

	return 0;
}

//---------------------------------------------------------------------------

ReaderWriter* IOImpl::_findReaderWriter(const Char* type)
{
	ReaderWriter* rw = 0;

	// Convert the type code to a known case
	Char typeCode[32];
	strcpy(typeCode, type);
	stringUpper(typeCode);

	// See if there is an entry in the registry for this type
	Registry::iterator i = m_registry.find(typeCode);
	if (i != m_registry.end())
	{
		rw = (ReaderWriter*)Core->createA((*i).second.c_str());
		if (rw) return rw;
	}

	// Look for a package specific to this type
	Char service[256];
	strcpy(service, typeCode);
	strcat(service, ".ReaderWriter");

	rw = (ReaderWriter*)Core->createA(service);
	return rw;
}

