/**
 * @file action.h
 *
 * @author  Jason Perkins (jason@379.com)
 * @date    Copyright (C) 2001 by 379, Inc.
 * @version $Id: action.h,v 1.2 2002/04/22 01:51:56 jason379 Exp $
 */
#if !defined(_F4_STDLIB_ACTION_H_)
#define _F4_STDLIB_ACTION_H_

#include <Core/event.h>
ENTER_NAMESPACE(f4);


/**
 * The "action" event is sent whenever a mapped input event occurs
 * (see ActionMapper).
 *
 * @ingroup f4_stdlib
 */
struct Action : public Event
{

	/**
	 * Prepare a new action.
	 * @par Scripted as:
	 *      <I>bool initialize(string, float)</I>
	 */
	virtual bool initialize(const Char* actionName, Float value) = 0;


	/**
	 * Get the name of the action.
	 * @par Scripted as:
	 *      [r/o string] @a name
	 */
	virtual const Char* getName() const = 0;


	/**
	 * Get the value of the action inputs.
	 * @par Scripted as:
	 *      [r/o float] @a value
	 */
	virtual Float getValue() const = 0;

};


// --- begin scripting interface ---

SCRIPT_METHOD(Action, getName, RETURNS_STRING)
END_METHOD

SCRIPT_METHOD(Action, getValue, RETURNS_FLOAT)
END_METHOD

SCRIPT_METHOD(Action, initialize, RETURNS_BOOL)
	ARG_STRING, ARG_FLOAT
END_METHOD

EXPORTS(Action)
	INTERFACE(Event)
	RO_PROP_STRING(Action, name, getName)
	RO_PROP_FLOAT(Action, value, getValue)
	METHOD(Action, initialize)
END_EXPORTS

// --- end scripting interface ---


EXIT_NAMESPACE;
#endif
