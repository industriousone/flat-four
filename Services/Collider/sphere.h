/**
 * @file sphere.h
 *
 * @author  Jason Perkins (jason@379.com)
 * @date    Copyright (C) 2001 by 379, Inc.
 * @version $Id: sphere.h,v 1.3 2002/05/15 10:40:04 jason379 Exp $
 */
#if !defined(_F4_COLLIDER_SPHERE_H_)
#define _F4_COLLIDER_SPHERE_H_

#include "shape.h"
ENTER_NAMESPACE(f4);


/**
 * A bounding sphere.
 * @ingroup f4_collide
 */
struct BoundingSphere : public BoundingShape
{

	/**
	 * Specify a radius for the sphere.
	 */
	virtual bool initialize(Float radius) = 0;


	/**
	 * Specify a radius for the sphere.
	 */
	virtual void setRadius(Float radius) = 0;


	/**
	 * Get the sphere radius.
	 */
	virtual Float getRadius() = 0;

};


// --- begin scripting interface ---

SCRIPT_METHOD(BoundingSphere, getRadius, RETURNS_FLOAT)
END_METHOD

SCRIPT_METHOD(BoundingSphere, initialize, RETURNS_BOOL)
	ARG_FLOAT
END_METHOD

SCRIPT_METHOD(BoundingSphere, setRadius, RETURNS_VOID)
	ARG_FLOAT
END_METHOD

EXPORTS(BoundingSphere)
	INTERFACE(BoundingShape)
	RW_PROP_FLOAT(BoundingSphere, radius, getRadius, setRadius)
	METHOD(BoundingSphere, initialize)
END_EXPORTS

// --- end scripting interface ---


EXIT_NAMESPACE;
#endif
