/**
 * @file motion_event.h
 *
 * @author  Jason Perkins (jason@379.com)
 * @date    Copyright (C) 2001 by 379, Inc.
 * @version $Id: motion_event.h,v 1.3 2002/06/02 18:55:02 jason379 Exp $
 */
#if !defined(_F4_SYSTEM_MOTION_EVENT_H_)
#define _F4_SYSTEM_MOTION_EVENT_H_

#include <Core/event.h>
ENTER_NAMESPACE(f4);


/**
 * Description of a mouse motion event. All coordinates are in screen space.
 *
 * @ingroup f4_system
 */
struct MotionEvent : public Event
{

	/**
	 * Initialize a new event.
	 */
	virtual bool initialize(int x, int y, int buttons) = 0;


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


	/**
	 * Get the relative change in X position since the last event.
	 * @par Scripted as:
	 *      [r/o int] @a relX
	 */
	virtual int getRelX() const = 0;


	/**
	 * Get the relative change in Y position since the last event.
	 * @par Scripted as:
	 *      [r/o int] @a relY
	 */
	virtual int getRelY() const = 0;


	/**
	 * Get the status of a particular button.
	 *
	 * @return True if the button is currently pressed.
	 *
	 * @par Scripted as:
	 *      <I>bool getButtonState(int)</I>
	 */
	virtual bool getButtonState(int button) const = 0;

};



// --- begin scripting interface ---

SCRIPT_METHOD(MotionEvent, getButtonState, RETURNS_BOOL)
	ARG_INT
END_METHOD

SCRIPT_METHOD(MotionEvent, getRelX, RETURNS_INT)
END_METHOD

SCRIPT_METHOD(MotionEvent, getRelY, RETURNS_INT)
END_METHOD

SCRIPT_METHOD(MotionEvent, getX, RETURNS_INT)
END_METHOD

SCRIPT_METHOD(MotionEvent, getY, RETURNS_INT)
END_METHOD

SCRIPT_METHOD(MotionEvent, initialize, RETURNS_BOOL)
	ARG_INT, ARG_INT, ARG_INT
END_METHOD

EXPORTS(MotionEvent)
	INTERFACE(Event)
	RO_PROP_INT(MotionEvent, relX, getRelX)
	RO_PROP_INT(MotionEvent, relY, getRelY)
	RO_PROP_INT(MotionEvent, x, getX)
	RO_PROP_INT(MotionEvent, y, getY)
	METHOD(MotionEvent, getButtonState)
	METHOD(MotionEvent, initialize)
END_EXPORTS

// --- end scripting interface ---


EXIT_NAMESPACE;
#endif
