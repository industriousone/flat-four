/**
 * @file capsule.h
 *
 * @author  Jason Perkins (jason@379.com)
 * @date    Copyright (C) 2001 by 379, Inc.
 * @version $Id: capsule.h,v 1.3 2002/05/15 10:40:04 jason379 Exp $
 */
#if !defined(_F4_COLLIDER_CAPSULE_H_)
#define _F4_COLLIDER_CAPSULE_H_

#include "shape.h"
ENTER_NAMESPACE(f4);


/**
 * A bounding capsule.
 * @ingroup f4_collide
 */
struct BoundingCapsule : public BoundingShape
{

	/**
	 * Specify a radius and length of the cylinder.
	 */
	virtual bool initialize(Float radius, Float length) = 0;

};


// --- begin scripting interface ---

SCRIPT_METHOD(BoundingCapsule, initialize, RETURNS_BOOL)
	ARG_FLOAT, ARG_FLOAT
END_METHOD

EXPORTS(BoundingCapsule)
	INTERFACE(BoundingShape)
	METHOD(BoundingCapsule, initialize)
END_EXPORTS

// --- end scripting interface ---


EXIT_NAMESPACE;
#endif
