/**
 * @file attribute.h
 *
 * @author  Jason Perkins (jason@379.com)
 * @date    Copyright (C) 2002 by 379, Inc.
 * @version $Id: attribute.h,v 1.7 2002/07/16 01:11:36 jason379 Exp $
 *
 * @todo The render, update, etc. methods should not have be called on every
 *       entity. Add a bitmask and accessors to control which messages are
 *       actually needed, store and apply the bitmask in Entity.
 *
 */
#if !defined(_F4_SCENE_ATTRIBUTE_H_)
#define _F4_SCENE_ATTRIBUTE_H_

ENTER_NAMESPACE(f4);

struct Entity;
struct EventVisitor;
struct RenderVisitor;
struct UpdateVisitor;
struct CollisionVisitor;
struct Visitor;



/**
 * An attribute defines a new behavior for an entity. This is a generic
 * interface which allows the attribute to contribute to the scene.
 *
 * @ingroup f4_scene
 */
struct Attribute : public Service
{

	/**
	 * Connect this attribute to an entity. In general, you shouldn't call
	 * this method directly, rather use Entity::addAttribute(). The default
	 * implementation of this method does nothing.
	 */
	virtual void connect(Entity* ent) { }


	/**
	 * Disconnect this attribute from an entity. In general, you shouldn't
	 * call this method directly, rather use Entity::removeAttribute(). The
	 * default implementation of this method does nothing.
	 */
	virtual void disconnect() { }


	/**
	 * Update the state of this attribute. The default implementation of
	 * this method does nothing.
	 */
	virtual void update(UpdateVisitor* uv) { }


	/**
	 * Render the contents of this attribute. The default implementation of
	 * this method does nothing.
	 */
	virtual void render(RenderVisitor* rv) { }


	/**
	 * Test for collisions between entities.
	 */
	virtual void collide(CollisionVisitor* cv) { }


	/**
	 * Respond to a collision with another entity.
	 */
	virtual void touch(CollisionVisitor* cv) { }


	/**
	 * Handle an event message. 
	 */
	virtual void event(EventVisitor* ev) { }

};



// --- begin scripting interface ---

SCRIPT_METHOD(Attribute, collide, RETURNS_VOID)
	ARG_SERVICE(CollisionVisitor)
END_METHOD

SCRIPT_METHOD(Attribute, connect, RETURNS_VOID)
	ARG_SERVICE(Entity)
END_METHOD

SCRIPT_METHOD(Attribute, disconnect, RETURNS_VOID)
END_METHOD

SCRIPT_METHOD(Attribute, event, RETURNS_VOID)
	ARG_SERVICE(EventVisitor)
END_METHOD

SCRIPT_METHOD(Attribute, render, RETURNS_VOID)
	ARG_SERVICE(RenderVisitor)
END_METHOD

SCRIPT_METHOD(Attribute, touch, RETURNS_VOID)
	ARG_SERVICE(CollisionVisitor)
END_METHOD

SCRIPT_METHOD(Attribute, update, RETURNS_VOID)
	ARG_SERVICE(UpdateVisitor)
END_METHOD

EXPORTS(Attribute)
	METHOD(Attribute, collide)
	METHOD(Attribute, connect)
	METHOD(Attribute, disconnect)
	METHOD(Attribute, event)
	METHOD(Attribute, render)
	METHOD(Attribute, update)
END_EXPORTS

// --- end scripting interface ---


EXIT_NAMESPACE;
#endif
