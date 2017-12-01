/**
 * @file display_event.h
 *
 * @author  Jason Perkins (jason@379.com)
 * @date    Copyright (C) 2001 by 379, Inc.
 * @version $Id: display_event.h,v 1.2 2002/04/22 01:51:56 jason379 Exp $
 */
#if !defined(_F4_SYSTEM_DISPLAY_EVENT_H_)
#define _F4_SYSTEM_DISPLAY_EVENT_H_

#include <Core/event.h>
ENTER_NAMESPACE(f4);


/**
 * This message is sent whenever the size of the display changes.
 * @ingroup f4_system
 */
struct DisplayEvent : public Event
{
	/**
	 * Prepare the event for dispatch.
	 */
	virtual bool initialize(int width, int height) = 0;


	/**
	 * Get the new width of the display.
	 */
	virtual int getWidth() const = 0;


	/**
	 * Get the new height of the display.
	 */
	virtual int getHeight() const = 0;

};


// --- begin scripting interface ---

SCRIPT_METHOD(DisplayEvent, initialize, RETURNS_BOOL)
	ARG_INT, ARG_INT
END_METHOD

SCRIPT_METHOD(DisplayEvent, getWidth, RETURNS_INT)
END_METHOD

SCRIPT_METHOD(DisplayEvent, getHeight, RETURNS_INT)
END_METHOD

EXPORTS(DisplayEvent)
	RO_PROP_INT(DisplayEvent, width, getWidth)
	RO_PROP_INT(DisplayEvent, height, getHeight)
	METHOD(DisplayEvent, initialize)
	INTERFACE(Event)
END_EXPORTS

// --- end scripting interface ---


EXIT_NAMESPACE;
#endif
