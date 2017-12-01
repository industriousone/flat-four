/*
** $Id: lualib.h,v 1.2 2002/04/22 01:51:55 jason379 Exp $
** Lua standard libraries
** See Copyright Notice in lua.h
*/


#ifndef lualib_h
#define lualib_h

#include "lua.h"


#ifndef LUALIB_API
#define LUALIB_API	extern
#endif


#define LUA_ALERT               "_ALERT"

LUALIB_API int lua_baselibopen (lua_State *L);
LUALIB_API int lua_iolibopen (lua_State *L);
LUALIB_API int lua_strlibopen (lua_State *L);
LUALIB_API int lua_mathlibopen (lua_State *L);
LUALIB_API int lua_dblibopen (lua_State *L);


#endif
