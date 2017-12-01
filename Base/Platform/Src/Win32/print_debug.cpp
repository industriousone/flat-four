/**
 * @file print_debug.cpp
 *
 * Win32 version of the printDebug functions.
 *
 * @author  Jason Perkins (jason@379.com)
 * @date    Copyright (C) 2001 by 379, Inc.
 * @version $Id: print_debug.cpp,v 1.3 2002/05/22 01:41:07 jason379 Exp $
 */
#include <Platform/platform.h>
#include <Platform/print_debug.h>
#include <stdio.h>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
using namespace f4;


#if defined(DEBUG)

void f4::printDebug(const Char* msg, ...)
{
	va_list vargs;
	va_start(vargs, msg);
	vprintDebug(msg, vargs);
	va_end(vargs);

}

void f4::vprintDebug(const Char* msg, va_list vargs)
{
	Char buffer[1024];
	vsprintf(buffer, (Char*)msg, vargs);
	OutputDebugString(buffer);
}
	

#endif

