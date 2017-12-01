/**
 * @file font_factory.h
 *
 * @author  Jason Perkins (jason@379.com)
 * @date    Copyright (C) 2001 by 379, Inc.
 * @version $Id: font_factory.h,v 1.2 2002/04/22 01:51:56 jason379 Exp $
 */
#if !defined(_F4_TEXT_FONT_FACTORY_H_)
#define _F4_TEXT_FONT_FACTORY_H_

#include "texture_font.h"
ENTER_NAMESPACE(f4);


/**
 * Generate a new font object.
 *
 * FontFactory uses FreeType2 (http://www.freetype.org/) to generate the
 * font information. The FT2 shared library must be present in order for
 * this service to work.
 *
 * @ingroup f4_text
 */
struct FontFactory : public Service
{

	/**
	 * Initialize the factory and the FreeType engine.
	 *
	 * @return True if successful, false if the FreeType engine could not
	 *         be started.
	 */
	virtual bool initialize() = 0;


	/**
	 * Specify the typeface for the font. FontFactory supports TrueType and
	 * Type1 fonts.
	 *
	 * @param filename  The font file path.
	 */
	virtual bool setTypeface(const Char* filename) = 0;


	/**
	 * Specify the size of the character cell, in pixels. From the FreeType
	 * documentation: "The character size is really the size of an abstract
	 * square called the EM, used to design the font. However, depending on
	 * the font design, glyphs will be smaller or greater than the EM. This
	 * means that setting the pixel size to, say, 8x8 doesn't guarantee in
	 * any way that you will get glyph bitmaps that all fit within an 8x8
	 * cell (sometimes even far from it)."
	 *
	 * @return True if successful, false if @a pixels is out of the valid
	 *         range of sizes.
	 */
	virtual bool setPixelSize(int pixels) = 0;


	/**
	 * Specify a set of characters for the new font. If you do not specify
	 * a glyph list, you will get the standard ASCII character set.
	 */
	virtual void setGlyphs(const Char* glyphs) = 0;


	/**
	 * Specify a maximum size for the texture. The parameter @a size must be
	 * a power of two and greater than four. The default is 256.
	 */
	virtual bool setMaxTextureSize(int size) = 0;


	/**
	 * Enable/disable antialiasing. Enabled by default.
	 */
	virtual void antialias(bool yesNo) = 0;


	/**
	 * Create a texture font.
	 */
	virtual TextureFont* createTextureFont() = 0;

};


// --- begin scripting interface ---

SCRIPT_METHOD(FontFactory, antialias, RETURNS_VOID)
	ARG_BOOL
END_METHOD

SCRIPT_METHOD(FontFactory, createTextureFont, RETURNS_SERVICE)
END_METHOD

SCRIPT_METHOD(FontFactory, initialize, RETURNS_BOOL)
END_METHOD

SCRIPT_METHOD(FontFactory, setGlyphs, RETURNS_VOID)
	ARG_STRING
END_METHOD

SCRIPT_METHOD(FontFactory, setMaxTextureSize, RETURNS_BOOL)
	ARG_INT
END_METHOD

SCRIPT_METHOD(FontFactory, setPixelSize, RETURNS_BOOL)
	ARG_INT
END_METHOD

SCRIPT_METHOD(FontFactory, setTypeface, RETURNS_BOOL)
	ARG_STRING
END_METHOD

EXPORTS(FontFactory)
	METHOD(FontFactory, antialias)
	METHOD(FontFactory, createTextureFont)
	METHOD(FontFactory, initialize)
	METHOD(FontFactory, setGlyphs)
	METHOD(FontFactory, setMaxTextureSize)
	METHOD(FontFactory, setPixelSize)
	METHOD(FontFactory, setTypeface)
END_EXPORTS

// --- end scripting interface ---


EXIT_NAMESPACE;
#endif

