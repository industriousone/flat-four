/**
 * @file time_event.h
 *
 * @author  Jason Perkins (jason@379.com)
 * @date    Copyright (C) 2001 by 379, Inc.
 * @version $Id: time_event.h,v 1.2 2002/04/22 01:51:56 jason379 Exp $
 */
#if !defined(_F4_SYSTEM_TIME_EVENT_H_)
#define _F4_SYSTEM_TIME_EVENT_H_

#include <Core/event.h>
ENTER_NAMESPACE(f4);


/**
 * Time events are sent whenever the system event queue is empty.
 * @ingroup f4_system
 */
struct TimeEvent : public Event
{

	/**
	 * Initialize a new event.
	 * @par Scripted as:
	 *      <I>bool initialize(float)</I>
	 */
	virtual bool initialize(Float elapsedTime) = 0;


	/**
	 * Retrieve the elapsed time, in seconds, since the last TimeEvent.
	 * @par Scripted as:
	 *      [r/w float] @a elapsed
	 */
	virtual Float getElapsed() const = 0;


	/**
	 * Set the elapsed time.
	 * @par Scripted as:
	 *      [r/w float] @a elapsed
	 */
	virtual void setElapsed(Float elapsedTime) = 0;

};


// --- begin scripting interface ---

SCRIPT_METHOD(TimeEvent, getElapsed, RETURNS_FLOAT)
END_METHOD

SCRIPT_METHOD(TimeEvent, initialize, RETURNS_BOOL)
	ARG_FLOAT
END_METHOD

SCRIPT_METHOD(TimeEvent, setElapsed, RETURNS_VOID)
	ARG_FLOAT
END_METHOD

EXPORTS(TimeEvent)
	INTERFACE(Event)
	RW_PROP_FLOAT(TimeEvent, elapsed, getElapsed, setElapsed)
	METHOD(TimeEvent, initialize)
END_EXPORTS

// --- end scripting interface ---


EXIT_NAMESPACE;
#endif
