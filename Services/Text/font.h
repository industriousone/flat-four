/**
 * @file font.h
 *
 * @author  Jason Perkins (jason@379.com)
 * @date    Copyright (C) 2001 by 379, Inc.
 * @version $Id: font.h,v 1.2 2002/04/22 01:51:56 jason379 Exp $
 */
#if !defined(_F4_TEXT_FONT_H_)
#define _F4_TEXT_FONT_H_

ENTER_NAMESPACE(f4);


/**
 * Abstract interface to font objects.
 * @ingroup f4_text
 */
struct Font : public Service
{

};


// --- begin scripting interface ---

EXPORTS(Font)
END_EXPORTS

// --- end scripting interface ---


EXIT_NAMESPACE;
#endif
