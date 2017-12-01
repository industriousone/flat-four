/**
 * @file refs.cpp
 *
 * @author  Jason Perkins (jason@379.com)
 * @date    Copyright (C) 2001 by 379, Inc.
 * @version $Id: refs.cpp,v 1.2 2002/04/22 01:51:55 jason379 Exp $
 */
extern "C"
{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

#include <Core/core.h>
#include <Script/script.h>
#include "extensions.h"
#include <Memory/memory.h>
using namespace f4;


//---------------------------------------------------------------------------
// Local data
//---------------------------------------------------------------------------

static const int WEAK_TABLE_INDEX   = 1;
static const int STRONG_TABLE_INDEX = 2; 
 
static int _makeRef(lua_State* L, int table);




//---------------------------------------------------------------------------

void _initRefs(lua_State* L)
{
	// Create tables to store both weak and strong references. Put both
	// in the Lua registry
	lua_newtable(L);
	lua_setweakmode(L, LUA_WEAK_VALUE);
	lua_rawseti(L, LUA_REGISTRYINDEX, WEAK_TABLE_INDEX);

	lua_newtable(L);
	lua_rawseti(L, LUA_REGISTRYINDEX, STRONG_TABLE_INDEX);
}

//---------------------------------------------------------------------------

int _makeStrongRef(lua_State* L)
{
	int ref = _makeRef(L, STRONG_TABLE_INDEX);
	return ref;
}

//---------------------------------------------------------------------------

int _makeWeakRef(lua_State* L)
{
	int ref = -(_makeRef(L, WEAK_TABLE_INDEX));
	return ref;
}

//---------------------------------------------------------------------------

void _dropRef(lua_State* L, int ref)
{
	if (ref < 0)
	{
		lua_rawgeti(L, LUA_REGISTRYINDEX, WEAK_TABLE_INDEX);
		ref = -ref;
	}
	else
	{
		lua_rawgeti(L, LUA_REGISTRYINDEX, STRONG_TABLE_INDEX);
	}

	// If Lua is being shut down, the ref tables may have been collected
	if (lua_isnil(L, -1)) return;

	// Add this index to the free chain. Writing the next free index into
	// the newly freed spot cause whatever object was there to be released
	lua_rawgeti(L, -1, 0);
	lua_rawseti(L, -2, ref);

	// Store the new free index at index zero
	lua_pushnumber(L, ref);
	lua_rawseti(L, -2, 0);

	lua_pop(L, 1);
}

//---------------------------------------------------------------------------

void _loadRef(lua_State* L, int ref)
{
	if (ref < 0)
	{
		lua_rawgeti(L, LUA_REGISTRYINDEX, WEAK_TABLE_INDEX);
		ref = -ref;
	}
	else
	{
		lua_rawgeti(L, LUA_REGISTRYINDEX, STRONG_TABLE_INDEX);
	}

	lua_rawgeti(L, -1, ref);
	lua_remove(L, -2);
}

//---------------------------------------------------------------------------

int _makeRef(lua_State* L, int table)
{
	lua_rawgeti(L, LUA_REGISTRYINDEX, table);

	// Find the next free index in the table
	int ref;
	lua_rawgeti(L, -1, 0);
	if (lua_isnil(L, -1))
	{
		ref = lua_getn(L, -2) + 1;
	}
	else
	{
		ref = static_cast<int>(lua_tonumber(L, -1));
		lua_rawgeti(L, -2, ref);
		lua_rawseti(L, -3, 0);
	}

	lua_pop(L, 1);

	// Store the requested object at the index
	lua_pushvalue(L, -2);
	lua_rawseti(L, -2, ref);
	lua_pop(L, 2);

	return ref;
}
