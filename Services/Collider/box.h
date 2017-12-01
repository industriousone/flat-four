/**
 * @file box.h
 *
 * @author  Jason Perkins (jason@379.com)
 * @date    Copyright (C) 2001 by 379, Inc.
 * @version $Id: box.h,v 1.3 2002/05/15 10:40:04 jason379 Exp $
 */
#if !defined(_F4_COLLIDER_BOX_H_)
#define _F4_COLLIDER_BOX_H_

#include "shape.h"
ENTER_NAMESPACE(f4);


/**
 * A bounding box.
 * @ingroup f4_collide
 */
struct BoundingBox : public BoundingShape
{

	/**
	 * Specify the box dimensions.
	 */
	virtual bool initialize(Float x, Float y, Float z) = 0;

};


// --- begin scripting interface ---

SCRIPT_METHOD(BoundingBox, initialize, RETURNS_BOOL)
	ARG_FLOAT, ARG_FLOAT, ARG_FLOAT
END_METHOD

EXPORTS(BoundingBox)
	INTERFACE(BoundingShape)
	METHOD(BoundingBox, initialize)
END_EXPORTS

// --- end scripting interface ---


EXIT_NAMESPACE;
#endif
