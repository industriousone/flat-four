/**
 * @file collision_visitor.h
 *
 * @author  Jason Perkins (jason@379.com)
 * @date    Copyright (C) 2002 by 379, Inc.
 * @license L-GPL
 * @version $Id: collision_visitor.h,v 1.1 2002/06/19 01:39:59 jason379 Exp $
 */
#if !defined(_F4_SCENE_COLLISION_VISITOR_H_)
#define _F4_SCENE_COLLISION_VISITOR_H_

#include "visitor.h"
#include "surface.h"
#include <Physics/physics_object.h>
ENTER_NAMESPACE(f4);

struct Entity;


/**
 * Traverse the entity tree looking for collisions between surfaces.
 *
 * @ingroup f4_scene
 */
struct CollisionVisitor : public Visitor
{

	/**
	 * Start a new collision pass over an entity tree.
	 */
	virtual void collide(Entity* root) = 0;


	/**
	 * Flag the current entity has having moved. Only moved surfaces are
	 * tested for collisions.
	 */
	virtual void hasMoved() = 0;


	/**
	 * Set the physical properties of the entity being tested.
	 */
	virtual void setPhysics(PhysicsObject* physics) = 0;


	/**
	 * Provide a surface for testing.
	 */
	virtual void test(Surface* surf) = 0;

};


// --- begin scripting interface ---

SCRIPT_METHOD(CollisionVisitor, collide, RETURNS_VOID)
	ARG_SERVICE(Entity)
END_METHOD

SCRIPT_METHOD(CollisionVisitor, hasMoved, RETURNS_VOID)
END_METHOD

SCRIPT_METHOD(CollisionVisitor, setPhysics, RETURNS_VOID)
	ARG_SERVICE(PhysicsObject)
END_METHOD

SCRIPT_METHOD(CollisionVisitor, test, RETURNS_VOID)
	ARG_SERVICE(Surface)
END_METHOD

EXPORTS(CollisionVisitor)
	INTERFACE(Visitor)
	METHOD(CollisionVisitor, collide)
	METHOD(CollisionVisitor, hasMoved)
	METHOD(CollisionVisitor, setPhysics)
	METHOD(CollisionVisitor, test)
END_EXPORTS

// --- end scripting interface

EXIT_NAMESPACE;
#endif
