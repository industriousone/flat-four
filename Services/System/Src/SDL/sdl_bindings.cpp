/**
 * @file sdl_bindings.cpp
 *
 * @author  Jason Perkins (jason@379.com)
 * @date    Copyright (C) 2001 by 379, Inc.
 * @version $Id: sdl_bindings.cpp,v 1.3 2002/05/24 02:15:19 jason379 Exp $
 */
#define _SDL_NOMAP_
#include "sdl_bindings.h"

#include <Core/core.h>
#include <System/key_event.h>
#include <Platform/library.h>
using namespace f4;


//---------------------------------------------------------------------------
// Local data
//---------------------------------------------------------------------------

static SharedLibrary _libSDL = 0;

char*          (*_SDL_GetError)();
Uint32         (*_SDL_GetTicks)();
SDL_VideoInfo* (*_SDL_GetVideoInfo)();
int            (*_SDL_GL_SetAttribute)(int, int);
void           (*_SDL_GL_SwapBuffers)();
int            (*_SDL_Init)(Uint32);
int            (*_SDL_JoystickEventState)(int);
char*          (*_SDL_JoystickName)(int);
void*          (*_SDL_JoystickOpen)(int);
int            (*_SDL_NumJoysticks)();
int            (*_SDL_PollEvent)(SDL_Event*);
void           (*_SDL_Quit)();
void           (*_SDL_WM_SetCaption)(const char*, const char*);
SDL_Surface*   (*_SDL_SetVideoMode)(int, int, int, Uint32);



//---------------------------------------------------------------------------

#define BIND(S)   if (!bindSymbol((void**)&_##S, _libSDL, #S)) return false

#if defined(F4_ISLINUX)
#include <dlfcn.h>
static void* _pthread = 0;
#endif

bool _loadSDL()
{
#if defined(F4_ISLINUX)
	// Nasty configuration on Linux
	_pthread = dlopen("libpthread.so", RTLD_GLOBAL | RTLD_NOW);
#endif

	_libSDL = openLibrary("SDL");
	if (!_libSDL) return false;

	BIND(SDL_GetError);
	BIND(SDL_GetTicks);
	BIND(SDL_GetVideoInfo);
	BIND(SDL_GL_SetAttribute);
	BIND(SDL_GL_SwapBuffers);
	BIND(SDL_Init);
	BIND(SDL_JoystickEventState);
	BIND(SDL_JoystickName);
	BIND(SDL_JoystickOpen);
	BIND(SDL_NumJoysticks);
	BIND(SDL_PollEvent);
	BIND(SDL_Quit);
	BIND(SDL_WM_SetCaption);
	BIND(SDL_SetVideoMode);

	return true;
}

//---------------------------------------------------------------------------

void _unloadSDL()
{
	if (_libSDL)
	{
		closeLibrary(_libSDL);
		_libSDL = 0;
	}
	
#if defined(F4_ISLINUX)
	if (_pthread) dlclose(_pthread);
#endif
}

//---------------------------------------------------------------------------

