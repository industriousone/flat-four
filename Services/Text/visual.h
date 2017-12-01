/**
 * @file visual.h
 *
 * @author  Jason Perkins (jason@379.com)
 * @date    Copyright (C) 2001 by 379, Inc.
 * @version $Id: visual.h,v 1.2 2002/04/22 01:51:56 jason379 Exp $
 */
#if !defined(_F4_TEXT_VISUAL_H_)
#define _F4_TEXT_VISUAL_H_

#include <Render/visual.h>
ENTER_NAMESPACE(f4);

struct TextureFont;


/**
 * A custom Visual class specialized for text rendering.
 * @ingroup f4_text
 */
struct TextVisual : public Visual
{

	enum { LEFT, RIGHT, CENTER, TOP, BOTTOM };


	/**
	 * Specify the text to be rendered by this visual.
	 * @par Scripted as:
	 *      [w/o string] @a text
	 */
	virtual void setText(const Char* msg) = 0;


	/**
	 * Set the text alignment. The default alignment is LEFT and BOTTOM.
	 *
	 * @param horiz  One of @c LEFT, @c CENTER, @c RIGHT.
	 * @param vert   One of @c TOP, @c CENTER, @c BOTTOM.
	 *
	 * @par Scripted as:
	 *      <I>setAlignment(int, int)</I>
	 */
	virtual void setAlignment(int horiz, int vert) = 0;


	// --- internal interface ---
	virtual bool initialize(TextureFont* font, Shader* shader) = 0;
	// --- internal interface ---

};



// --- begin scripting interface ---

SCRIPT_METHOD(TextVisual, setAlignment, RETURNS_VOID)
	ARG_INT, ARG_INT
END_METHOD

SCRIPT_METHOD(TextVisual, setText, RETURNS_VOID)
	ARG_STRING
END_METHOD

EXPORTS(TextVisual)
	INTERFACE(Visual)
	WO_PROP_STRING(TextVisual, text, setText)
	METHOD(TextVisual, setAlignment)
	CONST_INT(TextVisual, LEFT)
	CONST_INT(TextVisual, RIGHT)
	CONST_INT(TextVisual, CENTER)
	CONST_INT(TextVisual, TOP)
	CONST_INT(TextVisual, BOTTOM)
END_EXPORTS

// --- end scripting interface ---


EXIT_NAMESPACE;
#endif
