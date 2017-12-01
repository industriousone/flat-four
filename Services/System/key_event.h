/**
 * @file key_event.h
 *
 * @author  Jason Perkins (jason@379.com)
 * @date    Copyright (C) 2001 by 379, Inc.
 * @version $Id: key_event.h,v 1.2 2002/04/22 01:51:56 jason379 Exp $
 */
#if !defined(_F4_SYSTEM_KEY_EVENT_H_)
#define _F4_SYSTEM_KEY_EVENT_H_

#include <Core/event.h>
ENTER_NAMESPACE(f4);


/**
 * Description of a keyboard event.
 * @ingroup f4_system
 */
struct KeyEvent : public Event
{
	enum {
		UNKNOWN,
		ESCAPE,
		UP, 
		DOWN, 
		LEFT, 
		RIGHT, 
		HOME, 
		END, 
		PGUP, 
		PGDN,
		INSERT, 
		DELETE,
		LSHIFT, 
		RSHIFT,
		LCTRL, 
		RCTRL,
		LALT, 
		RALT,
		TILDE,
		F1, 
		F2, 
		F3,	
		F4, 
		F5,	
		F6,	
		F7,	
		F8,	
		F9,	
		F10, 
		F11, 
		F12, 
		F13,	
		F14, 
		F15,
		ONE,
		TWO,
		THREE,
		FOUR,
		FIVE,
		SIX,
		SEVEN,
		EIGHT,
		NINE,
		ZERO,
		KP1, 
		KP2, 
		KP3, 
		KP4, 
		KP5, 
		KP6, 
		KP7, 
		KP8, 
		KP9, 
		KP0,
		MINUS,
		EQUALS,
		DECIMAL,
		DIVIDE,
		MULTIPLY,
		KPMINUS,
		KPPLUS,
		KPENTER,
		KPEQUALS,
		PERIOD,
		COMMA,
		BACKSPACE,
		CLEAR,
		TAB,
		PRINTSCREEN,
		CAPSLOCK, 
		NUMLOCK, 
		SCROLL, 
		PAUSE,
		LBRACKET, 
		RBRACKET,
		ENTER,
		SPACE,
		QUOTE,
		SLASH,
		BACKSLASH,
		SEMICOLON,
		LSYSTEM, 
		RSYSTEM, 
		MENU,
		A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
		NUM_KEYS
	};


	/**
	 * Initialize a new event.
	 * @par Scripted as:
	 *      [method]
	 */
	virtual bool initialize(int code, bool pressed, bool repeat) = 0;


	/**
	 * Retrieve the key code.
	 * @par Scripted as:
	 *      [r/o int] @a code
	 */
	virtual int getCode() const = 0;


	/**
	 * Retrieve the key description.
	 * @par Scripted as:
	 *      [r/o string] @a description
	 */
	virtual const Char* getDescription() const = 0;


	/**
	 * Returns true if the button is currently pressed.
	 * @par Scripted as:
	 *      [r/o bool] @a pressed
	 */
	virtual bool isPressed() const = 0;


	/**
	 * Is this an auto-repeat event? (Not yet implemented)
	 * @par Scripted as:
	 *      [r/o bool] @a repeat
	 */
	virtual bool isRepeat() const = 0;

};


// --- begin scripting interface ---

SCRIPT_METHOD(KeyEvent, getCode, RETURNS_INT)
END_METHOD

SCRIPT_METHOD(KeyEvent, getDescription, RETURNS_STRING)
END_METHOD

SCRIPT_METHOD(KeyEvent, initialize, RETURNS_BOOL)
	ARG_INT, ARG_BOOL, ARG_BOOL
END_METHOD

SCRIPT_METHOD(KeyEvent, isPressed, RETURNS_BOOL)
END_METHOD

SCRIPT_METHOD(KeyEvent, isRepeat, RETURNS_INT)
END_METHOD

