/**
 * @file physics.h
 *
 * @author  Jason Perkins (jason@379.com)
 * @date    Copyright (C) 2001 by 379, Inc.
 * @version $Id: physics.h,v 1.3 2002/05/24 19:05:21 jason379 Exp $
 */
#if !defined(_F4_PHYSICS_H_)
#define _F4_PHYSICS_H_

#include <Math/math.h>
ENTER_NAMESPACE(f4);

struct ContactList;
struct ContactPhysics;
struct PhysicsObject;


/**
 * Physics system interface.
 * @ingroup f4_physics
 */
struct PhysicsService : public Service
{

	/**
	 * Advance the simulation of all simulated objects by the given timestep.
	 */
	virtual void advance(Float timestep) = 0;


	/**
	 * Specify the global gravity vector.
	 */
	virtual void setGravity(Float x, Float y, Float z) = 0;


	/**
	 * Get the global gravity vector.
	 */
	virtual Vector3 getGravity() const = 0;


	/**
	 * Resolve a collision between two rigid bodies.
	 */
	virtual void resolveCollision(PhysicsObject* p1, PhysicsObject* p2, ContactList* cl, ContactPhysics* cp) = 0;

};



// --- begin scripting interface ---

SCRIPT_METHOD(PhysicsService, advance, RETURNS_VOID)
	ARG_FLOAT
END_METHOD

SCRIPT_METHOD(PhysicsService, getGravity, RETURNS_VECTOR)
END_METHOD

SCRIPT_METHOD(PhysicsService, resolveCollision, RETURNS_VOID)
	ARG_SERVICE(PhysicsObject), ARG_SERVICE(PhysicsObject), ARG_SERVICE(ContactList), ARG_SERVICE(ContactPhysics)
END_METHOD

SCRIPT_METHOD(PhysicsService, setGravity, RETURNS_VOID)
	ARG_FLOAT, ARG_FLOAT, ARG_FLOAT
END_METHOD

EXPORTS(PhysicsService)
	METHOD(PhysicsService, advance)
	METHOD(PhysicsService, getGravity)
	METHOD(PhysicsService, resolveCollision)
	METHOD(PhysicsService, setGravity)
END_EXPORTS

// --- end scripting interface ---


EXIT_NAMESPACE;
#endif
