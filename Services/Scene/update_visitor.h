/**
 * @file update_visitor.h
 *
 * @author  Jason Perkins (jason@379.com)
 * @date    Copyright (C) 2002 by 379, Inc.
 * @version $Id: update_visitor.h,v 1.2 2002/07/20 02:00:33 jason379 Exp $
 */
#if !defined(_F4_SCENE_UPDATE_VISITOR_H_)
#define _F4_SCENE_UPDATE_VISITOR_H_

#include "visitor.h"
ENTER_NAMESPACE(f4);

struct Pose;


/**
 * Traverse the entity tree updating state.
 *	
 * @ingroup f4_scene
 */
struct UpdateVisitor : public Visitor
{

	/**
	 * Start an update pass over an entity tree.
	 *
	 * @param elapsedTime   the elapsed time since the last update, measured
	 *                      in seconds.
	 */
	virtual void update(Entity* root, Float elapsedTime) = 0;


	/**
	 * Retrieve the time elapsed since the last update, in seconds.
	 */
	virtual Float getElapsedTime() const = 0;


	/**
	 * In order to maintain stability and predictability, the physics system
	 * is run at a fixed rate, usually higher than the frame rate. This method
	 * sets the physics rate. The default is 0.01 (100Hz).
	 *
	 * @param rate   The time between physics updates, measured in seconds.
	 *               If this value is zero, the physics system will not be 
	 *               updated.
	 */
	virtual void setPhysicsRate(Float rate) = 0;


	/**
	 * Makes the specified pose "active". The world matrix of the Pose is 
	 * cached and becomes accessible via getWorldMatrix(). The visitor will 
	 * store a reference to the world matrix and make sure it gets applied 
	 * to the appropriate parts of the entity tree.
	 */
	virtual void setPose(Pose* pose) = 0;


	/**
	 * Get the current local->world space matrix.
	 */
	virtual const Matrix44& getWorldMatrix() const = 0;

};


// --- begin scripting interface ---

SCRIPT_METHOD(UpdateVisitor, getElapsedTime, RETURNS_FLOAT)
END_METHOD

SCRIPT_METHOD(UpdateVisitor, getWorldMatrix, RETURNS_MATRIX)
END_METHOD

SCRIPT_METHOD(UpdateVisitor, setPhysicsRate, RETURNS_VOID)
	ARG_FLOAT
END_METHOD

SCRIPT_METHOD(UpdateVisitor, setPose, RETURNS_VOID)
	ARG_SERVICE(Pose)
END_METHOD

SCRIPT_METHOD(UpdateVisitor, update, RETURNS_VOID)
	ARG_SERVICE(Entity), ARG_FLOAT
END_METHOD

EXPORTS(UpdateVisitor)
	INTERFACE(Visitor)
	RO_PROP_FLOAT(UpdateVisitor, elapsed, getElapsedTime)
	METHOD(UpdateVisitor, getWorldMatrix)
	METHOD(UpdateVisitor, setPhysicsRate)
	METHOD(UpdateVisitor, setPose)
	METHOD(UpdateVisitor, update)
END_EXPORTS

// --- end scripting interface ---


EXIT_NAMESPACE;
#endif
