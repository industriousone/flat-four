/**
 * @file contact_physics.h
 *
 * @author  Jason Perkins (jason@379.com)
 * @date    Copyright (C) 2001 by 379, Inc.
 * @version $Id: contact_physics.h,v 1.3 2002/05/24 19:05:21 jason379 Exp $
 */
#if !defined(_F4_PHYSICS_CONTACT_PHYSICS_H_)
#define _F4_PHYSICS_CONTACT_PHYSICS_H_

#include <Collider/contact_list.h>
ENTER_NAMESPACE(f4);


/**
 * Describe a collision and it's response parameters.
 * @ingroup f4_physics
 */
struct ContactPhysics : public Service
{
	enum { INFINITY = -1 };

	/**
	 * Specify a coefficient of restitution.
	 *
	 * @param r   A value between 0.0 and 1.0, where 0.0 is no bounce, and
	 *            1.0 is a "perfect" bounce.
	 *
	 * @par Scripted as:
	 *      [ r/w float ] @a restitution
	 */
	virtual void setRestitution(Float r) = 0;


	/**
	 * Retrieve the coefficient of restitution.
	 * @par Scripted as:
	 *      [ r/w float ] @a restitution
	 */
	virtual Float getRestitution() const = 0;


	/**
	 * Specify a friction constant.
	 *
	 * @param mu    A value >0.0, or the constant @c INFINITY.
	 *
	 * @par Scripted as:
	 *      [ r/w float ] @a friction
	 */
	virtual void setFriction(Float mu) = 0;


	/**
	 * Retrieve the friction constant.
	 * @par Scripted as:
	 *      [ r/w float ] @a friction
	 */
	virtual Float getFriction() const = 0;

};


// --- begin scripting interface ---

SCRIPT_METHOD(ContactPhysics, getFriction, RETURNS_FLOAT)
END_METHOD

SCRIPT_METHOD(ContactPhysics, getRestitution, RETURNS_FLOAT)
END_METHOD

SCRIPT_METHOD(ContactPhysics, setFriction, RETURNS_VOID)
	ARG_FLOAT
END_METHOD

SCRIPT_METHOD(ContactPhysics, setRestitution, RETURNS_VOID)
	ARG_FLOAT
END_METHOD

EXPORTS(ContactPhysics)
	RW_PROP_FLOAT(ContactPhysics, friction, getFriction, setFriction)
	RW_PROP_FLOAT(ContactPhysics, restitution, getRestitution, setRestitution)
	CONST_INT(ContactPhysics, INFINITY)
END_EXPORTS

// --- end scripting interface ---


EXIT_NAMESPACE;
#endif
