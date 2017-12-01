/**
 * @file simple_space.h
 *
 * @author  Jason Perkins (jason@379.com)
 * @date    Copyright (C) 2002 by 379, Inc.
 * @version $Id: simple_space.h,v 1.1 2002/05/18 10:59:15 jason379 Exp $
 */
#if !defined(_F4_SCENE_SIMPLE_SPACE_H_)
#define _F4_SCENE_SIMPLE_SPACE_H_

#include "space.h"
ENTER_NAMESPACE(f4);


/**
 * A simple space stores a list of entities with no spatial organization.
 *
 * @ingroup f4_scene
 */
struct SimpleSpace : public Space
{

};


// --- begin scripting interface ---

EXPORTS(SimpleSpace)
	INTERFACE(Space)
END_EXPORTS

// --- end scripting interface ---


EXIT_NAMESPACE;
#endif
