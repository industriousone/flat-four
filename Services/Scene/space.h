/**
 * @file space.h
 *
 * @author  Jason Perkins (jason@379.com)
 * @date    Copyright (C) 2002 by 379, Inc.
 * @version $Id: space.h,v 1.2 2002/06/12 23:04:49 jason379 Exp $
 */
#if !defined(_F4_SCENE_SPACE_H_)
#define _F4_SCENE_SPACE_H_

#include "attribute.h"
ENTER_NAMESPACE(f4);


/**
 * An abstract interface to a group of entities. A "Space" allows entities to
 * be ordered into a hierarchical structure.
 *
 * @ingroup f4_xem
 */
struct Space : public Attribute
{

	/**
	 * Add an entity to the space.
	 */
	virtual void addEntity(Entity* ent) = 0;


	/**
	 * Locate an entity by name.
	 */
	virtual Entity* queryEntity(const Char* name) = 0;

};


EXIT_NAMESPACE;
#include "entity.h"
ENTER_NAMESPACE(f4);


// --- begin scripting interface ---

SCRIPT_METHOD(Space, addEntity, RETURNS_VOID)
	ARG_SERVICE(Entity)
END_METHOD

SCRIPT_METHOD(Space, queryEntity, RETURNS_SERVICE)
	ARG_STRING
END_METHOD

EXPORTS(Space)
	INTERFACE(Attribute)
	METHOD(Space, addEntity)
	METHOD(Space, queryEntity)
END_EXPORTS

// --- end scripting interface ---


EXIT_NAMESPACE;
#endif
