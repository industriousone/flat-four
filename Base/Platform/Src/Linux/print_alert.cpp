/**
 * @file print_alert.cpp
 *
 * Linux version of the printAlert functions.
 *
 * @author  Jason Perkins (jason@379.com)
 * @date    Copyright (C) 2001 by 379, Inc.
 * @version $Id: print_alert.cpp,v 1.3 2002/05/24 02:15:19 jason379 Exp $
 */
#include <stdio.h>
#include <Platform/platform.h>
#include <Platform/print_alert.h>
using namespace f4;


void f4::printAlert(const char* caption, const char* msg, ...)
{
	va_list vargs;
	va_start(vargs, msg);
	fprintf(stderr, msg, vargs);
	va_end(vargs);
	fputs("\n", stderr);
}

void f4::vprintAlert(const char* caption, const char* msg, va_list vargs)
{
	vfprintf(stderr, msg, vargs);
	fputs("\n", stderr);
}
