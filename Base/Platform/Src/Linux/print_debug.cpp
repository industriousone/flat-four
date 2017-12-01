/**
 * @file print_debug.cpp
 *
 * POSIX version of the printDebug() functions.
 *
 * @author  Jason Perkins (jason@379.com)
 * @date    Copyright (C) 2001 by 379, Inc.
 * @version $Id: print_debug.cpp,v 1.2 2002/05/24 02:15:19 jason379 Exp $
 */
#include <stdio.h>
#include <Platform/platform.h>
#include <Platform/print_debug.h>
using namespace f4;


#if defined(DEBUG)

void f4::printDebug(const char* msg, ...)
{
	va_list vargs;
	va_start(vargs, msg);
	vfprintf(stderr, msg, vargs);
	va_end(vargs);

}

void f4::vprintDebug(const char* msg, va_list vargs)
{
	vfprintf(stderr, msg, vargs);
}


#endif

