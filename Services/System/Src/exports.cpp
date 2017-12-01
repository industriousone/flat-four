/**
 * @file exports.cpp
 *
 * @author  Jason Perkins (jason@379.com)
 * @date    Copyright (C) 2001 by 379, Inc.
 * @version $Id: exports.cpp,v 1.2 2002/04/22 01:51:56 jason379 Exp $
 */
#include <Core/core.h>
using namespace f4;

DECLARE_SERVICE(AxisEvent)
DECLARE_SERVICE(ButtonEvent)
DECLARE_SERVICE(ClickEvent)
DECLARE_SERVICE(DisplayEvent)
DECLARE_SERVICE(KeyEvent)
DECLARE_SERVICE(MotionEvent)
DECLARE_SERVICE(SystemService)
DECLARE_SERVICE(TimeEvent)

PACKING_LIST
	SERVICE(AxisEvent)
	SERVICE(ButtonEvent)
	SERVICE(ClickEvent)
	SERVICE(DisplayEvent)
	SERVICE(KeyEvent)
	SERVICE(MotionEvent)
	SERVICE(SystemService)
	SERVICE(TimeEvent)
END_PACKING_LIST
