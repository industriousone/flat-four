/**
 * @file body.h
 *
 * @author  Jason Perkins (jason@379.com)
 * @date    Copyright (C) 2002 by 379, Inc.
 * @version $Id: body.h,v 1.2 2002/06/02 18:54:48 jason379 Exp $
 */
#if !defined(_F4_SCENE_BODY_H_)
#define _F4_SCENE_BODY_H_

#include <Render/visual.h>
#include "attribute.h"
ENTER_NAMESPACE(f4);



/**
 * The Body attribute provides a visual representation of the entity.
 *
 * @ingroup f4_scene
 */
struct Body : public Attribute
{

	/**
	 * Adds a new visual to the body.
	 */
	virtual void addVisual(Visual* viz) = 0;


	/**
	 * Retrieves the number of visuals in the body.
	 */
	virtual int countVisuals() const = 0;


	/**
	 * Retrieves a visual by index.
	 */
	virtual Visual* getVisual(int i) = 0;

};



// --- begin scripting interface ---

SCRIPT_METHOD(Body, addVisual, RETURNS_VOID)
	ARG_SERVICE(Visual)
END_METHOD

SCRIPT_METHOD(Body, countVisuals, RETURNS_INT)
END_METHOD

SCRIPT_METHOD(Body, getVisual, RETURNS_SERVICE)
	ARG_INT
END_METHOD

EXPORTS(Body)
	INTERFACE(Attribute)
	METHOD(Body, addVisual)
	METHOD(Body, countVisuals)
	METHOD(Body, getVisual)
END_EXPORTS

// --- end scripting interface ---


EXIT_NAMESPACE;
#endif