EXPORTS(KeyEvent)
	INTERFACE(Event)
	RO_PROP_INT(KeyEvent, code, getCode)
	RO_PROP_BOOL(KeyEvent, pressed, isPressed)
	RO_PROP_BOOL(KeyEvent, repeat, isRepeat)
	RO_PROP_STRING(KeyEvent, description, getDescription)
	METHOD(KeyEvent, initialize)
	CONST_INT(KeyEvent, UNKNOWN)
	CONST_INT(KeyEvent, ESCAPE)
	CONST_INT(KeyEvent, UP)
	CONST_INT(KeyEvent, DOWN)
	CONST_INT(KeyEvent, LEFT)
	CONST_INT(KeyEvent, RIGHT)
	CONST_INT(KeyEvent, HOME)
	CONST_INT(KeyEvent, END)
	CONST_INT(KeyEvent, PGUP)
	CONST_INT(KeyEvent, PGDN)
	CONST_INT(KeyEvent, INSERT)
	CONST_INT(KeyEvent, DELETE)
	CONST_INT(KeyEvent, LSHIFT)
	CONST_INT(KeyEvent, RSHIFT)
	CONST_INT(KeyEvent, LCTRL)
	CONST_INT(KeyEvent, RCTRL)
	CONST_INT(KeyEvent, LALT)
	CONST_INT(KeyEvent, RALT)
	CONST_INT(KeyEvent, TILDE)
	CONST_INT(KeyEvent, F1) 
	CONST_INT(KeyEvent, F2) 
	CONST_INT(KeyEvent, F3)	
	CONST_INT(KeyEvent, F4) 
	CONST_INT(KeyEvent, F5)	
	CONST_INT(KeyEvent, F6)	
	CONST_INT(KeyEvent, F7)	
	CONST_INT(KeyEvent, F8)	
	CONST_INT(KeyEvent, F9)	
	CONST_INT(KeyEvent, F10) 
	CONST_INT(KeyEvent, F11) 
	CONST_INT(KeyEvent, F12) 
	CONST_INT(KeyEvent, F13)	
	CONST_INT(KeyEvent, F14) 
	CONST_INT(KeyEvent, F15)
	CONST_INT(KeyEvent, ONE) 
	CONST_INT(KeyEvent, TWO) 
	CONST_INT(KeyEvent, THREE) 
	CONST_INT(KeyEvent, FOUR) 
	CONST_INT(KeyEvent, FIVE) 
	CONST_INT(KeyEvent, SIX) 
	CONST_INT(KeyEvent, SEVEN) 
	CONST_INT(KeyEvent, EIGHT) 
	CONST_INT(KeyEvent, NINE) 
	CONST_INT(KeyEvent, ZERO)
	CONST_INT(KeyEvent, KP1) 
	CONST_INT(KeyEvent, KP2) 
	CONST_INT(KeyEvent, KP3) 
	CONST_INT(KeyEvent, KP4) 
	CONST_INT(KeyEvent, KP5) 
	CONST_INT(KeyEvent, KP6) 
	CONST_INT(KeyEvent, KP7) 
	CONST_INT(KeyEvent, KP8) 
	CONST_INT(KeyEvent, KP9) 
	CONST_INT(KeyEvent, KP0)
	CONST_INT(KeyEvent, MINUS)
	CONST_INT(KeyEvent, EQUALS)
	CONST_INT(KeyEvent, DECIMAL)
	CONST_INT(KeyEvent, DIVIDE)
	CONST_INT(KeyEvent, MULTIPLY)
	CONST_INT(KeyEvent, KPMINUS)
	CONST_INT(KeyEvent, KPPLUS)
	CONST_INT(KeyEvent, KPENTER)
	CONST_INT(KeyEvent, KPEQUALS)
	CONST_INT(KeyEvent, PERIOD)
	CONST_INT(KeyEvent, COMMA)
	CONST_INT(KeyEvent, BACKSPACE)
	CONST_INT(KeyEvent, CLEAR)
	CONST_INT(KeyEvent, TAB)
	CONST_INT(KeyEvent, PRINTSCREEN)
	CONST_INT(KeyEvent, CAPSLOCK)
	CONST_INT(KeyEvent, NUMLOCK)
	CONST_INT(KeyEvent, SCROLL)
	CONST_INT(KeyEvent, PAUSE)
	CONST_INT(KeyEvent, LBRACKET)
	CONST_INT(KeyEvent, RBRACKET)
	CONST_INT(KeyEvent, ENTER)
	CONST_INT(KeyEvent, SPACE)
	CONST_INT(KeyEvent, QUOTE)
	CONST_INT(KeyEvent, SLASH)
	CONST_INT(KeyEvent, BACKSLASH)
	CONST_INT(KeyEvent, SEMICOLON)
	CONST_INT(KeyEvent, LSYSTEM)
	CONST_INT(KeyEvent, RSYSTEM)
	CONST_INT(KeyEvent, MENU)
	CONST_INT(KeyEvent, A) 
	CONST_INT(KeyEvent, B) 
	CONST_INT(KeyEvent, C) 
	CONST_INT(KeyEvent, D) 
	CONST_INT(KeyEvent, E) 
	CONST_INT(KeyEvent, F) 
	CONST_INT(KeyEvent, G) 
	CONST_INT(KeyEvent, H) 
	CONST_INT(KeyEvent, I) 
	CONST_INT(KeyEvent, J) 
	CONST_INT(KeyEvent, K) 
	CONST_INT(KeyEvent, L) 
	CONST_INT(KeyEvent, M) 
	CONST_INT(KeyEvent, N) 
	CONST_INT(KeyEvent, O) 
	CONST_INT(KeyEvent, P) 
	CONST_INT(KeyEvent, Q) 
	CONST_INT(KeyEvent, R) 
	CONST_INT(KeyEvent, S) 
	CONST_INT(KeyEvent, T) 
	CONST_INT(KeyEvent, U) 
	CONST_INT(KeyEvent, V) 
	CONST_INT(KeyEvent, W) 
	CONST_INT(KeyEvent, X) 
	CONST_INT(KeyEvent, Y) 
	CONST_INT(KeyEvent, Z)
END_EXPORTS

// --- end scripting interface ---


EXIT_NAMESPACE;
#endif
