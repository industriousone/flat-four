/**
 * @file system.cpp
 *
 * @author  Jason Perkins (jason@379.com)
 * @date    Copyright (C) 2001 by 379, Inc.
 * @version $Id: system.cpp,v 1.3 2002/05/18 10:57:29 jason379 Exp $
 *
 * @todo Report SDL version numbers from DLL, not constants from header.
 *
 */
#include "sdl_bindings.h"

#include <Core/core.h>
#include <System/system.h>
#include <System/key_event.h>
#include <Memory/memory.h>
using namespace f4;

extern const Char* _keyNames[];


struct SystemImpl : public SystemService
{
	SystemImpl();
	virtual ~SystemImpl();

	virtual void   setWindowTitle(const Char* title);
	virtual int    getDisplayWidth() const;
	virtual int    getDisplayHeight() const;
	virtual Float  getDisplayAspect() const;
	virtual void   swapBuffers();
	virtual Double getTimer();
	virtual int    getKeyCode(const Char* name) const;

private:
	int  m_width, m_height;
	DECL_SERVICE;
};

IMPL_SINGLETON_SERVICE(SystemImpl, SystemService)



//---------------------------------------------------------------------------
// Local Data
//---------------------------------------------------------------------------

DECLARE_SERVICE(EventLoop)



//---------------------------------------------------------------------------
// Message text
//---------------------------------------------------------------------------

static const Char* START_MSG     = "Initializing System service\n";
static const Char* STOP_MSG      = "Closing System service\n";
static const Char* CREDITS       = "  SDL %d.%d.%d - Copyright (C) 1997-2001 Sam Lantinga\n";
static const Char* BAD_SDL       = "This application requires SDL v%d.%d. Please visit http://www.libsdl.org/.";
static const Char* DEFAULT_TITLE = "F4 Engine";



//---------------------------------------------------------------------------

SystemImpl::SystemImpl()
: m_width(640), m_height(480)
{
	Core->print(START_MSG);
	Core->print(CREDITS, SDL_MAJOR_VERSION, SDL_MINOR_VERSION, SDL_PATCHLEVEL);

	
	// It's always possible that SDL isn't installed, or it's an incompatible
	// version...the only way to detect this and recover gracefully is to
	// dynamically load and bind SDL at runtime
	if (!_loadSDL())
	{
		char errMsg[512];
		sprintf(errMsg, BAD_SDL, SDL_MAJOR_VERSION, SDL_MINOR_VERSION);
		Core->die(errMsg);
	}

	// Set up a new SDL session
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) < 0)
		Core->die(SDL_GetError());
	SDL_WM_SetCaption(DEFAULT_TITLE, 0);


	// Find and initialize all game controllers
	Core->print("  Found %d game controllers\n", SDL_NumJoysticks());
	SDL_JoystickEventState(SDL_ENABLE);
	for (int ctrl = 0; ctrl < SDL_NumJoysticks(); ++ctrl)
	{
		Core->print("  - %s...", SDL_JoystickName(ctrl));
		if (SDL_JoystickOpen(ctrl))
			Core->print("ok\n");
		else
			Core->print("failed\n");
	} 


	// Create an event loop and register it with CoreService so the other
	// engine services will be able to use it
	EventLoop* el = (EventLoop*)CREATEA_LOCAL(EventLoop);
	Core->registerEventLoop(el);


	// Initialize the random number generator
	srand(SDL_GetTicks());
}

//---------------------------------------------------------------------------

SystemImpl::~SystemImpl()
{
	Core->print(STOP_MSG);

	SDL_Quit();
	_unloadSDL();
}

//---------------------------------------------------------------------------

void SystemImpl::setWindowTitle(const Char* title)
{
	SDL_WM_SetCaption(title, 0);
}

//---------------------------------------------------------------------------

int SystemImpl::getDisplayWidth() const
{
	return m_width;
}

//---------------------------------------------------------------------------

int SystemImpl::getDisplayHeight() const
{
	return m_height;
}

//---------------------------------------------------------------------------

Float SystemImpl::getDisplayAspect() const
{
	return (Float)m_width / (Float)m_height;
}

//---------------------------------------------------------------------------

void SystemImpl::swapBuffers()
{
	SDL_GL_SwapBuffers();
}

//---------------------------------------------------------------------------

Double SystemImpl::getTimer()
{
	return static_cast<Double>(SDL_GetTicks()) / 1000.0;
}

//---------------------------------------------------------------------------

int SystemImpl::getKeyCode(const Char* name) const
{
	for (int i = 0; i < KeyEvent::NUM_KEYS; ++i)
	{
		if (strcmp(_keyNames[i], name) == 0)
			return i;
	}

	return 0;
}
