/**
 * @file event_loop.cpp
 *
 * @author  Jason Perkins (jason@379.com)
 * @date    Copyright (C) 2001 by 379, Inc.
 * @version $Id: event_loop.cpp,v 1.3 2002/05/24 19:05:31 jason379 Exp $
 */
#include "sdl_bindings.h"

#include <Core/core.h>
#include <Core/event_loop.h>
#include <System/axis_event.h>
#include <System/button_event.h>
#include <System/click_event.h>
#include <System/display_event.h>
#include <System/key_event.h>
#include <System/motion_event.h>
#include <System/time_event.h>
#include <Memory/memory.h>
using namespace f4;


struct SDLEventLoop : public EventLoop
{
	SDLEventLoop();
	virtual ~SDLEventLoop() { }

	virtual Event* getEvent();

private:
	bool m_prepared;
	Double m_lastTimeEvent;

	Ref<AxisEvent>    m_axisEvent;
	Ref<ButtonEvent>  m_buttonEvent;
	Ref<ClickEvent>   m_clickEvent;
	Ref<DisplayEvent> m_displayEvent;
	Ref<KeyEvent>     m_keyEvent;
	Ref<MotionEvent>  m_motionEvent;
	Ref<TimeEvent>    m_timeEvent;

	DECL_SERVICE;
};

IMPL_SERVICE(SDLEventLoop, EventLoop)


//---------------------------------------------------------------------------
// Local stuff
//---------------------------------------------------------------------------

DECLARE_SERVICE(AxisEvent)
DECLARE_SERVICE(ButtonEvent)
DECLARE_SERVICE(ClickEvent)
DECLARE_SERVICE(DisplayEvent);
DECLARE_SERVICE(KeyEvent);
DECLARE_SERVICE(MotionEvent);
DECLARE_SERVICE(TimeEvent);

static bool _prepareDisplay();



//---------------------------------------------------------------------------

SDLEventLoop::SDLEventLoop()
: m_prepared(false),
  m_lastTimeEvent(0)
{
	// Event objects can be reused
	m_axisEvent    = CREATEA_LOCAL(AxisEvent);
	m_buttonEvent  = CREATEA_LOCAL(ButtonEvent);
	m_clickEvent   = CREATEA_LOCAL(ClickEvent);
	m_displayEvent = CREATEA_LOCAL(DisplayEvent);
	m_keyEvent     = CREATEA_LOCAL(KeyEvent);
	m_motionEvent  = CREATEA_LOCAL(MotionEvent);
	m_timeEvent    = CREATEA_LOCAL(TimeEvent);
}

//---------------------------------------------------------------------------

Event* SDLEventLoop::getEvent()
{
	// If this is the first call, create and initialize the display window.
	// This will go away when I get proper video mode management
	if (!m_prepared)
	{
		_prepareDisplay();
		m_prepared = true;
	}

	// SDL doesn't provide a timestamp for events, even when the 
	// underlying OS does...too bad
	Double tstamp = static_cast<Double>(SDL_GetTicks()) / 1000.0;

	Event* e = 0;
	SDL_Event event;
	if (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_VIDEORESIZE:
			m_displayEvent->initialize(event.resize.w, event.resize.h);
			e = m_displayEvent;
			break;

		case SDL_QUIT:
			Core->quit();
			return 0;

		case SDL_KEYDOWN:
			m_keyEvent->initialize(_sdl_keyMap[event.key.keysym.sym], true, false);
			e = m_keyEvent;
			break;

		case SDL_KEYUP:
			m_keyEvent->initialize(_sdl_keyMap[event.key.keysym.sym], false, false);
			e = m_keyEvent;
			break;

		case SDL_MOUSEBUTTONDOWN:
			m_clickEvent->initialize(event.button.button-1, true, event.button.x, event.button.y);
			e = m_clickEvent;
			break;

		case SDL_MOUSEBUTTONUP:
			m_clickEvent->initialize(event.button.button-1, false, event.button.x, event.button.y);
			e = m_clickEvent;
			break;

		case SDL_MOUSEMOTION:
			m_motionEvent->initialize(event.motion.x, event.motion.y, event.motion.state);
			e = m_motionEvent;
			break;

		case SDL_JOYBUTTONDOWN:
			m_buttonEvent->initialize(event.jbutton.which, event.jbutton.button, true);
			e = m_buttonEvent;
			break;

		case SDL_JOYBUTTONUP:
			m_buttonEvent->initialize(event.jbutton.which, event.jbutton.button, false);
			e = m_buttonEvent;
			break;

		case SDL_JOYAXISMOTION:
			m_axisEvent->initialize(event.jaxis.which, event.jaxis.axis, (Float)event.jaxis.value / 32768.0f);
			e = m_axisEvent;
			break;

		default:
			return 0;
		}
	}
	else 
	{
		// If this is the first update, elapsed time is zero
		Float elapsed = (m_lastTimeEvent == 0) ? 0 : tstamp - m_lastTimeEvent;
		m_timeEvent->initialize(elapsed);
		m_lastTimeEvent = tstamp;
		e = m_timeEvent;
	}

	e->setTimestamp(tstamp);
	return e;
}

//---------------------------------------------------------------------------

/*
 * Create and initialize a display window. This should go away when I 
 * implement the mode selection methods.
 */
bool _prepareDisplay()
{
	// I just copied this from the SDL tutorial...

	int w = 640;
	int h = 480;
	
	const SDL_VideoInfo* vi = SDL_GetVideoInfo();
	int bpp = vi->vfmt->BitsPerPixel;

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE,   5);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 5);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE,  5);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	if (!SDL_SetVideoMode(w, h, bpp, SDL_OPENGL | SDL_RESIZABLE))
		Core->die(SDL_GetError());


	// Send the new display metrics to the other services
	DisplayEvent* e = CREATEA_LOCAL(DisplayEvent);
	e->initialize(640, 480);
	Core->queueEvent(e);

	return true;
}
