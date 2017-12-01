/**
 * @file quaternion.cpp
 *
 * @author  Jason Perkins (jason@379.com)
 * @date    Copyright (C) 2001 by 379, Inc.
 * @version $Id: quaternion.cpp,v 1.3 2002/04/22 20:16:27 jason379 Exp $
 */
extern "C"
{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

#include <Core/core.h>
#include <Script/script.h>
#include <Math/math.h>
#include "extensions.h"
#include <Memory/memory.h>
using namespace f4;


//---------------------------------------------------------------------------
// Local stuff
//---------------------------------------------------------------------------

static int _quat_metatable;

static int _quat_setRotation(lua_State* L);
static int _quat_mul(lua_State* L);


//---------------------------------------------------------------------------
// Message text
//---------------------------------------------------------------------------

static const Char* BAD_MULTIPLIER = "unable to multiply with quaternion";



//---------------------------------------------------------------------------
// Install the new type 'Quaternion' into Lua
//---------------------------------------------------------------------------

void _initQuat(lua_State* L)
{
	lua_pushcfunction(L, _quat_constructor);
	lua_setglobal(L, "Quaternion");

	// Create a quaternion metatable
	lua_newtable(L);

		// Add a type identifier
		lua_pushnumber(L, LUA4_TQUAT);
		lua_rawseti(L, -2, EVENTTABLE_TYPE_INDEX);

		// Tag methods
		lua_pushstring(L, "mul");
		lua_pushcfunction(L, _quat_mul);
		lua_settable(L, -3);

		// Create a table of quaternion methods
		lua_pushstring(L, "gettable");
		lua_newtable(L);

		lua_pushstring(L, "setRotation");
		lua_pushcfunction(L, _quat_setRotation);
		lua_settable(L, -3);

		lua_settable(L, -3);

	// Remember the metatable so I can attach to objects later
	_quat_metatable = _makeStrongRef(L);
}



//---------------------------------------------------------------------------
// Creates a Lua userdata object for a new quaternion, leaves it on top
// of the stack. 
//---------------------------------------------------------------------------

Quaternion* _quat_create(lua_State* L)
{
	Quaternion* q = static_cast<Quaternion*>(lua_newuserdata(L, sizeof(Quaternion)));

	_loadRef(L, _quat_metatable);
	lua_seteventtable(L, -2);
	return q;
}

						 

//---------------------------------------------------------------------------
// Called by the "Quaternion" method in Lua, creates a returns a new
// quaternion instance, initialized with the values on the stack.
//---------------------------------------------------------------------------

int _quat_constructor(lua_State* L)
{
	int top = lua_gettop(L);

	Quaternion* q = _quat_create(L);

	// Copy constructor?
	if (top == 1 && lua4_type(L, 1) == LUA4_TQUAT)
	{
		*q = *(static_cast<Quaternion*>(lua_touserdata(L, 1)));
	}

	// Initializing constructor?
	else 
	{
		q->w = (top < 1 || lua_type(L,1) == LUA_TNIL) ? 1 : luaL_check_number(L, 1);
		q->x = (top < 2 || lua_type(L,2) == LUA_TNIL) ? 0 : luaL_check_number(L, 2);
		q->y = (top < 3 || lua_type(L,3) == LUA_TNIL) ? 0 : luaL_check_number(L, 3);
		q->z = (top < 4 || lua_type(L,4) == LUA_TNIL) ? 0 : luaL_check_number(L, 4);
	}

	return 1;
}



//---------------------------------------------------------------------------
// Initialize a quaternion from an angle-axis rotation.
//---------------------------------------------------------------------------

int _quat_setRotation(lua_State* L)
{
	lua4_check_type(L, 1, LUA4_TQUAT);
	Quaternion* q = static_cast<Quaternion*>(lua_touserdata(L, 1));

	int top = lua_gettop(L);
	if (top == 2)
	{
		lua4_check_type(L, 2, LUA4_TQUAT);
		Quaternion* q2 = static_cast<Quaternion*>(lua_touserdata(L, 2));
		*q = *q2;
	}

	else if (top == 3)
	{
		Float   angle = luaL_check_number(L, 2);
		lua4_check_type(L, 3, LUA4_TVECTOR);
		Vector3* axis = static_cast<Vector3*>(lua_touserdata(L, 3));
		q->setAngleAxisRotation(angle, axis->x, axis->y, axis->z);
	}

	else if (top == 4)
	{
		Float h = luaL_check_number(L, 2);
		Float p = luaL_check_number(L, 3);
		Float r = luaL_check_number(L, 4);
		q->setEulerRotation(h, p, r);
	}

	else if (top == 5)
	{
		Float a = luaL_check_number(L, 2);
		Float x = luaL_check_number(L, 3);
		Float y = luaL_check_number(L, 4);
		Float z = luaL_check_number(L, 5);
		q->setAngleAxisRotation(a, x, y, z);
	}

	lua_settop(L, 1);
	return 1;
}




//---------------------------------------------------------------------------
// Multiply a quaternion by another quaternion or a vector.
//---------------------------------------------------------------------------

int _quat_mul(lua_State* L)
{
	int t1 = lua4_type(L, 1);
	int t2 = lua4_type(L, 2);

	if (t1 == LUA4_TQUAT && t2 == LUA4_TQUAT)
	{
		Quaternion* q1 = static_cast<Quaternion*>(lua_touserdata(L, 1));
		Quaternion* q2 = static_cast<Quaternion*>(lua_touserdata(L, 2));
		Quaternion* res = _quat_create(L);
		*res = (*q1) * (*q2);
		return 1;
	}

	lua_error(L, BAD_MULTIPLIER);
	return 0;
}
