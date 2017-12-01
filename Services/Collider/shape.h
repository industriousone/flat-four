/**
 * @file shape.h
 *
 * @author  Jason Perkins (jason@379.com)
 * @date    Copyright (C) 2001 by 379, Inc.
 * @version $Id: shape.h,v 1.3 2002/05/15 10:40:04 jason379 Exp $
 */
#if !defined(_F4_COLLIDER_SHAPE_H_)
#define _F4_COLLIDER_SHAPE_H_

ENTER_NAMESPACE(f4);


/**
 * Abstract interface to a collision shape.
 * @ingroup f4_collide
 */
struct BoundingShape : public Service
{

	/**
	 * Apply a matrix transform to the shape. Currently, only translation
	 * and rotation are supported.
	 */
	virtual void setMatrix(const Matrix44& m) = 0;


	// --- internal interface ---
	virtual void* getShapeData() = 0;
	// --- internal interface ---

};


// --- begin scripting interface ---

SCRIPT_METHOD(BoundingShape, setMatrix, RETURNS_VOID)
	ARG_MATRIX
END_METHOD

EXPORTS(BoundingShape)
	METHOD(BoundingShape, setMatrix)
END_EXPORTS

// --- end scripting interface ---


EXIT_NAMESPACE;
#endif
