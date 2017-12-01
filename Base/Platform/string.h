/**
 * @file string.h
 *
 * @author  Jason Perkins (jason@379.com)
 * @date    Copyright (C) 2001 by 379, Inc.
 * @version $Id: string.h,v 1.3 2002/06/12 23:05:16 jason379 Exp $
 *
 * @todo A real string object/API is needed. One that handles UNICODE would
 *       be ideal.
 *
 */
#if !defined(_F4_PLATFORM_STRING_H_)
#define _F4_PLATFORM_STRING_H_

ENTER_NAMESPACE(f4);


/**
 * @defgroup f4_platform_string String Handling
 * @ingroup f4_platform
 * @{
 */


/**
 * Convert a string to uppercase.
 */
void stringUpper(Char* s);



/**
 * Convert a string to lowercase.
 */
void stringLower(Char* s);


EXIT_NAMESPACE;
#endif
