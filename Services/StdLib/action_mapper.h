/**
 * @file action_mapper.h
 *
 * @author  Jason Perkins (jason@379.com)
 * @date    Copyright (C) 2001 by 379, Inc.
 * @version $Id: action_mapper.h,v 1.2 2002/04/22 01:51:56 jason379 Exp $
 */
#if !defined(_F4_STDLIB_ACTION_MAP_H_)
#define _F4_STDLIB_ACTION_MAP_H_

ENTER_NAMESPACE(f4);


/**
 * Convert between physical input events ("the [J] key was pressed") and
 * logical game actions ("jump"). 
 *
 * Start by creating new logical game objects -- toggles, triggers, and
 * axes -- then map one or more physical input objects to these logical
 * objects.
 *
 * @ingroup f4_stdlib
 */
struct ActionMapper : public Service
{

	/**
	 * A "trigger" fires an event when it is pressed (but not when it is
	 * released).
	 *
	 * @param name      A unique identifier for the object.
	 * @param pressAt   Used to convert from an analog axis to a digital 
	 *                  button. This is the axis position at which the state
	 *                  will switch from "released" to "pressed".
	 * @param relAt     Used to convert from an analog axis to a digital 
	 *                  button. This is the axis position at which the state
	 *                  will switch from "pressed" to "released".
	 *
	 * @par Scripted as:
	 *      <I>bool createTrigger(string)</I>
	 */
	virtual bool createTrigger(const Char* name, Float pressAt, Float relAt) = 0;


	/**
	 * A "toggle" fires an event when it is pressed and another when it is
	 * released.
	 *
	 * @param name      A unique identifier for the object.
	 * @param pressAt   Used to convert from an analog axis to a digital 
	 *                  button. This is the axis position at which the state
	 *                  will switch from "released" to "pressed".
	 * @param relAt     Used to convert from an analog axis to a digital 
	 *                  button. This is the axis position at which the state
	 *                  will switch from "pressed" to "released".
	 *
	 * @par Scripted as:
	 *      <I>bool createToggle(string)</I>
	 */
	virtual bool createToggle(const Char* name, Float pressAt, Float relAt) = 0;


	/**
	 * An "axis" has a value which ranges from 0.0 to 1.0, and fires an event
	 * whenever it's value changes. This corresponds to one half of a
	 * physical input axis.
	 *
	 * @param name      A unique identifier for the object.
	 * @param rampUp    Used to convert from a digital button to an analog
	 *                  axis value. This controls how quickly the axis will
	 *                  move from 0.0 to 1.0 when the button is pressed, in
	 *                  seconds.
	 * @param rampDn    Used to convert from a digital button to an analog
	 *                  axis value. This controls how quickly the axis will
	 *                  move from 1.0 to 0.0 when the button is released, in
	 *                  seconds.
	 *
	 * @par Scripted as:
	 *      <I>bool createAxis(string)</I>
	 */
	virtual bool createAxis(const Char* name, Float rampUp, Float rampDn) = 0;


	/**
	 * Binds a physical input object, such as a keyboard button, to a logical
	 * object like a trigger.
	 *
	 * @param physical   A description of the physics input object to be bound.
	 *                   For a keyboard button, this is the name of the button
	 *                   (see @c F4/Services/System/Src/key_event.cpp for a 
	 *                   complete list). For a game controller, the format is
	 *                   "ctrl0.+axis0" for the positive half of the first axis on
	 *                   the first controller, or "ctrl0.btn0" for the first
	 *                   button. Mouse buttons may be mapped ("mouse.btn0") but
	 *                   currently not the mouse axes.
	 *
	 * @param logical    The name of the logical input object, specified in the
	 *                   call to createTrigger(), etc. If this parameter is 
	 *                   @c NULL, the binding is removed.
	 *
	 * @return True if successful, false if @a physical is improperly formatted
	 *         or does not refer to an actual physical object.
	 *
	 * @par Scripted as:
	 *      <I>bind(string, string)</I>
	 */
	virtual bool bind(const Char* physical, const Char* logical) = 0;


	/**
	 * Translate a physical input event to a logical action. If a mapping
	 * exists, a new Action event will be added to the event queue. This is
	 * called automatically for all unhandled user input events.
	 */
	virtual bool translate(Event* event) = 0;

};



// --- begin scripting interface ---

SCRIPT_METHOD(ActionMapper, bind, RETURNS_BOOL)
	ARG_STRING, ARG_STRING
END_METHOD

SCRIPT_METHOD(ActionMapper, createAxis, RETURNS_BOOL)
	ARG_STRING, ARG_FLOAT, ARG_FLOAT
END_METHOD

SCRIPT_METHOD(ActionMapper, createToggle, RETURNS_BOOL)
	ARG_STRING, ARG_FLOAT, ARG_FLOAT
END_METHOD

SCRIPT_METHOD(ActionMapper, createTrigger, RETURNS_BOOL)
	ARG_STRING, ARG_FLOAT, ARG_FLOAT
END_METHOD

SCRIPT_METHOD(ActionMapper, translate, RETURNS_BOOL)
	ARG_SERVICE(Event)
END_METHOD

EXPORTS(ActionMapper)
	METHOD(ActionMapper, bind)
	METHOD(ActionMapper, createAxis)
	METHOD(ActionMapper, createToggle)
	METHOD(ActionMapper, createTrigger)
	METHOD(ActionMapper, translate)
END_EXPORTS

// --- end scripting interface ---


EXIT_NAMESPACE;
#endif
