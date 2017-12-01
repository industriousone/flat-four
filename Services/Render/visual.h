/**
 * @file visual.h
 *
 * @author  Jason Perkins (jason@379.com)
 * @date    Copyright (C) 2001 by 379, Inc.
 * @version $Id: visual.h,v 1.2 2002/04/22 01:51:56 jason379 Exp $
 */
#if !defined(_F4_RENDER_VISUAL_H_)
#define _F4_RENDER_VISUAL_H_

#include "geometry.h"
#include "shader.h"
ENTER_NAMESPACE(f4);



/**
 * A Visual represents a unique geometry/shader pair.
 * @ingroup f4_render
 */
struct Visual : public Service
{

	/**
	 * Assign geometry and a shader to this visual.
	 */
	virtual bool initialize(Geometry* geom, Shader* shader) = 0;


	/**
	 * Get the encapsulated geometry object.
	 */
	virtual Geometry* getGeometry() = 0;


	/**
	 * Get the encapsulated shader object.
	 */
	virtual Shader* getShader() = 0;


	// --- internal interface ---
	virtual void execute() = 0;
	// --- internal interface ---

};


// --- begin scripting interface ---

SCRIPT_METHOD(Visual, getGeometry, RETURNS_SERVICE)
END_METHOD

SCRIPT_METHOD(Visual, getShader, RETURNS_SERVICE)
END_METHOD

SCRIPT_METHOD(Visual, initialize, RETURNS_VOID)
	ARG_SERVICE(Geometry), ARG_SERVICE(Shader)
END_METHOD

EXPORTS(Visual)
	RO_PROP_SERVICE(Visual, geom, getGeometry)
	RO_PROP_SERVICE(Visual, shader, getShader)
	METHOD(Visual, initialize)
END_EXPORTS

// --- end scripting interface ---


EXIT_NAMESPACE;
#endif
