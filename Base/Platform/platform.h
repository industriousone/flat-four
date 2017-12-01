/**
 * @file platform.h
 *
 * Set up the F4 Engine build environment.
 *
 * @author  Jason Perkins (jason@379.com)
 * @date    Copyright (C) 2001 by 379, Inc.
 * @version $Id: platform.h,v 1.2 2002/06/12 23:05:15 jason379 Exp $
 *
 * @todo Char should really be 'unsigned char', not 'char'. Requires some
 *       new string handling routines, casting macros, etc. 
 *
 * @todo I think I should provide a complete replacement for the stdc
 *       routines. Just a hunch.
 *
 */
#if !defined(_F4_PLATFORM_H_)
#define _F4_PLATFORM_H_

// Try to come up with some symbols for each supported platform
#if defined(WIN32)
	#define F4_ISWIN32
#elif defined(__GNUC__) && defined(__BEOS__)
	#define F4_ISBEOS
#elif defined(__GNUC__)
	#define F4_ISLINUX
#endif


// Linkage specifications for DLL symbols
#if !defined(__GNUC__) || defined(__BEOS__)
	#define F4_IMPORT	extern "C" __declspec(dllimport)
	#define F4_EXPORT	extern "C" __declspec(dllexport)
#else
	#define F4_IMPORT	extern "C"
	#define F4_EXPORT	extern "C"
#endif


// Define a standard symbol for debug and release builds
#if defined(_DEBUG) && !defined(DEBUG)
	#define DEBUG
#endif

#if !defined(DEBUG) && !defined(NDEBUG)
	#define NDEBUG
#endif


// Disable some problematic MSVC++ warnings
#if defined(WIN32)
	#pragma warning(disable:4786)   // debug symbols longer than 255 chars
	#pragma warning(disable:4503)   // decorated name length exceeded
	#pragma warning(disable:4273)   // inconsistent dll linkage. dllexport assumed
	#pragma warning(disable:4244)   // conversion from double to float
	#pragma warning(disable:4305)   // truncation of double to float
#endif


// I don't like indenting the contents of namespaces, these macros prevent
// code editors from seeing the braces and trying to autoindent everything
#define ENTER_NAMESPACE(space)   namespace space {
#define EXIT_NAMESPACE           }



// Create some well-defined atomic types for when size matters
ENTER_NAMESPACE(f4);

typedef char Int8;              ///< An 8 bit signed integer.
typedef unsigned char UInt8;    ///< An 8 bit unsigned integer.
typedef short Int16;            ///< A 16 bit signed integer.
typedef unsigned short UInt16;  ///< A 16 bit unsigned integer.
typedef int Int32;              ///< A 32 bit signed integer.
typedef unsigned int UInt32;    ///< A 32 bit unsigned integer.
typedef float Float;            ///< A single-precision floating point number.
typedef double Double;          ///< A double-precision floating point number.
typedef char Char;              ///< A single character.


EXIT_NAMESPACE;
#endif
