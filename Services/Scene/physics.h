/**
 * @file physics.h
 *
 * @author  Jason Perkins (jason@379.com)
 * @date    Copyright (C) 2002 by 379, Inc.
 * @version $Id: physics.h,v 1.1 2002/05/18 10:59:15 jason379 Exp $
 */
#if !defined(_F4_SCENE_PHYSICS_H_)
#define _F4_SCENE_PHYSICS_H_

#include <Physics/rigid_body.h>
#include "pose.h"
ENTER_NAMESPACE(f4);


/**
 * A Pose attribute that provides physically-based animation, including
 * collision response.
 *
 * @ingroup f4_scene
 */
struct Physics : public Pose
{

	/** 
	 * Get the associated RigidBody interface.
	 */
	virtual RigidBody* getBody() = 0;

};



// --- begin scripting interface ---

SCRIPT_METHOD(Physics, getBody, RETURNS_SERVICE)
END_METHOD

EXPORTS(Physics)
	INTERFACE(Pose)
	RO_PROP_SERVICE(Physics, body, getBody)
END_EXPORTS

// --- end scripting interface ---


EXIT_NAMESPACE;
#endif

