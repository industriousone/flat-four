/**
 * @file file.h
 *
 * @author  Jason Perkins (jason@379.com)
 * @date    Copyright (C) 2001 by 379, Inc.
 * @version $Id: file.h,v 1.2 2002/04/22 01:51:55 jason379 Exp $
 *
 * @todo Implement platform specific versions using native I/O functions.
 *
 */
#if !defined(_F4_PLATFORM_FILE_H_)
#define _F4_PLATFORM_FILE_H_

ENTER_NAMESPACE(f4);

typedef void* File;

enum { FILE_START, FILE_CURRENT, FILE_END };


/**
 * @defgroup f4_platform_file Files
 * @ingroup f4_platform
 * @{
 */


/**
 * Open a file.
 *
 * @param path   The path of the file.
 * @param access One of the following string values: "in" for input only, 
 *               "out" for output only (existing file will be lost), "inout" 
 *               for both input and output (existing file will be overwritten 
 *               but not lost).
 *
 * @return A new file object or @c NULL on error.
 */
File fileOpen(const Char* path, const Char* access);



/**
 * Close a file.
 */
void fileClose(File file);




/**
 * Determine if a file has reached it's end.
 */
bool fileEOF(File file);




/**
 * Read raw bytes from a file.
 *
 * @param file   The file to read from.
 * @param buffer A buffer to hold the data as it is read.
 * @param num    The number of bytes to read.
 *
 * @return The number of bytes actually read.
 */
unsigned fileRead(File file, void* buffer, unsigned num);




/**
 * Write raw bytes into a file.
 *
 * @param file   The file to write into.
 * @param buffer The bytes to be written.
 * @param num    The number of bytes to write.
 *
 * @return The number of bytes actually written.
 */
unsigned fileWrite(File file, void* buffer, unsigned num);



/**
 * Flush all buffered output to the file.
 *
 * @param file   The file to flush.
 *
 * @return True if successful, false if an error occurred.
 */
bool fileFlush(File file);



/**
 * Move the file's current read/write position.
 *
 * @param file   The file object.
 * @param offset The new read/write offset from the specified @a origin.
 * @param origin One of the constants @a FILE_START, @a FILE_CURRENT, or
 *               @a FILE_END.
 *
 * @return True if successful, false on error.
 */
bool fileSeek(File file, int offset, int origin);



/**
 * Get a file's current read/write position.
 */
unsigned fileTell(File file);



/**
 * Determine if a given file exists and is accessible.
 */
bool fileExists(const Char* file);



/** @} */

EXIT_NAMESPACE;
#endif
