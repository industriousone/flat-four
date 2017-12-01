/**
 * @file click_event.h
 *
 * @author  Jason Perkins (jason@379.com)
 * @date    Copyright (C) 2001 by 379, Inc.
 * @version $Id: click_event.h,v 1.3 2002/06/02 18:55:02 jason379 Exp $
 */
#if !defined(_F4_SYSTEM_CLICK_EVENT_H_)
#define _F4_SYSTEM_CLICK_EVENT_H_

#include <Core/event.h>
ENTER_NAMESPACE(f4);


/**
 * Description of a mouse click event. All coordinates are in screen space.
 *
 * @ingroup f4_system
 */
struct ClickEvent : public Event
{

	/**
	 * Initialize a new event.
	 */
	virtual bool initialize(int button, bool pressed, int x, int y) = 0;


	/**
	 * Get the index of the pressed button.
	 * @par Scripted as:
	 *      [r/o int] @a button
	 */
	virtual int getButton() const = 0;


	/**
	 * Returns @c true if the button is currently pressed.
	 * @par Scripted as:
	 *      [r/o int] @a pressed
	 */
	virtual bool isPressed() const = 0;


	/**
	 * Get the current X position.
	 * @par Scripted as:
	 *      [r/o int] @a x
	 */
	virtual int getX() const = 0;


	/**
	 * Get the current Y position.
	 * @par Scripted as:
	 *      [r/o int] @a y
	 */
	virtual int getY() const = 0;


};



// --- begin scripting interface ---

SCRIPT_METHOD(ClickEvent, getButton, RETURNS_INT)
END_METHOD

SCRIPT_METHOD(ClickEvent, getX, RETURNS_INT)
END_METHOD

SCRIPT_METHOD(ClickEvent, getY, RETURNS_INT)
END_METHOD

SCRIPT_METHOD(ClickEvent, initialize, RETURNS_BOOL)
	ARG_INT, ARG_BOOL, ARG_INT, ARG_INT
END_METHOD

SCRIPT_METHOD(ClickEvent, isPressed, RETURNS_BOOL)
END_METHOD

EXPORTS(ClickEvent)
	INTERFACE(Event)
	RO_PROP_INT(ClickEvent, button, getButton)
	RO_PROP_INT(ClickEvent, x, getX)
	RO_PROP_INT(ClickEvent, y, getY)
	RO_PROP_BOOL(ClickEvent, pressed, isPressed)
	METHOD(ClickEvent, initialize)
END_EXPORTS

// --- end scripting interface ---


EXIT_NAMESPACE;
#endif

