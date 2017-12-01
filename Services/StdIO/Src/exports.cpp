/**
 * @file exports.cpp
 *
 * @author  Jason Perkins (jason@379.com)
 * @date    Copyright (C) 2001 by 379, Inc.
 * @version $Id: exports.cpp,v 1.2 2002/04/22 01:51:56 jason379 Exp $
 */
#include <Core/core.h>
using namespace f4;

DECLARE_SERVICE(Register)
DECLARE_SERVICE(rw4FNT)
DECLARE_SERVICE(rwBMP)
DECLARE_SERVICE(rwOBJ)
DECLARE_SERVICE(rwPNG)
DECLARE_SERVICE(rwTGA)
DECLARE_SERVICE(ZlibFilter)

PACKING_LIST
	SERVICE(Register)
	SERVICE(rw4FNT)
	SERVICE(rwBMP)
	SERVICE(rwOBJ)
	SERVICE(rwPNG)
	SERVICE(rwTGA)
	SERVICE(ZlibFilter)
END_PACKING_LIST

