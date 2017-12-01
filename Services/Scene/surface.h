/**
 * @file surface.h
 *
 * @author  Jason Perkins (jason@379.com)
 * @date    Copyright (C) 2002 by 379, Inc.
 * @version $Id: surface.h,v 1.2 2002/06/02 18:54:48 jason379 Exp $
 */
#if !defined(_F4_SCENE_SURFACE_H_)
#define _F4_SCENE_SURFACE_H_

#include "attribute.h"
#include <Collider/shape.h>
#include <Physics/contact_physics.h>
ENTER_NAMESPACE(f4);


/**
 * The entity's surface defines a hard edge for collision detection, along
 * with parameters for collision response. The surface is described as a
 * collection of bounding shapes.
 *
 * @ingroup f4_surface
 */
struct Surface : public Attribute
{

	/**
	 * Add a bounding shape to the surface.
	 */
	virtual void addBoundingShape(BoundingShape* bs) = 0;


	/**
	 * Returns the number of bounding shapes used by the surface.
	 */
	virtual int countBoundingShapes() const = 0;


	/**
	 * Retrieve a particular bounding shape used by the surface.
	 */
	virtual BoundingShape* getBoundingShape(int i) = 0;


	/**
	 * Specify the response physics of the surface.
	 */
	virtual void setContactPhysics(ContactPhysics* cp) = 0;


	/**
	 * Retrieve the response physics of the surface.
	 */
	virtual ContactPhysics* getContactPhysics() = 0;

};


// --- begin scripting interface ---

SCRIPT_METHOD(Surface, addBoundingShape, RETURNS_VOID)
	ARG_SERVICE(BoundingShape)
END_METHOD

SCRIPT_METHOD(Surface, countBoundingShapes, RETURNS_INT)
END_METHOD

SCRIPT_METHOD(Surface, getBoundingShape, RETURNS_SERVICE)
	ARG_INT
END_METHOD

SCRIPT_METHOD(Surface, getContactPhysics, RETURNS_SERVICE)
END_METHOD

SCRIPT_METHOD(Surface, setContactPhysics, RETURNS_VOID)
	ARG_SERVICE(ContactPhysics)
END_METHOD

EXPORTS(Surface)
	INTERFACE(Attribute)
	METHOD(Surface, addBoundingShape)
	METHOD(Surface, countBoundingShapes)
	METHOD(Surface, getBoundingShape)
	METHOD(Surface, getContactPhysics)
	METHOD(Surface, setContactPhysics)
END_EXPORTS

// --- end scripting interface ---


EXIT_NAMESPACE;
#endif
