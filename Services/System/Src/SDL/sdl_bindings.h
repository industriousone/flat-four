/**
 * @file sdl_bindings.h
 *
 * @author  Jason Perkins (jason@379.com)
 * @date    Copyright (C) 2001 by 379, Inc.
 * @version $Id: sdl_bindings.h,v 1.2 2002/04/22 01:51:56 jason379 Exp $
 */
#if !defined(_F4_SYSTEM_SDL_BINDINGS_H_)
#define _F4_SYSTEM_SDL_BINDINGS_H_

// Don't know why but Win32 distro is different from Linux
#if defined(WIN32)
#include <SDL.h>
#define WINGDIAPI __declspec(dllimport)
#define APIENTRY  __stdcall
#else
#include <SDL/SDL.h>
#endif


// Have to use different names or GNU linker will get confused
extern char*          (*_SDL_GetError)();
extern Uint32         (*_SDL_GetTicks)();
extern SDL_VideoInfo* (*_SDL_GetVideoInfo)();
extern int            (*_SDL_GL_SetAttribute)(int, int);
extern void           (*_SDL_GL_SwapBuffers)();
extern int            (*_SDL_Init)(Uint32);
extern int            (*_SDL_JoystickEventState)(int);
extern char*          (*_SDL_JoystickName)(int);
extern void*          (*_SDL_JoystickOpen)(int);
extern int            (*_SDL_NumJoysticks)();
extern int            (*_SDL_PollEvent)(SDL_Event*);
extern void           (*_SDL_Quit)();
extern void           (*_SDL_WM_SetCaption)(const char*, const char*);
extern SDL_Surface*   (*_SDL_SetVideoMode)(int, int, int, Uint32);

#if !defined(_SDL_NOMAP_)
#define SDL_GetError           _SDL_GetError
#define SDL_GetTicks           _SDL_GetTicks
#define SDL_GetVideoInfo       _SDL_GetVideoInfo
#define SDL_GL_SetAttribute    _SDL_GL_SetAttribute
#define SDL_GL_SwapBuffers     _SDL_GL_SwapBuffers
#define SDL_Init               _SDL_Init
#define SDL_JoystickEventState _SDL_JoystickEventState
#define SDL_JoystickName       _SDL_JoystickName
#define SDL_JoystickOpen       _SDL_JoystickOpen
#define SDL_NumJoysticks       _SDL_NumJoysticks
#define SDL_PollEvent          _SDL_PollEvent
#define SDL_Quit               _SDL_Quit
#define SDL_WM_SetCaption      _SDL_WM_SetCaption
#define SDL_SetVideoMode       _SDL_SetVideoMode
#endif


//---------------------------------------------------------------------------

extern Uint16 _sdl_keyMap[];

bool _loadSDL();
void _unloadSDL();

//---------------------------------------------------------------------------

#endif
