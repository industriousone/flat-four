/**
 * @file exports.cpp
 *
 * @author  Jason Perkins (jason@379.com)
 * @date    Copyright (C) 2001 by 379, Inc.
 * @version $Id: exports.cpp,v 1.2 2002/04/22 01:51:56 jason379 Exp $
 */
#include <Core/core.h>
using namespace f4;

DECLARE_SERVICE(IOService)

PACKING_LIST
	SERVICE(IOService)
END_PACKING_LIST

