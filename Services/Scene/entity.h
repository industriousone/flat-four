/**
 * @file entity.h
 *
 * @author  Jason Perkins (jason@379.com)
 * @date    Copyright (C) 2002 by 379, Inc.
 * @version $Id: entity.h,v 1.6 2002/06/19 01:39:59 jason379 Exp $
 *
 * @todo queryAttribute() can only return the first instance of the requested
 *       type. Should include a mechanism to iterate all of the attributes.
 *
 */
#if !defined(_F4_SCENE_ENTITY_H_)
#define _F4_SCENE_ENTITY_H_

#include "attribute.h"
ENTER_NAMESPACE(f4);


/**
 * An entity represents an object in the scene. Entities are generic and
 * consistent: you don't create new implementations of the Entity 
 * interface. Instead, you create a generic entity, and then specialize
 * it by adding @a Attributes to it. Each attribute contributes to the
 * final behavior of the entity. 
 *
 * Note that attributes are always processed in order, and the first one
 * added will be the first one processed. This can have some effect on
 * the final behavior of the entity. Also important: in general, entities
 * must be singular, that is they cannot appear in the hierarchy more than
 * once. 
 *
 * @ingroup f4_scene
 */
struct Entity : public Service
{

	/**
	 * Every entity is identified by a text name.
	 */
	virtual bool initialize(const Char* name) = 0;


	/**
	 * Change the entity's name.
	 * @par Scripted as:
	 *      [r/w string] @a name
	 */
	virtual void setName(const Char* name) = 0;


	/**
	 * Retrieve an entity's name.
	 * @par Scripted as:
	 *      [r/w string] @a name
	 */
	virtual const Char* getName() const = 0;


	/**
	 * Add a new attribute to the end of the attribute list.
	 */
	virtual void addAttribute(Attribute* a) = 0;


	/**
	 * Remove an attribute from the entity.
	 */
	virtual void removeAttribute(Attribute* a) = 0;


	/**
	 * Insert a new attribute into the list. If the attribute @a before is
	 * not part of this entity, this method will behave the same as
	 * addAttribute().
	 */
	virtual void insertAttribute(Attribute* a, Attribute* before) = 0;


	/**
	 * Replace one attribute with another, without changing the order of the
	 * attributes. If the target attribute does not exist, this method will
	 * behave the same as addAttribute().
	 */
	virtual void replaceAttribute(Attribute* aOld, Attribute* aNew) = 0;


	/**
	 * Locate an attribute of an entity.
	 */
	virtual Attribute* queryAttribute(const Char* type) = 0;


	/**
	 * Traverse the list of attributes to perform some operation.
	 */
	virtual void visit(Visitor* v) = 0;


	/**
	 * Respond to a collision with another entity. Unlike the update() and
	 * render(), this method is not propogated to the entity's children.
	 */
	virtual void touch(CollisionVisitor* cv) = 0;

};



// --- begin scripting interface ---

SCRIPT_METHOD(Entity, addAttribute, RETURNS_VOID)
	ARG_SERVICE(Attribute)
END_METHOD

SCRIPT_METHOD(Entity, getName, RETURNS_STRING)
END_METHOD

SCRIPT_METHOD(Entity, initialize, RETURNS_BOOL)
	ARG_STRING
END_METHOD

SCRIPT_METHOD(Entity, insertAttribute, RETURNS_VOID)
	ARG_SERVICE(Attribute), ARG_SERVICE(Attribute)
END_METHOD

SCRIPT_METHOD(Entity, queryAttribute, RETURNS_SERVICE)
	ARG_STRING
END_METHOD

SCRIPT_METHOD(Entity, removeAttribute, RETURNS_VOID)
	ARG_SERVICE(Attribute)
END_METHOD

SCRIPT_METHOD(Entity, replaceAttribute, RETURNS_VOID)
	ARG_SERVICE(Attribute), ARG_SERVICE(Attribute)
END_METHOD

SCRIPT_METHOD(Entity, setName, RETURNS_VOID)
	ARG_STRING
END_METHOD

SCRIPT_METHOD(Entity, touch, RETURNS_VOID)
	ARG_SERVICE(CollisionVisitor)
END_METHOD

SCRIPT_METHOD(Entity, visit, RETURNS_VOID)
	ARG_SERVICE(Visitor)
END_METHOD

EXPORTS(Entity)
	RW_PROP_STRING(Entity, name, getName, setName)
	METHOD(Entity, addAttribute)
	METHOD(Entity, initialize)
	METHOD(Entity, insertAttribute)
	METHOD(Entity, queryAttribute)
	METHOD(Entity, removeAttribute)
	METHOD(Entity, replaceAttribute)
	METHOD(Entity, touch)
	METHOD(Entity, visit)
END_EXPORTS

// --- end scripting interface ---


EXIT_NAMESPACE;
#endif

