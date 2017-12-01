/**
 * @file physics_object.h
 *
 * @author  Jason Perkins (jason@379.com)
 * @date    Copyright (C) 2001 by 379, Inc.
 * @version $Id: physics_object.h,v 1.1 2002/05/15 10:39:00 jason379 Exp $
 */
#if !defined(_F4_PHYSICS_OBJECT_H_)
#define _F4_PHYSICS_OBJECT_H_

ENTER_NAMESPACE(f4);


/**
 * A generic physically-simulated object.
 * @ingroup f4_physics
 */
struct PhysicsObject : public Service
{

	// --- internal interface ---
	virtual void* getBodyData() = 0;
	// --- internal interface ---

};



// --- begin scripting interface ---

EXPORTS(PhysicsObject)
END_EXPORTS

// --- end scripting interface ---


EXIT_NAMESPACE;
#endif
