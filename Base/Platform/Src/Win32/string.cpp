/**
 * @file string.cpp
 *
 * @author  Jason Perkins (jason@379.com)
 * @date    Copyright (C) 2001 by 379, Inc.
 * @version $Id: string.cpp,v 1.3 2002/05/22 01:41:07 jason379 Exp $
 */
#include <string.h>

#include <Platform/platform.h>
#include <Platform/string.h>
using namespace f4;


//---------------------------------------------------------------------------

void f4::stringUpper(Char* s)
{
	_strupr(s);
}

//---------------------------------------------------------------------------

void f4::stringLower(Char* s)
{
	_strlwr(s);
}
