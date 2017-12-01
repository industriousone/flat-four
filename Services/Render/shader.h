/**
 * @file shader.h
 *
 * @author  Jason Perkins (jason@379.com)
 * @date    Copyright (C) 2001 by 379, Inc.
 * @version $Id: shader.h,v 1.2 2002/04/22 01:51:56 jason379 Exp $
 */
#if !defined(_F4_RENDER_SHADER_H_)
#define _F4_RENDER_SHADER_H_

#include "texture.h"
ENTER_NAMESPACE(f4);



/**
 * A description of the appearance of a surface.
 * @ingroup f4_render
 */
struct Shader : public Service
{
	/// Values for setAlphaBlend()
	enum BlendOps 
	{ 
		ZERO, ONE,
		DST_COLOR, INV_DST_COLOR, SRC_COLOR, INV_SRC_COLOR,
		DST_ALPHA, INV_DST_ALPHA, SRC_ALPHA, INV_SRC_ALPHA
	};

	/// Flags for setTextureStyle()
	enum TextureStyles
	{
		MODULATE = 0x00, REPEAT = 0x00,      // defaults
		DECAL = 0x01, CLAMP = 0x02
	};


	/**
	 * Enable alpha blending.
	 * @par Scripted as:
	 *      <I>enableAlphaBlend</I>
	 */
	virtual void enableAlphaBlend() = 0;


	/**
	 * Disable alpha blending.
	 * @par Scripted as:
	 *      <I>disableAlphaBlend</I>
	 */
	virtual void disableAlphaBlend() = 0;


	/**
	 * Enable Z testing.
	 * @par Scripted as:
	 *      <I>enableDepthTest</I>
	 */
	virtual void enableDepthTest() = 0;

	
	/**
	 * Disable depth testing.
	 * @par Scripted as:
	 *      <I>disableDepthTest()</I>
	 */
	virtual void disableDepthTest() = 0;


	/**
	 * Set the alpha blending mode.
	 * @par Scripted as:
	 *      <I>setAlphaBlend(int, int)</I>
	 */
	virtual void setAlphaBlend(BlendOps src, BlendOps dst) = 0;


	/**
	 * The surface base color. This may be overridden by the geometry 
	 * vertices.
	 * @par Scripted as:
	 *      <I>setColor(float, float, float)</I>
	 */
	virtual void setColor(Float red, Float green, Float blue) = 0;

	
	/**
	 * Set the alpha level. Zero is transparent, one is opaque.
	 * @par Scripted as:
	 *      <I>setAlpha(float)</I>
	 */
	virtual void setAlpha(Float alpha) = 0;


	/**
	 * Specify a texture. Set texture to @c NULL to clear the texture. If
	 * the texture contains an alpha channel, alpha blending will be enabled
	 * automatically.
	 * @par Scripted as:
	 *      [r/w service] @a texture
	 */
	virtual void setTexture(Texture* t) = 0;


	/**
	 * Retrieve the texture.
	 * @par Scripted as:
	 *      [r/w service] @a texture
	 */
	virtual Texture* getTexture() = 0;


	/**
	 * Specify the texturing style.
	 */
	virtual void setTextureStyle(TextureStyles style) = 0;


	// --- internal interface ---
	virtual void execute() = 0;
	// --- internal interface ---

};


// --- begin scripting interface ---

SCRIPT_METHOD(Shader, disableAlphaBlend, RETURNS_VOID)
END_METHOD

SCRIPT_METHOD(Shader, disableDepthTest, RETURNS_VOID)
END_METHOD

SCRIPT_METHOD(Shader, enableAlphaBlend, RETURNS_VOID)
END_METHOD

SCRIPT_METHOD(Shader, enableDepthTest, RETURNS_VOID)
END_METHOD

SCRIPT_METHOD(Shader, getTexture, RETURNS_SERVICE)
END_METHOD

SCRIPT_METHOD(Shader, setAlpha, RETURNS_VOID)
	ARG_FLOAT
END_METHOD

SCRIPT_METHOD(Shader, setAlphaBlend, RETURNS_VOID)
	ARG_ENUM(Shader::BlendOps), ARG_ENUM(Shader::BlendOps)
END_METHOD

SCRIPT_METHOD(Shader, setColor, RETURNS_VOID)
	ARG_FLOAT, ARG_FLOAT, ARG_FLOAT
END_METHOD

SCRIPT_METHOD(Shader, setTexture, RETURNS_VOID)
	ARG_SERVICE(Texture)
END_METHOD

SCRIPT_METHOD(Shader, setTextureStyle, RETURNS_VOID)
	ARG_ENUM(Shader::TextureStyles)
END_METHOD

EXPORTS(Shader)
	RW_PROP_SERVICE(Shader, texture, getTexture, setTexture)
	METHOD(Shader, disableAlphaBlend)
	METHOD(Shader, disableDepthTest)
	METHOD(Shader, enableAlphaBlend)
	METHOD(Shader, enableDepthTest)
	METHOD(Shader, setAlpha)
	METHOD(Shader, setAlphaBlend)
	METHOD(Shader, setColor)
	METHOD(Shader, setTextureStyle)
	CONST_INT(Shader, ZERO)
	CONST_INT(Shader, ONE)
	CONST_INT(Shader, DST_COLOR)
	CONST_INT(Shader, INV_DST_COLOR)
	CONST_INT(Shader, SRC_COLOR)
	CONST_INT(Shader, INV_SRC_COLOR)
	CONST_INT(Shader, DST_ALPHA)
	CONST_INT(Shader, INV_DST_ALPHA)
	CONST_INT(Shader, SRC_ALPHA)
	CONST_INT(Shader, INV_SRC_ALPHA)
	CONST_INT(Shader, DECAL)
	CONST_INT(Shader, MODULATE)
END_EXPORTS

// --- end scripting interface ---


EXIT_NAMESPACE;
#endif
