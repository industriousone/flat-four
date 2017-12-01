/**
 * @file render.h
 *
 * @author  Jason Perkins (jason@379.com)
 * @date    Copyright (C) 2001 by 379, Inc.
 * @version $Id: render.h,v 1.2 2002/04/22 01:51:56 jason379 Exp $
 */
#if !defined(_F4_RENDER_H_)
#define _F4_RENDER_H_

#include <Core/event_listener.h>
#include "texture.h"
ENTER_NAMESPACE(f4);

struct Visual;


/**
 * The "render" services provides 2D/3D drawing capabilities.
 * @ingroup f4_render
 */
struct RenderService : public EventListener
{

	/**
	 * Prepare the renderer for a new frame of animation.
	 */
	virtual void start() = 0;


	/**
	 * Complete rendering of a frame of animation. Flushes all queued
	 * queued visuals.
	 */
	virtual void finish() = 0;


	/**
	 * Specify a new projection matrix. Changing the projection matrix flushes
	 * the visuals queue.
	 */
	virtual void setProjectionMatrix(const Matrix44& m) = 0;


	/**
	 * Sets up a 2D orthographic projection matrix. Changing the projection
	 * matrix flushes the visuals queue.
	 */
	virtual void setProjection2D(Float left, Float top, Float right, Float bottom) = 0;


	/**
	 * Sets up a 3D perspective projection matrix. Changing the projection
	 * matrix flushes the visuals queue.
	 */
	virtual void setProjection3D(Float fov, Float aspect, Float nearClip, Float farClip) = 0;


	/**
	 * Sets the "camera" matrix, which describes the scene's viewpoint.
	 * Generally, you would set the view matrix immediately after calling
	 * start(), and before queueing any visuals. You may change the view
	 * matrix if you wish to display two different views of a scene, this
	 * will cause all queued visuals to be flushed.
	 */
	virtual void setViewMatrix(const Matrix44& m) = 0;


	/**
	 * Sets the world space transform for subsequently queued visuals.
	 *
	 * This matrix is not copied, rather a pointer is kept and attached to all
	 * visuals that are queued. It is the caller's responsibility to make sure
	 * that the matrix remains valid until all queued visuals have been 
	 * flushed.
	 */
	virtual void setWorldMatrix(const Matrix44& m) = 0;


	/**
	 * Gets the active view matrix.
	 */
	virtual const Matrix44& getViewMatrix() const = 0;


	/**
	 * Gets the active world matrix.
	 */
	virtual const Matrix44& getWorldMatrix() const = 0;


	/**
	 * Adds a Visual to the rendering queue. The visual will be rendered using
	 * the current matrix (see pushMatrix).
	 */
	virtual void queue(Visual* v) = 0;

	
	/**
	 * Specify the background clear color.
	 */
	virtual void setBackgroundColor(Float r, Float g, Float b) = 0;


	/**
	 * Create a new texture from the current screen contents.
	 */
	virtual Texture* grabScreen(int left, int top, int right, int bottom) const = 0;

};


// --- begin scripting interface ---

SCRIPT_METHOD(RenderService, finish, RETURNS_VOID)
END_METHOD

SCRIPT_METHOD(RenderService, getViewMatrix, RETURNS_MATRIX)
END_METHOD

SCRIPT_METHOD(RenderService, getWorldMatrix, RETURNS_MATRIX)
END_METHOD

SCRIPT_METHOD(RenderService, grabScreen, RETURNS_SERVICE)
	ARG_INT, ARG_INT, ARG_INT, ARG_INT
END_METHOD

SCRIPT_METHOD(RenderService, queue, RETURNS_VOID)
	ARG_SERVICE(Visual)
END_METHOD

SCRIPT_METHOD(RenderService, setBackgroundColor, RETURNS_VOID)
	ARG_FLOAT, ARG_FLOAT, ARG_FLOAT
END_METHOD

SCRIPT_METHOD(RenderService, setProjection2D, RETURNS_VOID)
	ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT
END_METHOD

SCRIPT_METHOD(RenderService, setProjection3D, RETURNS_VOID)
	ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT
END_METHOD

SCRIPT_METHOD(RenderService, setProjectionMatrix, RETURNS_VOID)
	ARG_MATRIX
END_METHOD

SCRIPT_METHOD(RenderService, setViewMatrix, RETURNS_VOID)
	ARG_MATRIX
END_METHOD

SCRIPT_METHOD(RenderService, setWorldMatrix, RETURNS_VOID)
	ARG_MATRIX
END_METHOD

SCRIPT_METHOD(RenderService, start, RETURNS_VOID)
END_METHOD

EXPORTS(RenderService)
	INTERFACE(EventListener)
	METHOD(RenderService, finish)
	METHOD(RenderService, getViewMatrix)
	METHOD(RenderService, getWorldMatrix)
	METHOD(RenderService, grabScreen)
	METHOD(RenderService, queue)
	METHOD(RenderService, setBackgroundColor)
	METHOD(RenderService, setProjection2D)
	METHOD(RenderService, setProjection3D)
	METHOD(RenderService, setProjectionMatrix)
	METHOD(RenderService, setViewMatrix)
	METHOD(RenderService, setWorldMatrix)
	METHOD(RenderService, start)
END_EXPORTS

// --- end scripting interface ---


EXIT_NAMESPACE;
#endif