Uint16 _sdl_keyMap[] = 
{
	KeyEvent::UNKNOWN,
	0,              // 1
	0,              // 2
	0,              // 3
	0,              // 4
	0,              // 5
	0,              // 6
	0,              // 7
	KeyEvent::BACKSPACE,
	KeyEvent::TAB,
	0,              // 10
	0,              // 11
	KeyEvent::CLEAR,
	KeyEvent::ENTER,
	0,              // 14
	0,              // 15
	0,              // 16
	0,              // 17
	0,              // 18
	KeyEvent::PAUSE,
	0,              // 20
	0,              // 21
	0,              // 22
	0,              // 23
	0,              // 24
	0,              // 25
	0,              // 26
	KeyEvent::ESCAPE,
	0,              // 28
	0,              // 29
	0,              // 30
	0,              // 31
	KeyEvent::SPACE,
	0,              // 33
	0,              // 34
	0,              // 35
	0,              // 36
	0,              // 37
	0,              // 38
	KeyEvent::QUOTE,
	0,              // 40
	0,              // 41
	0,              // 42
	0,              // 43
	KeyEvent::COMMA,
	KeyEvent::MINUS,
	KeyEvent::PERIOD,
	KeyEvent::SLASH,
	KeyEvent::ZERO,
	KeyEvent::ONE,
	KeyEvent::TWO,
	KeyEvent::THREE,
	KeyEvent::FOUR,
	KeyEvent::FIVE,
	KeyEvent::SIX,
	KeyEvent::SEVEN,
	KeyEvent::EIGHT,
	KeyEvent::NINE,
	0,              // 59
	KeyEvent::SEMICOLON,
	0,              // 60
	KeyEvent::EQUALS,
	0,              // 62
	0,              // 63
	0,              // 64
	0,              // 65
	0,              // 66
	0,              // 67
	0,              // 68
	0,              // 69
	0,              // 70
	0,              // 71
	0,              // 72
	0,              // 73
	0,              // 74
	0,              // 75
	0,              // 76
	0,              // 77
	0,              // 78
	0,              // 79
	0,              // 80
	0,              // 81
	0,              // 82
	0,              // 83
	0,              // 84
	0,              // 85
	0,              // 86
	0,              // 87
	0,              // 88
	0,              // 89
	0,              // 90
	KeyEvent::LBRACKET,
	KeyEvent::BACKSLASH,
	KeyEvent::RBRACKET,
	0,              // 94
	0,              // 95
	KeyEvent::TILDE,
	KeyEvent::A,
	KeyEvent::B,
	KeyEvent::C,
	KeyEvent::D,
	KeyEvent::E,
	KeyEvent::F,
	KeyEvent::G,
	KeyEvent::H,
	KeyEvent::I,
	KeyEvent::J,
	KeyEvent::K,
	KeyEvent::L,
	KeyEvent::M,
	KeyEvent::N,
	KeyEvent::O,
	KeyEvent::P,
	KeyEvent::Q,
	KeyEvent::R,
	KeyEvent::S,
	KeyEvent::T,
	KeyEvent::U,
	KeyEvent::V,
	KeyEvent::W,
	KeyEvent::X,
	KeyEvent::Y,
	KeyEvent::Z,
	0,              // 123
	0,              // 124
	0,              // 125
	0,              // 126
	KeyEvent::DELETE,
	0,              // 128
	0,              // 129
	0,              // 130
	0,              // 131
	0,              // 132
	0,              // 133
	0,              // 134
	0,              // 135
	0,              // 136
	0,              // 137
	0,              // 138
	0,              // 139
	0,              // 140
	0,              // 141
	0,              // 142
	0,              // 143
	0,              // 144
	0,              // 145
	0,              // 146
	0,              // 147
	0,              // 148
	0,              // 149
	0,              // 150
	0,              // 151
	0,              // 152
	0,              // 153
	0,              // 154
	0,              // 155
	0,              // 156
	0,              // 157
	0,              // 158
	0,              // 159
	0,              // 160
	0,              // 161
	0,              // 162
	0,              // 163
	0,              // 164
	0,              // 165
	0,              // 166
	0,              // 167
	0,              // 168
	0,              // 169
	0,              // 170
	0,              // 171
	0,              // 172
	0,              // 173
	0,              // 174
	0,              // 175
	0,              // 176
	0,              // 177
	0,              // 178
	0,              // 179
	0,              // 180
	0,              // 181
	0,              // 182
	0,              // 183
	0,              // 184
	0,              // 185
	0,              // 186
	0,              // 187
	0,              // 188
	0,              // 189
	0,              // 190
	0,              // 191
	0,              // 192
	0,              // 193
	0,              // 194
	0,              // 195
	0,              // 196
	0,              // 197
	0,              // 198
	0,              // 199
	0,              // 200
	0,              // 201
	0,              // 202
	0,              // 203
	0,              // 204
	0,              // 205
	0,              // 206
	0,              // 207
	0,              // 208
	0,              // 209
	0,              // 210
	0,              // 211
	0,              // 212
	0,              // 213
	0,              // 214
	0,              // 215
	0,              // 216
	0,              // 217
	0,              // 218
	0,              // 219
	0,              // 220
	0,              // 221
	0,              // 222
	0,              // 223
	0,              // 224
	0,              // 225
	0,              // 226
	0,              // 227
	0,              // 228
	0,              // 229
	0,              // 230
	0,              // 231
	0,              // 232
	0,              // 233
	0,              // 234
	0,              // 235
	0,              // 236
	0,              // 237
	0,              // 238
	0,              // 239
	0,              // 240
	0,              // 241
	0,              // 242
	0,              // 243
	0,              // 244
	0,              // 245
	0,              // 246
	0,              // 247
	0,              // 248
	0,              // 249
	0,              // 250
	0,              // 251
	0,              // 252
	0,              // 253
	0,              // 254
	0,              // 255
	KeyEvent::KP0,
	KeyEvent::KP1,
	KeyEvent::KP2,
	KeyEvent::KP3,
	KeyEvent::KP4,
	KeyEvent::KP5,
	KeyEvent::KP6,
	KeyEvent::KP7,
	KeyEvent::KP8,
	KeyEvent::KP9,
	KeyEvent::DECIMAL,
	KeyEvent::DIVIDE,
	KeyEvent::MULTIPLY,
	KeyEvent::KPMINUS,
	KeyEvent::KPPLUS,
	KeyEvent::KPENTER,
	KeyEvent::KPEQUALS,
	KeyEvent::UP,
	KeyEvent::DOWN,
	KeyEvent::RIGHT,
	KeyEvent::LEFT,
	KeyEvent::INSERT,
	KeyEvent::HOME,
	KeyEvent::END,
	KeyEvent::PGUP,
	KeyEvent::PGDN,
	KeyEvent::F1,
	KeyEvent::F2,
	KeyEvent::F3,
	KeyEvent::F4,
	KeyEvent::F5,
	KeyEvent::F6,
	KeyEvent::F7,
	KeyEvent::F8,
	KeyEvent::F9,
	KeyEvent::F10,
	KeyEvent::F11,
	KeyEvent::F12,
	KeyEvent::F13,
	KeyEvent::F14,
	KeyEvent::F15,
	0,              // 297
	0,              // 298
	0,              // 299
	KeyEvent::NUMLOCK,
	KeyEvent::CAPSLOCK,
	KeyEvent::SCROLL,
	KeyEvent::RSHIFT,
	KeyEvent::LSHIFT,
	KeyEvent::RCTRL,
	KeyEvent::LCTRL,
	KeyEvent::RALT,
	KeyEvent::LALT,
	0,              // 309
	0,              // 310
	KeyEvent::LSYSTEM,
	KeyEvent::RSYSTEM,
	0,              // 313
	0,              // 314
	0,              // 315
	0,              // 316
	KeyEvent::PRINTSCREEN,
	0,              // 318
	KeyEvent::MENU,
	0,              // 320
	0,              // 321
	0,              // 322
	0,              // 323
	0,              // 324
};

