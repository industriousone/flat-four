/**
 * @file exports.cpp
 *
 * @author  Jason Perkins (jason@379.com)
 * @date    Copyright (C) 2001 by 379, Inc.
 * @version $Id: exports.cpp,v 1.7 2002/07/16 01:11:37 jason379 Exp $
 */
#include <Core/core.h>
using namespace f4;

DECLARE_SERVICE(Body)
DECLARE_SERVICE(CollisionVisitor)
DECLARE_SERVICE(Entity)
DECLARE_SERVICE(EventVisitor)
DECLARE_SERVICE(PerspectiveCamera)
DECLARE_SERVICE(Physics)
DECLARE_SERVICE(Pose)
DECLARE_SERVICE(RenderVisitor)
DECLARE_SERVICE(SimpleSpace)
DECLARE_SERVICE(Surface)
DECLARE_SERVICE(UpdateVisitor)

PACKING_LIST
	SERVICE(Body)
	SERVICE(CollisionVisitor)
	SERVICE(Entity)
	SERVICE(EventVisitor)
	SERVICE(PerspectiveCamera)
	SERVICE(Physics)
	SERVICE(Pose)
	SERVICE(RenderVisitor)
	SERVICE(SimpleSpace)
	SERVICE(Surface)
	SERVICE(UpdateVisitor)
END_PACKING_LIST
