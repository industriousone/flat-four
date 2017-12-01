/**
 * @file print_debug.h
 *
 * Display formatted message to the debug output target.
 *
 * @author  Jason Perkins (jason@379.com)
 * @date    Copyright (C) 2001 by 379, Inc.
 * @version $Id: print_debug.h,v 1.2 2002/04/22 01:51:55 jason379 Exp $
 */
#if !defined(_F4_PLATFORM_PRINT_DEBUG_H_)
#define _F4_PLATFORM_PRINT_DEBUG_H_

#include <stdarg.h>
ENTER_NAMESPACE(f4);


#if defined(DEBUG)

/**
 * Write a formatted string to the platform's idea of a debug output target,
 * using @c printf() style codes. The total length of the formatted string
 * must not exceed 1024 characters. This function becomes a no-op in release
 * builds.
 * @ingroup f4_platform_msg
 */
void printDebug(const Char* msg, ...);


/**
 * Write a formatted string to the platform's idea of a debug output target,
 * using @c printf() style codes. The total length of the formatted string
 * must not exceed 1024 characters. This function becomes a no-op in release
 * builds.
 * @ingroup f4_platform_msg
 */
void vprintDebug(const Char* msg, va_list vargs);


#else
inline void printDebug(const Char* msg, ...) { }
inline void vprintDebug(const Char* msg, va_list vargs) { }
#endif


EXIT_NAMESPACE;
#endif
