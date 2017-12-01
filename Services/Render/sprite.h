/**
 * @file sprite.h
 *
 * @author  Jason Perkins (jason@379.com)
 * @date    Copyright (C) 2001 by 379, Inc.
 * @version $Id: sprite.h,v 1.2 2002/04/22 01:51:56 jason379 Exp $
 */
#if !defined(_F4_RENDER_SPRITE_H_)
#define _F4_RENDER_SPRITE_H_

#include "geometry.h"
ENTER_NAMESPACE(f4);


/**
 * A sprite is essentially a 2D quad. You may set the width and height of
 * the quad, as well as the texture extents.
 * @ingroup f4_render
 */
struct Sprite : public Geometry
{

	/**
	 * Specify the size of the sprite. The default is 1.0 units wide
	 * and tall.
	 */
	virtual bool initialize(Float width, Float height) = 0;


	/**
	 * Set the sprite height.
	 */
	virtual void setWidth(Float width) = 0;


	/**
	 * Get the sprite width.
	 */
	virtual Float getWidth() const = 0;


	/**
	 * Set the sprite height.
	 */
	virtual void setHeight(Float height) = 0;


	/**
	 * Get the sprite height.
	 */
	virtual Float getHeight() const = 0;


	/**
	 * Flip the sprite horizontally.
	 */
	virtual void flipX() = 0;


	/**
	 * Flip the sprite vertically.
	 */
	virtual void flipY() = 0;


	/**
	 * Is the sprite flipped horizontally?
	 */
	virtual bool isXFlipped() = 0;

	
	/**
	 * Is the sprite flipped vertically?
	 */
	virtual bool isYFlipped() = 0;

};


// --- begin scripting interface ---

SCRIPT_METHOD(Sprite, flipX, RETURNS_VOID)
END_METHOD

SCRIPT_METHOD(Sprite, flipY, RETURNS_VOID)
END_METHOD

SCRIPT_METHOD(Sprite, getHeight, RETURNS_FLOAT)
END_METHOD

SCRIPT_METHOD(Sprite, getWidth, RETURNS_FLOAT)
END_METHOD

SCRIPT_METHOD(Sprite, initialize, RETURNS_BOOL)
	ARG_FLOAT, ARG_FLOAT
END_METHOD

SCRIPT_METHOD(Sprite, isXFlipped, RETURNS_BOOL)
END_METHOD

SCRIPT_METHOD(Sprite, isYFlipped, RETURNS_BOOL)
END_METHOD

SCRIPT_METHOD(Sprite, setHeight, RETURNS_VOID)
	ARG_FLOAT
END_METHOD

SCRIPT_METHOD(Sprite, setWidth, RETURNS_VOID)
	ARG_FLOAT
END_METHOD

EXPORTS(Sprite)
	INTERFACE(Geometry)
	RW_PROP_FLOAT(Sprite, height, getHeight, setHeight)
	RW_PROP_FLOAT(Sprite, width,  getWidth,  setWidth)
	METHOD(Sprite, flipX)
	METHOD(Sprite, flipY)
	METHOD(Sprite, initialize)
	METHOD(Sprite, isXFlipped)
	METHOD(Sprite, isYFlipped)
END_EXPORTS

// --- end scripting interface ---


EXIT_NAMESPACE;
#endif
