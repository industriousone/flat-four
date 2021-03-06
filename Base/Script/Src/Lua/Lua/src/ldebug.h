/*
** $Id: ldebug.h,v 1.2 2002/04/22 01:51:55 jason379 Exp $
** Auxiliary functions from Debug Interface module
** See Copyright Notice in lua.h
*/

#ifndef ldebug_h
#define ldebug_h


#include "lstate.h"
#include "luadebug.h"


void luaG_typeerror (lua_State *L, StkId o, const char *op);
void luaG_concaterror (lua_State *L, StkId p1, StkId p2);
void luaG_aritherror (lua_State *L, StkId p1, TObject *p2);
int luaG_getline (int *lineinfo, int pc, int refline, int *refi);
void luaG_ordererror (lua_State *L, const TObject *p1, const TObject *p2);
int luaG_checkcode (const Proto *pt);


#endif
