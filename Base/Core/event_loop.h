/**
 * @file event_loop.h
 *
 * @author  Jason Perkins (jason@379.com)
 * @date    Copyright (C) 2001 by 379, Inc.
 * @version $Id: event_loop.h,v 1.2 2002/04/22 01:51:55 jason379 Exp $
 */
#if !defined(_F4_CORE_EVENT_LOOP_H_)
#define _F4_CORE_EVENT_LOOP_H_

#include "event.h"
ENTER_NAMESPACE(f4);


/**
 * A standardized event loop interface. Used by CoreService to talk to
 * whoever provides the event loop services.
 *
 * @ingroup f4_core
 */
struct EventLoop : public Service
{

	/**
	 * Retrieve an event from the loop. 
	 * @return An event object if one is available, or NULL if the event
	 *         queue is empty.
	 */
	virtual Event* getEvent() = 0;

};


// --- begin scripting interface ---

SCRIPT_METHOD(EventLoop, getEvent, RETURNS_SERVICE) END_METHOD

EXPORTS(EventLoop)
	METHOD(EventLoop, getEvent)
END_EXPORTS

// --- end scripting interface ---


EXIT_NAMESPACE;
#endif
