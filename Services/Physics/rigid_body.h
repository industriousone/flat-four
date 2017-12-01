/**
 * @file rigid_body.h
 *
 * @author  Jason Perkins (jason@379.com)
 * @date    Copyright (C) 2001 by 379, Inc.
 * @version $Id: rigid_body.h,v 1.3 2002/05/15 10:39:00 jason379 Exp $
 */
#if !defined(_F4_PHYSICS_RIGID_BODY_H_)
#define _F4_PHYSICS_RIGID_BODY_H_

#include <Math/matrix.h>
#include "physics_object.h"
ENTER_NAMESPACE(f4);

struct MassHelper;


/**
 * A rigid body simulation.
 * @ingroup f4_physics
 */
struct RigidBody : public PhysicsObject
{

	/**
	 * Specify the mass and inertia of the body from a helper.
	 */
	virtual void setMassFromHelper(MassHelper* helper) = 0;


	/**
	 * Specify the mass of the body.
	 */
	virtual void setMass(Float mass) = 0;


	/**
	 * Calculate the inertia for a solid box of the given dimensions.
	 */
	virtual void setInertiaAsSolidBox(Float x, Float y, Float z) = 0;


	/**
	 * Calculate the inertia for a solid cylinder with the given radius and
	 * thickness. By default, the cylinder is aligned with the X axis.
	 */
	virtual void setInertiaAsSolidCylinder(Float radius, Float width) = 0;


	/**
	 * Calculate the inertia for a solid sphere with the given radius.
	 */
	virtual void setInertiaAsSolidSphere(Float radius) = 0;



	/**
	 * Get the mass of the body.
	 */
	virtual Float getMass() const = 0;


	/**
	 * Get the tensor of inertia for the body.
	 */
	virtual Vector3 getInertia() const = 0;


	/**
	 * Set the position of the body.
	 */
	virtual void setPosition(Float x, Float y, Float z) = 0;


	/**
	 * Set the orientation of the body from a quaternion representation.
	 */
	virtual void setRotation(const Quaternion& q) = 0;


	/**
	 * Set the orientation of the body from an angle-axis representation.
	 */
	virtual void setAngleAxisRotation(Float angle, Float ax, Float ay, Float az) = 0;
	
	
	/**
	 * Set the orientation of the body from a euler representation.
	 */
	virtual void setEulerRotation(Float h, Float p, Float r) = 0;


	/**
	 * Get the position of the body.
	 */
	virtual Vector3 getPosition() const =  0;


	/**
	 * Get the rotation of the body.
	 */
	virtual Quaternion getRotation() const = 0;


	/**
	 * Set the angular velocity of the body.
	 */
	virtual void setAngularVelocity(Float x, Float y, Float z) = 0;


	/**
	 * Set the linear velocity of the body.
	 */
	virtual void setLinearVelocity(Float x, Float y, Float z) = 0;


	/**
	 * Zero out all velocities of the body.
	 */
	virtual void zeroVelocity() = 0;


	/**
	 * Get the angular velocity of the body.
	 */
	virtual Vector3 getAngularVelocity() const = 0;


	/**
	 * Get the linear velocity of the body.
	 */
	virtual Vector3 getLinearVelocity() const = 0;


	/**
	 * Apply a force in world coordinates on the body center of mass.
	 */
	virtual void applyForce(Float fx, Float fy, Float fz) = 0;


	/**
	 * Apply a force in a specific point on the body, with both specified in
	 * the body's local coordinate space.
	 */
	virtual void applyLocalForceAtLocalPos(Float fx, Float fy, Float fz, Float x, Float y, Float z) = 0;


	/**
	 * Apply a torque on the body.
	 */
	virtual void applyTorque(Float tx, Float ty, Float tz) = 0;

	
	/**
	 * Retrieve the current transform matrix for the body.
	 */
	virtual Matrix44 getMatrix() const = 0;


	// --- internal interface ---
	virtual void* getBodyData() = 0;
	// --- internal interface ---

};


// --- begin scripting interface ---

SCRIPT_METHOD(RigidBody, applyForce, RETURNS_VOID)
	ARG_FLOAT, ARG_FLOAT, ARG_FLOAT
END_METHOD

SCRIPT_METHOD(RigidBody, applyLocalForceAtLocalPos, RETURNS_VOID)
	ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT
