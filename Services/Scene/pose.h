/**
 * @file pose.h
 *
 * @author  Jason Perkins (jason@379.com)
 * @date    Copyright (C) 2002 by 379, Inc.
 * @version $Id: pose.h,v 1.2 2002/07/16 01:11:36 jason379 Exp $
 */
#if !defined(_F4_SCENE_POSE_H_)
#define _F4_SCENE_POSE_H_

#include "attribute.h"
#include <Math/math.h>
ENTER_NAMESPACE(f4);


/**
 * Defines a position and orientation for the entity. 
 *
 * Note that a pose only effects the attributes that appear after it in the
 * entity's attribute list.
 *
 * @ingroup f4_scene
 */
struct Pose : public Attribute
{

	/**
	 * Set the position.
	 */
	virtual void setPosition(Float x, Float y, Float z) = 0;


	/**
	 * Get the position.
	 */
	virtual Vector3 getPosition() const = 0;


	/**
	 * Set the rotation from a euler representation.
	 */
	virtual void setEulerRotation(Float h, Float r, Float p) = 0;


	/**
	 * Set the rotation from an angle-axis representation.
	 */
	virtual void setAngleAxisRotation(Float a, Float x, Float y, Float z) = 0;


	/**
	 * Set the rotation from a quaternion.
	 */
	virtual void setRotation(const Quaternion& quat) = 0;


	/**
	 * Get the rotation.
	 */
	virtual Quaternion getRotation() const = 0;


	/**
	 * Retrieve a local-space transformation matrix.
	 */
	virtual Matrix44 getMatrix() const = 0;


	/**
	 * Get the local->world transformation matrix. Note that this method
	 * returns a <I>reference</I> to the matrix; each implementation is
	 * required to cache the world matrix and keep it up to date. This
	 * cached matrix is used by the engine subsystems during the update
	 * and render passes, and may be referenced elsewhere as well.
	 */
	virtual const Matrix44& getWorldMatrix() const = 0;

};



// --- begin scripting interface ---

SCRIPT_METHOD(Pose, getMatrix, RETURNS_MATRIX)
END_METHOD

SCRIPT_METHOD(Pose, getPosition, RETURNS_VECTOR)
END_METHOD

SCRIPT_METHOD(Pose, getRotation, RETURNS_QUAT)
END_METHOD

SCRIPT_METHOD(Pose, getWorldMatrix, RETURNS_MATRIX)
END_METHOD

SCRIPT_METHOD(Pose, setAngleAxisRotation, RETURNS_VOID)
	ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT
END_METHOD

SCRIPT_METHOD(Pose, setEulerRotation, RETURNS_VOID)
	ARG_FLOAT, ARG_FLOAT, ARG_FLOAT
END_METHOD

SCRIPT_METHOD(Pose, setPosition, RETURNS_VOID)
	ARG_FLOAT, ARG_FLOAT, ARG_FLOAT
END_METHOD

SCRIPT_METHOD(Pose, setRotation, RETURNS_VOID)
	ARG_QUAT
END_METHOD

EXPORTS(Pose)
	INTERFACE(Attribute)
	METHOD(Pose, getMatrix)
	METHOD(Pose, getPosition)
	METHOD(Pose, getRotation)
	METHOD(Pose, getWorldMatrix)
	METHOD(Pose, setAngleAxisRotation)
	METHOD(Pose, setEulerRotation)
	METHOD(Pose, setPosition)
	METHOD(Pose, setRotation)
END_EXPORTS

// --- end scripting interface ---


EXIT_NAMESPACE;
#endif

