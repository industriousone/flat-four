/**
 * @file directory.h
 *
 * Low-level management of file directories and path names.
 *
 * @author  Jason Perkins (jason@379.com)
 * @date    Copyright (C) 2001 by 379, Inc.
 * @version $Id: directory.h,v 1.2 2002/04/22 01:51:55 jason379 Exp $
 */
#if !defined(_F4_PLATFORM_DIRECTORY_H_)
#define _F4_PLATFORM_DIRECTORY_H_

ENTER_NAMESPACE(f4);

typedef void* DirIdx;


/**
 * @defgroup f4_platform_dir Directories
 * @ingroup f4_platform
 * @{
 */



/**
 * Change the current working directory.
 */
bool changeDirectory(const Char* dir);




/**
 * Copy the current working directory path into a buffer.
 */
void getCurrentDirectory(Char* buffer, int bufferSize);




/**
 * Given the argv[0] parameter supplied to main(), locate the directory
 * containing the program's executable file.
 */
void getExecutableDirectory(const char* argv0, Char* buffer, int bufferSize);




/**
 * Retrieve the system's path separation character. For Unix and BeOS, this
 * will return the forward slash '/', for Win32 the backslash '\'. MacOS,
 * when it is supported, will return ':'.
 */
char getSystemPathSeparator();




/**
 * Extract a specific portion of a path. Returns the requested portion in
 * the generic path format (using '/' to separate directories).
 *
 * @param path		the path
 * @param part		the section to extract:
 *                   - @c DIRECTORY: the directory list
 *                   - @c FILENAME:  the file name and extension
 *                   - @c BASENAME:  the file name with no extension
 *                   - @c EXTENSION: the file extension
 * @param buffer	buffer to hold the extracted result.
 * @param bufSize	the size of the buffer.
 */
void splitPath(const Char* path, int part, Char* buffer, int bufSize);
enum { DIRECTORY, FILENAME, BASENAME, EXTENSION };




/**
 * Convert from the generic path seperator ('/') to the system's native
 * path format ('\' on Win32, '/' on POSIX, ':' on MacOS.
 */
void makeNativePath(Char* path);




/**
 * Convert from the system's native path separator character to the generic
 * forward slash '/'.
 */
void makeGenericPath(Char* path);




/**
 * Create an directory index, which can be iterated to examine it's contents.
 */
DirIdx dirOpen(const Char* path);




/**
 * Close a directory index.
 */
void dirClose(DirIdx idx);




/**
 * Move to the next directory entry.
 *
 * @return False when there are no more entries.
 */
bool dirNextEntry(DirIdx idx);




/**
 * Get the name of the current directory entry.
 */
const Char* dirGetEntryName(DirIdx idx);



/** @} */

EXIT_NAMESPACE;
#endif
