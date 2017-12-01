/**
 * @file axis_event.h
 *
 * @author  Jason Perkins (jason@379.com)
 * @date    Copyright (C) 2001 by 379, Inc.
 * @version $Id: axis_event.h,v 1.3 2002/06/02 18:55:02 jason379 Exp $
 */
#if !defined(_F4_SYSTEM_AXIS_EVENT_H_)
#define _F4_SYSTEM_AXIS_EVENT_H_

#include <Core/event.h>
ENTER_NAMESPACE(f4);


/**
 * Description of game controller axis change event.
 * @ingroup f4_system
 */
struct AxisEvent : public Event
{

	/**
	 * Prepare a new event.
	 *
	 * @param device    The controller index.
	 * @param axis      The axis index.
	 * @param pos       The current position of the axis.
	 */
	virtual bool initialize(int device, int axis, Float pos) = 0;


	/**
	 * Get the device index.
	 * @par Scripted as:
	 *      [r/o int] @a device
	 */
	virtual int getDevice() const = 0;


	/**
	 * Get the axis index.
	 * @par Scripted as
	 *      [r/o int] @a axis
	 */
	virtual int getAxis() const = 0;

	
	/**
	 * Get the axis position.
	 * @par Scripted as:
	 *      [r/o float] @a position
	 */
	virtual Float getPosition() const = 0;

};


// --- begin scripting interface ---

SCRIPT_METHOD(AxisEvent, getAxis, RETURNS_INT)
END_METHOD

SCRIPT_METHOD(AxisEvent, getDevice, RETURNS_INT)
END_METHOD

SCRIPT_METHOD(AxisEvent, getPosition, RETURNS_FLOAT)
END_METHOD

SCRIPT_METHOD(AxisEvent, initialize, RETURNS_BOOL)
	ARG_INT, ARG_INT, ARG_FLOAT
END_METHOD

EXPORTS(AxisEvent)
	INTERFACE(Event)
	RO_PROP_INT(AxisEvent, axis, getAxis)
	RO_PROP_INT(AxisEvent, device, getDevice)
	RO_PROP_FLOAT(AxisEvent, position, getPosition)
	METHOD(AxisEvent, initialize)
END_EXPORTS

// --- end scripting interface ---


EXIT_NAMESPACE;
#endif
