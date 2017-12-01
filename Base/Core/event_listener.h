/**
 * @file event_listener.h
 *
 * @author  Jason Perkins (jason@379.com)
 * @date    Copyright (C) 2001 by 379, Inc.
 * @version $Id: event_listener.h,v 1.2 2002/04/22 01:51:55 jason379 Exp $
 */
#if !defined(_F4_CORE_EVENT_LISTENER_H_)
#define _F4_CORE_EVENT_LISTENER_H_

ENTER_NAMESPACE(f4);

struct Event;



/**
 * A target for events.
 * @ingroup f4_core
 */
struct EventListener : public Service
{

	/**
	 * Receive a new event.
	 */
	virtual bool receive(Event* e) = 0;

};


// --- begin scripting interface ---

SCRIPT_METHOD(EventListener, receive, RETURNS_BOOL)
	ARG_SERVICE(Event)
END_METHOD

EXPORTS(EventListener)
	METHOD(EventListener, receive)
END_EXPORTS

// --- end scripting interface ---


EXIT_NAMESPACE;
#endif
