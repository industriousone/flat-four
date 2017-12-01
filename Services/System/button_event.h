/**
 * @file button_event.h
 *
 * @author  Jason Perkins (jason@379.com)
 * @date    Copyright (C) 2001 by 379, Inc.
 * @version $Id: button_event.h,v 1.3 2002/06/02 18:55:02 jason379 Exp $
 */
#if !defined(_F4_SYSTEM_BUTTON_EVENT_H_)
#define _F4_SYSTEM_BUTTON_EVENT_H_

#include <Core/event.h>
ENTER_NAMESPACE(f4);


/**
 * Description of game controller button event.
 * @ingroup f4_system
 */
struct ButtonEvent : public Event
{

	/**
	 * Initialize a new event.
	 */
	virtual bool initialize(int device, int button, bool pressed) = 0;


	/**
	 * Get the device index.
	 * @par Scripted as:
	 *      [r/o int] @a device
	 */
	virtual int getDevice() const = 0;


	/**
	 * Get the button index.
	 * @par Scripted as
	 *      [r/o int] @a button
	 */
	virtual int getButton() const = 0;


	/**
	 * Returns @c true if the button is currently pressed.
	 * @par Scripted as:
	 *      [r/o int] @a pressed
	 */
	virtual bool isPressed() const = 0;

};


// --- begin scripting interface ---

SCRIPT_METHOD(ButtonEvent, getButton, RETURNS_INT)
END_METHOD

SCRIPT_METHOD(ButtonEvent, getDevice, RETURNS_INT)
END_METHOD

SCRIPT_METHOD(ButtonEvent, initialize, RETURNS_BOOL)
	ARG_INT, ARG_INT, ARG_BOOL
END_METHOD

SCRIPT_METHOD(ButtonEvent, isPressed, RETURNS_BOOL)
END_METHOD

EXPORTS(ButtonEvent)
	INTERFACE(Event)
	RO_PROP_INT(ButtonEvent, button, getButton)
	RO_PROP_INT(ButtonEvent, device, getDevice)
	RO_PROP_BOOL(ButtonEvent, pressed, isPressed)
	METHOD(ButtonEvent, initialize)
END_EXPORTS

// --- end scripting interface ---


EXIT_NAMESPACE;
#endif
