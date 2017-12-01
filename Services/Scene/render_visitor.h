/**
 * @file render_visitor.h
 *
 * @author  Jason Perkins (jason@379.com)
 * @date    Copyright (C) 2002 by 379, Inc.
 * @license L-GPL
 * @version $Id: render_visitor.h,v 1.1 2002/06/19 01:39:59 jason379 Exp $
 */
#if !defined(_F4_SCENE_RENDER_VISITOR_H_)
#define _F4_SCENE_RENDER_VISITOR_H_

#include "visitor.h"
#include <Render/render.h>
ENTER_NAMESPACE(f4);

struct Camera;
struct Pose;


/**
 * Traverse the entity tree, rendering visuals.
 *	
 * @ingroup f4_scene
 */
struct RenderVisitor : public Visitor
{

	/**
	 * Start a new rendering pass over an entity tree.
	 */
	virtual void render(Entity* root) = 0;


	/**
	 * Makes the specified pose "active". The world matrix of the Pose is
	 * sent to the renderer and will be applied to all subsequently queued
	 * visuals. The visitor will store a reference to the world matrix and
	 * make sure it gets applied to the appropriate parts of the entity tree.
	 */
	virtual void setPose(Pose* pose) = 0;


	/**
	 * Specify a viewpoint to use for subsequent rendering.
	 */
	virtual void setCamera(Camera* camera) = 0;


	/**
	 * Retrieve the RenderService interface.
	 */
	virtual RenderService* getRenderService() = 0;


	/**
	 * Queue a visual for rendering under the current context.
	 */
	virtual void queue(Visual* visual) = 0;

};



// --- begin scripting interface ---

SCRIPT_METHOD(RenderVisitor, getRenderService, RETURNS_SERVICE)
END_METHOD

SCRIPT_METHOD(RenderVisitor, render, RETURNS_VOID)
	ARG_SERVICE(Entity)
END_METHOD

SCRIPT_METHOD(RenderVisitor, queue, RETURNS_VOID)
	ARG_SERVICE(Visual)
END_METHOD

SCRIPT_METHOD(RenderVisitor, setCamera, RETURNS_VOID)
	ARG_SERVICE(Camera)
END_METHOD

SCRIPT_METHOD(RenderVisitor, setPose, RETURNS_VOID)
	ARG_SERVICE(Pose)
END_METHOD

EXPORTS(RenderVisitor)
	INTERFACE(Visitor)
	METHOD(RenderVisitor, getRenderService)
	METHOD(RenderVisitor, render)
	METHOD(RenderVisitor, queue)
	METHOD(RenderVisitor, setCamera)
	METHOD(RenderVisitor, setPose)
END_EXPORTS

// --- end scripting interface ---


EXIT_NAMESPACE;
#endif
