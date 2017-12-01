/**
 * @file directory.h
 *
 * @author  Jason Perkins (jason@379.com)
 * @date    Copyright (C) 2001 by 379, Inc.
 * @version $Id: directory.h,v 1.2 2002/04/22 01:51:56 jason379 Exp $
 *
 * @todo initialize(const Char*) should probably be hidden in the 
 *       implementation, not every type will use a const Char*
 *
 */
#if !defined(_F4_IO_DIRECTORY_H_)
#define _F4_IO_DIRECTORY_H_

#include "directory_iterator.h"
ENTER_NAMESPACE(f4);

struct InputStream;
struct OutputStream;


/**
 * A collection of files, roughly corresponding to a disk directory.
 * @ingroup f4_io
 */
struct Directory : public Service
{

	/**
	 * Set the full path of this directory. This method is called internally
	 * by IOService when a filesystem is mounted.
	 */
	virtual bool initialize(const Char* path) = 0;

	
	/**
	 * Open an input stream on a file contained in this directory.
	 */
	virtual InputStream* createInputStream(const Char* filename) = 0;


	/**
	 * Open an output stream to a new file in the directory.
	 */
	virtual OutputStream* createOutputStream(const Char* filename) = 0;


	/**
	 * Create a DirectoryIterator, which can be used to examine the contents
	 * of the directory.
	 */
	virtual DirectoryIterator* createIterator() = 0;


	/**
	 * Get a reference to a subdirectory.
	 */
	virtual Directory* getDirectory(const Char* path) = 0;


	/**
	 * Determine if a given subpath exists.
	 */
	virtual bool pathExists(const Char* path) = 0;

};



// --- begin scripting interface ---

EXIT_NAMESPACE;
#include "input_stream.h"
#include "output_stream.h"
ENTER_NAMESPACE(f4);

SCRIPT_METHOD(Directory, createInputStream, RETURNS_SERVICE)
	ARG_STRING
END_METHOD

SCRIPT_METHOD(Directory, createIterator, RETURNS_SERVICE)
END_METHOD

SCRIPT_METHOD(Directory, createOutputStream, RETURNS_SERVICE)
	ARG_STRING
END_METHOD

SCRIPT_METHOD(Directory, getDirectory, RETURNS_SERVICE)
	ARG_STRING
END_METHOD

SCRIPT_METHOD(Directory, pathExists, RETURNS_BOOL)
	ARG_STRING
END_METHOD

EXPORTS(Directory)
	METHOD(Directory, createInputStream)
	METHOD(Directory, createIterator)
	METHOD(Directory, createOutputStream)
	METHOD(Directory, getDirectory)
	METHOD(Directory, pathExists)
END_EXPORTS

// --- end scripting interface


EXIT_NAMESPACE;
#endif