END_METHOD

SCRIPT_METHOD(RigidBody, applyTorque, RETURNS_VOID)
	ARG_FLOAT, ARG_FLOAT, ARG_FLOAT
END_METHOD

SCRIPT_METHOD(RigidBody, getAngularVelocity, RETURNS_VECTOR)
END_METHOD

SCRIPT_METHOD(RigidBody, getInertia, RETURNS_VECTOR)
END_METHOD

SCRIPT_METHOD(RigidBody, getLinearVelocity, RETURNS_VECTOR)
END_METHOD

SCRIPT_METHOD(RigidBody, getMass, RETURNS_FLOAT)
END_METHOD

SCRIPT_METHOD(RigidBody, getMatrix, RETURNS_MATRIX)
END_METHOD

SCRIPT_METHOD(RigidBody, getPosition, RETURNS_VECTOR)
END_METHOD

SCRIPT_METHOD(RigidBody, getRotation, RETURNS_QUAT)
END_METHOD

SCRIPT_METHOD(RigidBody, setAngleAxisRotation, RETURNS_VOID)
	ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT
END_METHOD

SCRIPT_METHOD(RigidBody, setAngularVelocity, RETURNS_VOID)
	ARG_FLOAT, ARG_FLOAT, ARG_FLOAT
END_METHOD

SCRIPT_METHOD(RigidBody, setEulerRotation, RETURNS_VOID)
	ARG_FLOAT, ARG_FLOAT, ARG_FLOAT
END_METHOD

SCRIPT_METHOD(RigidBody, setInertiaAsSolidBox, RETURNS_VOID)
	ARG_FLOAT, ARG_FLOAT, ARG_FLOAT
END_METHOD

SCRIPT_METHOD(RigidBody, setInertiaAsSolidCylinder, RETURNS_VOID)
	ARG_FLOAT, ARG_FLOAT
END_METHOD

SCRIPT_METHOD(RigidBody, setInertiaAsSolidSphere, RETURNS_VOID)
	ARG_FLOAT
END_METHOD

SCRIPT_METHOD(RigidBody, setLinearVelocity, RETURNS_VOID)
	ARG_FLOAT, ARG_FLOAT, ARG_FLOAT
END_METHOD

SCRIPT_METHOD(RigidBody, setMass, RETURNS_VOID)
	ARG_FLOAT
END_METHOD

SCRIPT_METHOD(RigidBody, setMassFromHelper, RETURNS_VOID)
	ARG_SERVICE(MassHelper)
END_METHOD

SCRIPT_METHOD(RigidBody, setPosition, RETURNS_VOID)
	ARG_FLOAT, ARG_FLOAT, ARG_FLOAT
END_METHOD

SCRIPT_METHOD(RigidBody, setRotation, RETURNS_VOID)
	ARG_QUAT
END_METHOD

SCRIPT_METHOD(RigidBody, zeroVelocity, RETURNS_VOID)
END_METHOD

EXPORTS(RigidBody)
	INTERFACE(PhysicsObject)
	RW_PROP_VECTOR(RigidBody, angularVelocity, getAngularVelocity, setAngularVelocity)
	RW_PROP_VECTOR(RigidBody, linearVelocity, getLinearVelocity, setLinearVelocity)
	RW_PROP_FLOAT(RigidBody, mass, getMass, setMass)
	METHOD(RigidBody, applyForce)
	METHOD(RigidBody, applyLocalForceAtLocalPos)
	METHOD(RigidBody, applyTorque)
	METHOD(RigidBody, getInertia)
	METHOD(RigidBody, getMatrix)
	METHOD(RigidBody, getPosition)
	METHOD(RigidBody, getRotation)
	METHOD(RigidBody, setAngleAxisRotation)
	METHOD(RigidBody, setEulerRotation)
	METHOD(RigidBody, setInertiaAsSolidBox)
	METHOD(RigidBody, setInertiaAsSolidCylinder)
	METHOD(RigidBody, setInertiaAsSolidSphere)
	METHOD(RigidBody, setMassFromHelper)
	METHOD(RigidBody, setPosition)
	METHOD(RigidBody, setRotation)
	METHOD(RigidBody, zeroVelocity)
END_EXPORTS

// --- end scripting interface ---


EXIT_NAMESPACE;
#endif
