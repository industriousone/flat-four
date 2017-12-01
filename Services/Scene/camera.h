/**
 * @file camera.h
 *
 * @author  Jason Perkins (jason@379.com)
 * @date    Copyright (C) 2002 by 379, Inc.
 * @version $Id: camera.h,v 1.2 2002/06/19 01:39:59 jason379 Exp $
 */
#if !defined(_F4_SCENE_CAMERA_H_)
#define _F4_SCENE_CAMERA_H_

#include "attribute.h"
ENTER_NAMESPACE(f4);


/**
 * A camera is a viewpoint into a scene. This is an abstract interface.
 *
 * @ingroup f4_xem
 */
struct Camera : public Attribute
{

	/**
	 * Pass the camera settings to the renderer.
	 */
	virtual void apply(RenderVisitor* rv) = 0;

};



// --- begin scripting interface ---

SCRIPT_METHOD(Camera, apply, RETURNS_VOID)
	ARG_SERVICE(RenderVisitor)
END_METHOD

EXPORTS(Camera)
	INTERFACE(Attribute)
	METHOD(Camera, apply)
END_EXPORTS

// --- end scripting interface ---


EXIT_NAMESPACE;
#endif
