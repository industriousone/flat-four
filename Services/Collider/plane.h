/**
 * @file plane.h
 *
 * @author  Jason Perkins (jason@379.com)
 * @date    Copyright (C) 2001 by 379, Inc.
 * @version $Id: plane.h,v 1.3 2002/05/15 10:40:04 jason379 Exp $
 */
#if !defined(_F4_COLLIDER_PLANE_H_)
#define _F4_COLLIDER_PLANE_H_

#include "shape.h"
ENTER_NAMESPACE(f4);


/**
 * A bounding plane.
 * @ingroup f4_collide
 */
struct BoundingPlane : public BoundingShape
{

	/**
	 * Specify the plane equation.
	 */
	virtual bool initialize(Float a, Float b, Float c, Float d) = 0;

};


// --- begin scripting interface ---

SCRIPT_METHOD(BoundingPlane, initialize, RETURNS_BOOL)
	ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT
END_METHOD

EXPORTS(BoundingPlane)
	INTERFACE(BoundingShape)
	METHOD(BoundingPlane, initialize)
END_EXPORTS

// --- end scripting interface ---


EXIT_NAMESPACE;
#endif
