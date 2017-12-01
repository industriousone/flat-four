/**
 * @file cube.h
 *
 * @author  Jason Perkins (jason@379.com)
 * @date    Copyright (C) 2001 by 379, Inc.
 * @version $Id: cube.h,v 1.2 2002/04/22 01:51:56 jason379 Exp $
 */
#if !defined(_F4_STDLIB_CUBE_H_)
#define _F4_STDLIB_CUBE_H_

#include <Render/geometry.h>
ENTER_NAMESPACE(f4);


/**
 * A 3D cube.
 * @ingroup f4_stdlib
 */
struct Cube : public Geometry
{

	/**
	 * Create the cube geometry.
	 *
	 * @param x,y,z  The dimensions of the cube along each major axis.
	 *
	 * @par Scripted as:
	 *      [constructor] <I>(float, float, float)</I>
	 */
	virtual bool initialize(Float x, Float y, Float z) = 0;

};


// --- begin scripting interface ---

SCRIPT_METHOD(Cube, initialize, RETURNS_BOOL)
	ARG_FLOAT, ARG_FLOAT, ARG_FLOAT
END_METHOD

EXPORTS(Cube)
	INTERFACE(Geometry)
	METHOD(Cube, initialize)
END_EXPORTS

// --- end scripting interface ---


EXIT_NAMESPACE;
#endif
