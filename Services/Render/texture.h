/**
 * @file texture.h
 *
 * @author  Jason Perkins (jason@379.com)
 * @date    Copyright (C) 2001 by 379, Inc.
 * @version $Id: texture.h,v 1.2 2002/04/22 01:51:56 jason379 Exp $
 *
 * @todo Add a 'flags' parameter to initialize, w/flag 'BGR'. If renderer
 *       doesn't support BGR textures, converts pixel data.
 *
 */
#if !defined(_F4_RENDER_TEXTURE_H_)
#define _F4_RENDER_TEXTURE_H_

ENTER_NAMESPACE(f4);


/**
 * Wraps a texture image and rendering settings.
 * @ingroup f4_render
 */
struct Texture : public Service
{

	/**
	 * Prepare a new texture object.
	 *
	 * @param width      The width of the texture in texels.
	 * @param height     The height of the texture in texels.
	 * @param num        The number of color components.
	 * @param size       The size of a single color component in bytes.
	 * @param pixels     The texture bitmap.
	 *
	 * @par Scripted as:
	 *      [method]
	 */
	virtual bool initialize(int width, int height, int num, int size, void* pixels) = 0;


	/**
	 * Get the width of the texture in texels.
	 * @par Scripted as:
	 *      [r/o int] @a width
	 */
	virtual int getWidth() const = 0;


	/**
	 * Get the height of the texture in texels.
	 * @par Scripted as:
	 *      [r/o int] @a height
	 */
	virtual int getHeight() const = 0;


	/**
	 * Get the number of color components.
	 * @par Scripted as:
	 *      [r/o int] @a components
	 */
	virtual int getComponents() const = 0;


	/**
	 * Get the size of a color component in bytes.
	 * @par Scripted as:
	 *      [r/o int] @a componentSize
	 */
	virtual int getComponentSize() const = 0;


	/**
	 * Get a pointer to the raw pixel data.
	 * @par Scripted as:
	 *      [r/o ptr] @a pixels
	 */
	virtual void* getPixels() = 0;


	/**
	 * Returns true if the texture contains an alpha channel.
	 * @par Scripted as:
	 *      [r/o bool] @a hasAlpha
	 */
	virtual bool hasAlpha() const = 0;


	// --- internal interface ---
	virtual void execute() = 0;
	// --- internal interface ---

};


// --- begin scripting interface ---

SCRIPT_METHOD(Texture, getComponents, RETURNS_INT)
END_METHOD

SCRIPT_METHOD(Texture, getComponentSize, RETURNS_INT)
END_METHOD

SCRIPT_METHOD(Texture, getHeight, RETURNS_INT)
END_METHOD

SCRIPT_METHOD(Texture, getPixels, RETURNS_PTR)
END_METHOD

SCRIPT_METHOD(Texture, getWidth, RETURNS_INT)
END_METHOD

SCRIPT_METHOD(Texture, hasAlpha, RETURNS_BOOL)
END_METHOD

SCRIPT_METHOD(Texture, initialize, RETURNS_BOOL)
	ARG_INT, ARG_INT, ARG_INT, ARG_INT, ARG_PTR(void)
END_METHOD

EXPORTS(Texture)
	RO_PROP_INT(Texture, components, getComponents)
	RO_PROP_INT(Texture, componentSize, getComponentSize)
	RO_PROP_BOOL(Texture, hasAlpha, hasAlpha)
	RO_PROP_INT(Texture, height, getHeight)
	RO_PROP_PTR(Texture, pixels, getPixels)
	RO_PROP_INT(Texture, width, getWidth)
	METHOD(Texture, initialize)
END_EXPORTS

// --- end scripting interface ---


EXIT_NAMESPACE;
#endif

