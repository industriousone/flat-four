/**
 * @file geometry.h
 *
 * @author  Jason Perkins (jason@379.com)
 * @date    Copyright (C) 2001 by 379, Inc.
 * @version $Id: geometry.h,v 1.2 2002/04/22 01:51:56 jason379 Exp $
 */
#if !defined(_F4_RENDER_GEOMETRY_H_)
#define _F4_RENDER_GEOMETRY_H_

ENTER_NAMESPACE(f4);


/**
 * Base class for 3D geometry representations.
 * @ingroup f4_render
 */
struct Geometry : public Service
{

	/**
	 * Retrieve the bounding box extents of the geometry as the min and
	 * max corner coordinates. This function may be slow, depending on the
	 * geometry being queried.
	 */
	virtual void getExtents(Vector3* min, Vector3* max) = 0;


	// --- internal interface ---
	virtual void execute() = 0;
	// --- internal interface

};


// --- begin scripting interface ---

SCRIPT_METHOD(Geometry, getExtents, RETURNS_VOID)
	ARG_PTR(Vector3), ARG_PTR(Vector3)
END_METHOD

EXPORTS(Geometry)
	METHOD(Geometry, getExtents)
END_EXPORTS

// --- end scripting interface ---


EXIT_NAMESPACE;
#endif
