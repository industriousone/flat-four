/**
 * @file perspective_camera.h
 *
 * @author  Jason Perkins (jason@379.com)
 * @date    Copyright (C) 2002 by 379, Inc.
 * @version $Id: perspective_camera.h,v 1.3 2002/07/16 01:11:36 jason379 Exp $
 */
#if !defined(_F4_SCENE_PERSPECTIVE_CAMERA_H_)
#define _F4_SCENE_PERSPECTIVE_CAMERA_H_

#include "camera.h"
ENTER_NAMESPACE(f4);


/**
 * A perspective 3D camera.
 * @ingroup f4_scene
 */
struct PerspectiveCamera : public Camera
{

	/**
	 * Set the position of the camera in the parent entity coordinate space.
	 */
	virtual void setPosition(Float x, Float y, Float z) = 0;


	/**
	 * Aims the camera at a particular point in space.
	 */
	virtual void lookAt(Float atX, Float atY, Float atZ, Float upX, Float upY, Float upZ) = 0;


	/**
	 * Set the camera's rotation from a quaternion representation.
	 */
	virtual void setRotation(const Quaternion& quat) = 0;


	/**
	 * Set the camera's rotation from a euler representation.
	 */
	virtual void setEulerRotation(Float h, Float p, Float r) = 0;


	/**
	 * Set the horizontal field of view, in degrees.
	 * @par Scripted as:
	 *      [r/o float] @a fov
	 */
	virtual void setFieldOfView(Float fov) = 0;


	/**
	 * Specify the display aspect ratio (width/height).
	 * @par Scripted as:
	 *      [r/o float] @a aspect
	 */
	virtual void setAspectRatio(Float aspect) = 0;


	/**
	 * Specify the near clipping plane distance.
	 * @par Scripted as:
	 *      [r/o float] @a near
	 */
	virtual void setNearClipDistance(Float z) = 0;


	/**
	 * Specify the far clipping plane distance.
	 * @par Scripted as:
	 *      [r/o float] @a far
	 */
	virtual void setFarClipDistance(Float z) = 0;

};



// --- begin scripting interface ---

SCRIPT_METHOD(PerspectiveCamera, lookAt, RETURNS_VOID)
	ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT
END_METHOD

SCRIPT_METHOD(PerspectiveCamera, setAspectRatio, RETURNS_VOID)
	ARG_FLOAT
END_METHOD

SCRIPT_METHOD(PerspectiveCamera, setEulerRotation, RETURNS_VOID)
	ARG_FLOAT, ARG_FLOAT, ARG_FLOAT
END_METHOD

SCRIPT_METHOD(PerspectiveCamera, setFarClipDistance, RETURNS_VOID)
	ARG_FLOAT
END_METHOD

SCRIPT_METHOD(PerspectiveCamera, setFieldOfView, RETURNS_VOID)
	ARG_FLOAT
END_METHOD

SCRIPT_METHOD(PerspectiveCamera, setNearClipDistance, RETURNS_VOID)
	ARG_FLOAT
END_METHOD

SCRIPT_METHOD(PerspectiveCamera, setPosition, RETURNS_VOID)
	ARG_FLOAT, ARG_FLOAT, ARG_FLOAT
END_METHOD

SCRIPT_METHOD(PerspectiveCamera, setRotation, RETURNS_VOID)
	ARG_QUAT
END_METHOD

EXPORTS(PerspectiveCamera)
	INTERFACE(Camera)
	RO_PROP_FLOAT(PerspectiveCamera, aspect, setAspectRatio)
	RO_PROP_FLOAT(PerspectiveCamera, far,    setFarClipDistance)
	RO_PROP_FLOAT(PerspectiveCamera, fov,    setFieldOfView)
	RO_PROP_FLOAT(PerspectiveCamera, near,   setNearClipDistance)
	METHOD(PerspectiveCamera, lookAt)
	METHOD(PerspectiveCamera, setEulerRotation)
	METHOD(PerspectiveCamera, setPosition)
	METHOD(PerspectiveCamera, setRotation)
END_EXPORTS

// --- end scripting interface ---


EXIT_NAMESPACE;
#endif
