/**
 * @file visitor.h
 *
 * Entity tree visitor pattern.
 *
 * @author  Jason Perkins (jason@379.com)
 * @date    Copyright (C) 2002 by 379, Inc.
 * @version $Id: visitor.h,v 1.1 2002/06/19 01:39:59 jason379 Exp $
 */
#if !defined(_F4_SCENE_VISITOR_H_)
#define _F4_SCENE_VISITOR_H_

ENTER_NAMESPACE(f4);

struct Attribute;
struct Entity;


/**
 * An abstract visitor for the entity tree. 
 *
 * @ingroup f4_scene
 */
struct Visitor : public Service
{

	/**
	 * Descend into an entity.
	 */
	virtual void visitEntity(Entity* ent) = 0;


	/**
	 * Descend into an attribute.
	 */
	virtual void visitAttribute(Attribute* at) = 0;

};


// --- begin scripting interface ---

SCRIPT_METHOD(Visitor, visitAttribute, RETURNS_VOID)
	ARG_SERVICE(Attribute)
END_METHOD

SCRIPT_METHOD(Visitor, visitEntity, RETURNS_VOID)
	ARG_SERVICE(Entity)
END_METHOD

EXPORTS(Visitor)
	METHOD(Visitor, visitAttribute)
	METHOD(Visitor, visitEntity)
END_EXPORTS

// --- end scripting interface ---


EXIT_NAMESPACE;
#endif
