/**
 * @file library.h
 *
 * Shared library access functions.
 *
 * @author  Jason Perkins (jason@379.com)
 * @date    Copyright (C) 2001 by 379, Inc.
 * @version $Id: library.h,v 1.2 2002/04/22 01:51:55 jason379 Exp $
 */
#if !defined(_F4_PLATFORM_LIBRARY_H_)
#define _F4_PLATFORM_LIBRARY_H_

ENTER_NAMESPACE(f4);

/**
 * @defgroup f4_platform_lib Shared Libraries
 * @ingroup f4_platform
 * @{
 */


/// A handle to a shared library object.
typedef void* SharedLibrary;



/**
 * Load a shared library. The name given by @a libraryName should exclude
 * any system dependent prefix or extension, as these will be added by the
 * function itself. For example, the call openLibrary("Bin/Mylib") will load
 * the library "Bin/Mylib.dll" on Win32 systems, and "Bin/libMylib.so" on
 * POSIX systems.
 *
 * @returns A library handle if successful, or zero if the library could not
 *          be loaded.
 */
SharedLibrary openLibrary(const Char* libraryName);



/**
 * Unload a shared library. All bound pointers will become invalid.
 */
void closeLibrary(SharedLibrary library);



/**
 * Bind a pointer to a library entry point.
 *
 * @param ptr		returns a pointer to the library entry point
 * @param library   a library handle from openLibrary()
 * @param symbol	the name of the entry point
 *
 * @return True if successful, false if the entry point does not exist.
 */
bool bindSymbol(void** ptr, SharedLibrary library, const Char* symbol);



/**
 * Standardize the behavior of the library loader across platforms.
 * Under Win32, the directory containing the application executable
 * is considered part of the search path. This routine makes the
 * POSIX platforms behave the same way. Note that this function
 * may restart the program, which it must do to make the new search
 * path take effect.
 */
void normalizeLinkerBehavior(char* argv[]);


/** @} */

EXIT_NAMESPACE;
#endif
