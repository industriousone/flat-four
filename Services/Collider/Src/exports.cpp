/**
 * @file exports.cpp
 *
 * @author  Jason Perkins (jason@379.com)
 * @date    Copyright (C) 2001 by 379, Inc.
 * @version $Id: exports.cpp,v 1.4 2002/05/30 01:52:00 jason379 Exp $
 */
#include <Core/core.h>
using namespace f4;

DECLARE_SERVICE(BoundingBox)
DECLARE_SERVICE(BoundingCapsule)
DECLARE_SERVICE(BoundingMesh)
DECLARE_SERVICE(BoundingPlane)
DECLARE_SERVICE(CollisionService)
DECLARE_SERVICE(BoundingSphere)
DECLARE_SERVICE(ContactList)

PACKING_LIST
	SERVICE(BoundingBox)
	SERVICE(BoundingCapsule)
	SERVICE(BoundingMesh)
	SERVICE(BoundingPlane)
	SERVICE(CollisionService)
	SERVICE(BoundingSphere)
	SERVICE(ContactList)
END_PACKING_LIST
