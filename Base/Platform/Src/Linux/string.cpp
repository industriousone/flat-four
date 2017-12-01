/**
 * @file string.cpp
 *
 * @author  Jason Perkins (jason@379.com)
 * @date    Copyright (C) 2001 by 379, Inc.
 * @version $Id: string.cpp,v 1.3 2002/05/24 02:15:19 jason379 Exp $
 */
#include <ctype.h>
#include <string.h>

#include <Platform/platform.h>
#include <Platform/string.h>
using namespace f4;


//---------------------------------------------------------------------------

void f4::stringUpper(Char* s)
{
	for (char* c = s; *c; ++c)
		*c = toupper(*c);
}

//---------------------------------------------------------------------------

void f4::stringLower(Char* s)
{
	for (char* c = s; *c; ++c)
		*c = tolower(*c);
}
