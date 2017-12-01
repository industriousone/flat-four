/**
 * @file print_alert.cpp
 *
 * Win32 version of the printAlert functions.
 *
 * @author  Jason Perkins (jason@379.com)
 * @date    Copyright (C) 2001 by 379, Inc.
 * @version $Id: print_alert.cpp,v 1.3 2002/05/22 01:41:07 jason379 Exp $
 */
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>
#include <Platform/platform.h>
#include <Platform/print_alert.h>
using namespace f4;


void f4::printAlert(const Char* caption, const Char* msg, ...)
{
	va_list vargs;
	va_start(vargs, msg);
	vprintAlert(caption, msg, vargs);
	va_end(vargs);

}

void f4::vprintAlert(const Char* caption, const Char* msg, va_list vargs)
{
	Char buffer[1024];
	vsprintf(buffer, (Char*)msg, vargs);
	MessageBox(0, buffer, caption, MB_ICONEXCLAMATION);
}
