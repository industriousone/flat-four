/**
 * @file system.h
 *
 * @author  Jason Perkins (jason@379.com)
 * @date    Copyright (C) 2001 by 379, Inc.
 * @version $Id: system.h,v 1.3 2002/05/18 10:57:29 jason379 Exp $
 */
#if !defined(_F4_SYSTEM_H_)
#define _F4_SYSTEM_H_

ENTER_NAMESPACE(f4);


/**
 * The "system" service provides an abstracted interface to the underlying
 * hardware/OS platform. It allows the application to interact with the game
 * window, receive user input, read the system timer, and similar tasks.
 * @ingroup f4_system
 */
struct SystemService : public Service
{

	/**
	 * Set the title shown on the display window.
	 */
	virtual void setWindowTitle(const Char* title) = 0;


	/**
	 * Get the width of the display, in pixels.
	 */
	virtual int getDisplayWidth() const = 0;

	
	/**
	 * Get the height of the display, in pixels.
	 */
	virtual int getDisplayHeight() const = 0;


	/**
	 * Get the aspect ratio of the display, calculated as width/height.
	 */
	virtual Float getDisplayAspect() const = 0;


	/**
	 * Swap the rendering buffers. Ideally this method would be in the render
	 * service, but the OpenGL swap function is system specific.
	 */
	virtual void swapBuffers() = 0;


	/**
	 * Read the system timer. Returns the number of seconds since the system
	 * was started.
	 */
	virtual Double getTimer() = 0;


	/**
	 * Given a key name, returns the associated key code. Returns zero if the
	 * name is not recognized.
	 */
	virtual int getKeyCode(const Char* name) const = 0;

};


// --- begin scripting interface ---

SCRIPT_METHOD(SystemService, getDisplayAspect, RETURNS_FLOAT)
END_METHOD

SCRIPT_METHOD(SystemService, getDisplayHeight, RETURNS_INT)
END_METHOD

SCRIPT_METHOD(SystemService, getDisplayWidth, RETURNS_INT)
END_METHOD

SCRIPT_METHOD(SystemService, getKeyCode, RETURNS_INT)
	ARG_STRING
END_METHOD

SCRIPT_METHOD(SystemService, getTimer, RETURNS_DOUBLE)
END_METHOD

SCRIPT_METHOD(SystemService, setWindowTitle, RETURNS_VOID)
	ARG_STRING
END_METHOD

SCRIPT_METHOD(SystemService, swapBuffers, RETURNS_VOID)
END_METHOD

EXPORTS(SystemService)
	METHOD(SystemService, getDisplayAspect)
	METHOD(SystemService, getDisplayHeight)
	METHOD(SystemService, getDisplayWidth)
	METHOD(SystemService, getKeyCode)
	METHOD(SystemService, getTimer)
	METHOD(SystemService, setWindowTitle)
	METHOD(SystemService, swapBuffers)
END_EXPORTS

// --- end scripting interface ---


EXIT_NAMESPACE;
#endif
