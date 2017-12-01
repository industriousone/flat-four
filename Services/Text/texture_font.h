/**
 * @file texture_font.h
 *
 * @author  Jason Perkins (jason@379.com)
 * @date    Copyright (C) 2001 by 379, Inc.
 * @version $Id: texture_font.h,v 1.2 2002/04/22 01:51:56 jason379 Exp $
 */
#if !defined(_F4_TEXT_TEXTURE_FONT_H_)
#define _F4_TEXT_TEXTURE_FONT_H_

#include <Render/texture.h>
#include "font.h"
#include "visual.h"
ENTER_NAMESPACE(f4);

struct Texture;


/**
 * A texture font uses bitmaps of each character, collected onto one large
 * texture, to render text.
 *
 * @ingroup f4_text
 */
struct TextureFont : public Font
{

	/**
	 * Set the base values for the font.
	 */
	virtual bool initialize(int height, Texture* texture) = 0;


	/**
	 * Create a new text visual, used to render a text string with this font.
	 */
	virtual TextVisual* createVisual() = 0;


	/**
	 * Set up a glyph description.
	 *
	 * @param code        The character code of the glyph, ASCII or whatever.
	 * @param x,y         The position of the top left corner of the glyph on 
	 *                    the texture.
	 * @param w,h         The size of the glyph on the texture.
	 * @param advance     How far to move horizontally before drawing the next
	 *                    glyph.
	 * @param left,bottom The lower left corner of the glyph within it's own
	 *                    bounding box (offset from x, y).
	 */
	virtual void setGlyph(int code, int x, int y, int w, int h, int advance, int left, int bottom) = 0;


	/**
	 * Retrieve the number of glyphs in the font.
	 */
	virtual int countGlyphs() const = 0;


	/**
	 * Get a list of the glyph codes used in the font. @codes should be an
	 * array at least as large as the value returned by countGlyphs().
	 */
	virtual void getGlyphCodes(int* codes) = 0;


	/**
	 * Get information about a particular glyph. @a glyph should be an array
	 * of at least seven integers, which will be filled in the following
	 * order:
	 *     - [0]   x position of the glyph on the texture
	 *     - [1]   y position of the glyph on the texture
	 *     - [2]   width of the glyph in texels
	 *     - [3]   height of the glyph in texels
	 *     - [4]   amount to advance horizontally before drawing next glyph
	 *     - [5]   horizontal offset from the current drawing position
	 *     - [6]   vertical offset from the current drawing position
	 *
	 * @return True if successful, false if @a code does not exist in the 
	 *         font.
	 */
	virtual bool getGlyphInfo(int code, int* glyph) = 0;


	/**
	 * Retrieve the line height.
	 */
	virtual int getHeight() const = 0;


	/**
	 * Retrieve the font texture.
	 */
	virtual Texture* getTexture() = 0;

};


// --- begin scripting interface ---

SCRIPT_METHOD(TextureFont, countGlyphs, RETURNS_INT)
END_METHOD

SCRIPT_METHOD(TextureFont, createVisual, RETURNS_SERVICE)
END_METHOD

SCRIPT_METHOD(TextureFont, getGlyphCodes, RETURNS_VOID)
	ARG_PTR(int)
END_METHOD

SCRIPT_METHOD(TextureFont, getGlyphInfo, RETURNS_BOOL)
	ARG_INT, ARG_PTR(int)
END_METHOD

SCRIPT_METHOD(TextureFont, getHeight, RETURNS_INT)
END_METHOD

SCRIPT_METHOD(TextureFont, getTexture, RETURNS_SERVICE)
END_METHOD

SCRIPT_METHOD(TextureFont, setGlyph, RETURNS_VOID)
	ARG_INT, ARG_INT, ARG_INT, ARG_INT, ARG_INT, ARG_INT, ARG_INT, ARG_INT
END_METHOD

SCRIPT_METHOD(TextureFont, initialize, RETURNS_BOOL)
	ARG_INT, ARG_SERVICE(Texture)
END_METHOD

EXPORTS(TextureFont)
	INTERFACE(Font)
	RO_PROP_INT(TextureFont, height, getHeight)
	RO_PROP_SERVICE(TextureFont, texture, getTexture)
	METHOD(TextureFont, countGlyphs)
	METHOD(TextureFont, createVisual)
	METHOD(TextureFont, getGlyphCodes)
	METHOD(TextureFont, getGlyphInfo)
	METHOD(TextureFont, setGlyph)
	METHOD(TextureFont, initialize)
END_EXPORTS

// --- end scripting interface ---


EXIT_NAMESPACE;
#endif
