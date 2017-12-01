/**
 * @file io.h
 *
 * @author  Jason Perkins (jason@379.com)
 * @date    Copyright (C) 2001 by 379, Inc.
 * @version $Id: io.h,v 1.2 2002/04/22 01:51:56 jason379 Exp $
 */
#if !defined(_F4_IO_H_)
#define _F4_IO_H_

#include <Core/file_server.h>
#include "input_stream.h"
#include "output_stream.h"
ENTER_NAMESPACE(f4);



/**
 * Data management service.
 * @ingroup f4_io
 */
struct IOService : public CoreFileServer
{

	/**
	 * Add a new file source. 
	 *
	 * @param name   A unique name for the mount point. If a previous mount
	 *               uses the same name it will be overwritten. 
	 *
	 * @param path   The mount point. This may be a directory or (eventually)
	 *               an archive like a .zip file.
	 *
	 * @return True if successful, false if the mount point could not be
	 *         found or accessed.
	 *
	 * Once the filesystem has been mounted, you can access it by name:
	 * @code
	 * Ref<InputStream> s = io:createInputStream("<I>fsname/filename</I>");
	 * @endcode
	 * 
	 */
	virtual bool mountFilesystem(const Char* name, const Char* path) = 0;


	/**
	 * Remove a file source.
	 */
	virtual void unmountFilesystem(const Char* name) = 0;


	/**
	 * Associate a file extension with a particular ReaderWriter service.
	 */
	virtual void associate(const Char* type, const Char* rw) = 0;


	/**
	 * Open an input stream on a file. For many services, you may use
	 * fromFile() instead.
	 */
	virtual InputStream* createInputStream(const Char* filename) = 0;


	/**
	 * Open an output stream to a new file.
	 */
	virtual OutputStream* createOutputStream(const Char* filename) = 0;


	/**
	 * Load an object from a disk file. Uses the file extension to locate
	 * a ReaderWriter.
	 */
	virtual Service* fromFile(const Char* filename) = 0;


	/**
	 * Load an object from an input stream.
	 * 
	 * @param s     The input stream.
	 * @param type  A type string used to locate a reader for this service.
	 *              This is usually the file extension associated with the
	 *              data type, such as "png" for PNG image files.
	 *
	 * @return The loaded service is successful, @c NULL otherwise.
	 */
	virtual Service* fromStream(InputStream* s, const Char* type) = 0;


	/**
	 * Write an object to a disk file. Uses the file extension to locate
	 * a ReaderWriter.
	 */
	virtual bool toFile(Service* srv, const Char* filename) = 0;


	/**
	 * Write an object to an output stream.
	 *
	 * @param srv   The object to write.
	 * @param os    The output stream
	 * @param type  A type string is used to locate a writer for this service.
	 *              This is usually the file extension associated with the 
	 *              file format, such as "png" for PNG image files.
	 */
	virtual bool toStream(Service* srv, OutputStream* os, const Char* type) = 0;


	/**
	 * Read and execute script code from an input stream.
	 */
	virtual bool runStream(InputStream* s, Context* context = 0) = 0;


	/**
	 * Access a directory of files.
	 */
	virtual Directory* getDirectory(const Char* path) = 0;

};


// --- begin scripting interface ---

SCRIPT_METHOD(IOService, associate, RETURNS_VOID)
	ARG_STRING, ARG_STRING
END_METHOD

SCRIPT_METHOD(IOService, createInputStream, RETURNS_SERVICE)
	ARG_STRING
END_METHOD

SCRIPT_METHOD(IOService, createOutputStream, RETURNS_SERVICE)
	ARG_STRING
END_METHOD

SCRIPT_METHOD(IOService, fromFile, RETURNS_SERVICE)
	ARG_STRING
END_METHOD

SCRIPT_METHOD(IOService, fromStream, RETURNS_SERVICE)
	ARG_SERVICE(InputStream), ARG_STRING
END_METHOD

SCRIPT_METHOD(IOService, getDirectory, RETURNS_SERVICE)
	ARG_STRING
END_METHOD

SCRIPT_METHOD(IOService, mountFilesystem, RETURNS_BOOL)
	ARG_STRING, ARG_STRING
END_METHOD

SCRIPT_METHOD(IOService, toFile, RETURNS_BOOL)
	ARG_SERVICE(Service), ARG_STRING
END_METHOD

SCRIPT_METHOD(IOService, toStream, RETURNS_BOOL)
	ARG_SERVICE(Service), ARG_SERVICE(OutputStream), ARG_STRING
END_METHOD

EXPORTS(IOService)
	METHOD(IOService, associate)
	METHOD(IOService, createInputStream)
	METHOD(IOService, createOutputStream)
	METHOD(IOService, fromFile)
	METHOD(IOService, fromStream)
	METHOD(IOService, getDirectory)
	METHOD(IOService, mountFilesystem)
	METHOD(IOService, toFile)
	METHOD(IOService, toStream)
END_EXPORTS

// --- end scripting interface ---


EXIT_NAMESPACE;
#endif
