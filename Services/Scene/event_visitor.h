/**
 * @file event_visitor.h
 *
 * @author  Jason Perkins (jason@379.com)
 * @date    Copyright (C) 2002 by 379, Inc.
 * @version $Id: event_visitor.h,v 1.1 2002/07/16 01:14:10 jason379 Exp $
 */
#if !defined(_F4_SCENE_EVENT_VISITOR_H_)
#define _F4_SCENE_EVENT_VISITOR_H_

#include "visitor.h"
#include <Core/event.h>
ENTER_NAMESPACE(f4);


/**
 * Send an event message to an entity tree.
 *
 * @ingroup f4_scene
 */
struct EventVisitor	: public Visitor
{

	/**
	 * Send a new event into the tree.
	 */
	virtual void dispatch(Entity* root, Event* event) = 0;


	/**
	 * Retrieve the event being dispatch.
	 */
	virtual Event* getEvent() = 0;

};


// --- begin scripting interface ---

SCRIPT_METHOD(EventVisitor, dispatch, RETURNS_VOID)
	ARG_SERVICE(Entity), ARG_SERVICE(Event)
END_METHOD

SCRIPT_METHOD(EventVisitor, getEvent, RETURNS_SERVICE)
END_METHOD

EXPORTS(EventVisitor)
	INTERFACE(Visitor)
	RO_PROP_SERVICE(EventVisitor, event, getEvent)
	METHOD(EventVisitor, dispatch)
END_EXPORTS

// --- end scripting interface ---


EXIT_NAMESPACE;
#endif
