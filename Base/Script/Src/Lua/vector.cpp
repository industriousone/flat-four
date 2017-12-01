/**
 * @file vector.cpp
 *
 * @author  Jason Perkins (jason@379.com)
 * @date    Copyright (C) 2001 by 379, Inc.
 * @version $Id: vector.cpp,v 1.2 2002/04/22 01:51:55 jason379 Exp $
 */
#include <string.h>

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

static int _vec_metatable;

static int  _vec_gettable(lua_State* L);
static int  _vec_settable(lua_State* L);
static int  _vec_add(lua_State* L);
static int  _vec_sub(lua_State* L);
static int  _vec_mul(lua_State* L);
static int  _vec_unm(lua_State* L);
static int  _vec_normalize(lua_State* L);
static int  _vec_cross(lua_State* L);
static int  _vec_dot(lua_State* L);


//---------------------------------------------------------------------------
// Message text
//---------------------------------------------------------------------------

static const Char* BAD_MULTIPLIER = "unable to multiply with vector";


//---------------------------------------------------------------------------
// Install the new type 'Vector' into Lua
//---------------------------------------------------------------------------

void _initVector(lua_State* L)
{
	lua_pushcfunction(L, _vec_constructor);
	lua_setglobal(L, "Vector");

	// Create a vector metatable
	lua_newtable(L);

		// Add a type identifier
		lua_pushnumber(L, LUA4_TVECTOR);
		lua_rawseti(L, -2, EVENTTABLE_TYPE_INDEX);

		// Tag methods
		lua_pushstring(L, "gettable");
		lua_pushcfunction(L, _vec_gettable);
		lua_settable(L, -3);

		lua_pushstring(L, "settable");
		lua_pushcfunction(L, _vec_settable);
		lua_settable(L, -3);

		lua_pushstring(L, "add");
		lua_pushcfunction(L, _vec_add);
		lua_settable(L, -3);

		lua_pushstring(L, "sub");
		lua_pushcfunction(L, _vec_sub);
		lua_settable(L, -3);

		lua_pushstring(L, "mul");
		lua_pushcfunction(L, _vec_mul);
		lua_settable(L, -3);

		lua_pushstring(L, "unm");
		lua_pushcfunction(L, _vec_unm);
		lua_settable(L, -3);

	// Remember the metatable so I can attach to objects later
	_vec_metatable = _makeStrongRef(L);
}



//---------------------------------------------------------------------------
// Creates a Lua userdata object for a new vector, leaves it on top of 
// the stack. 
//---------------------------------------------------------------------------

Vector3* _vec_create(lua_State* L)
{
	Vector3* v = static_cast<Vector3*>(lua_newuserdata(L, sizeof(Vector3)));

	_loadRef(L, _vec_metatable);
	lua_seteventtable(L, -2);
	return v;
}



//---------------------------------------------------------------------------
// Called by the "Vector" method in Lua, creates a returns a new vector
// instance, initialized with the values on the stack.
//---------------------------------------------------------------------------

int _vec_constructor(lua_State* L)
{
	int top = lua_gettop(L);

	Vector3* v = _vec_create(L);

	// Copy constructor?
	if (top == 1 && lua4_type(L, 1) == LUA4_TVECTOR)
	{
		*v = *(static_cast<Vector3*>(lua_touserdata(L, 1)));
	}

	// Initializing constructor?
	else
	{
		v->x = (top < 1 || lua_type(L,1) == LUA_TNIL) ? 0 : luaL_check_number(L, 1);
		v->y = (top < 2 || lua_type(L,2) == LUA_TNIL) ? 0 : luaL_check_number(L, 2);
		v->z = (top < 3 || lua_type(L,3) == LUA_TNIL) ? 0 : luaL_check_number(L, 3);
	}

	return 1;
}



//---------------------------------------------------------------------------
// Access a vector component or one of the predefined methods.
//---------------------------------------------------------------------------

int _vec_gettable(lua_State* L)
{
	Vector3* v = static_cast<Vector3*>(lua_touserdata(L, 1));
	const char* i = luaL_check_string(L, 2);

	if (i[1] == 0)
	{
		switch(*i)
		{
		case 'x':
		case 'h':
			lua_pushnumber(L, v->x); 
			break;
		case 'y':
		case 'p':
			lua_pushnumber(L, v->y); 
			break;
		case 'z':
		case 'r':
			lua_pushnumber(L, v->z); 
			break;
		default:
			return 0;
		}
		return 1;
	}

	else if (strcmp(i, "cross") == 0)
	{
		lua_pushcfunction(L, _vec_cross);
		return 1;
	}

	else if (strcmp(i, "dot") == 0)
	{
		lua_pushcfunction(L, _vec_dot);
		return 1;
	}

	else if (strcmp(i, "normalize") == 0)
	{
		lua_pushcfunction(L, _vec_normalize);
		return 1;
	}

	else if (strcmp(i, "length") == 0)
	{
		lua_pushnumber(L, v->length());
		return 1;
	}

	return 0;
}



//---------------------------------------------------------------------------
// Store data in a vector component.
//---------------------------------------------------------------------------

int _vec_settable(lua_State* L)
{
	Vector3*    v = static_cast<Vector3*>(lua_touserdata(L, 1));
	const Char* i = luaL_check_string(L, 2);
	Float       a = static_cast<Float>(luaL_check_number(L, 3));

	if (i[1] == 0)
	{
		switch(*i)
		{
		case 'x':
		case 'h':
			v->x = a; 
			break;
		case 'y':
		case 'p':
			v->y = a; 
			break;
		case 'z':
		case 'r':
			v->z = a; 
			break;
		}
	}

	return 0;
}



//---------------------------------------------------------------------------
// Add two vectors.
//---------------------------------------------------------------------------

int _vec_add(lua_State* L)
{
	lua4_check_type(L, 1, LUA4_TVECTOR);
	lua4_check_type(L, 2, LUA4_TVECTOR);

	Vector3* a = static_cast<Vector3*>(lua_touserdata(L, 1));
	Vector3* b = static_cast<Vector3*>(lua_touserdata(L, 2));

	Vector3* r = _vec_create(L);
	*r = (*a) + (*b);
	return 1;
}



//---------------------------------------------------------------------------
// Subtract two vectors.
//---------------------------------------------------------------------------

int _vec_sub(lua_State* L)
{
	lua4_check_type(L, 1, LUA4_TVECTOR);
	lua4_check_type(L, 2, LUA4_TVECTOR);

	Vector3* a = static_cast<Vector3*>(lua_touserdata(L, 1));
	Vector3* b = static_cast<Vector3*>(lua_touserdata(L, 2));

	Vector3* r = _vec_create(L);
	*r = (*a) - (*b);
	return 1;
}



//---------------------------------------------------------------------------
// Scale a vector.
//---------------------------------------------------------------------------

int _vec_mul(lua_State* L)
{
	int t1 = lua4_type(L, 1);
	int t2 = lua4_type(L, 2);

	if (t1 == LUA_TNUMBER || t2 == LUA_TNUMBER)
	{
		Vector3* v = static_cast<Vector3*>(lua_touserdata(L, (t1 == LUA4_TVECTOR) ? 1 : 2));
		Float    a = static_cast<Float>(lua_tonumber(L, (t1 == LUA_TNUMBER) ? 1 : 2));

		Vector3* r = _vec_create(L);
		*r = (*v) * a;
		return 1;
	}

	lua_error(L, BAD_MULTIPLIER);
	return 0;
}



//---------------------------------------------------------------------------
// Negate a vector.
//---------------------------------------------------------------------------

int _vec_unm(lua_State* L)
{
	lua4_check_type(L, 1, LUA4_TVECTOR);
	Vector3* v = static_cast<Vector3*>(lua_touserdata(L, 1));

	Vector3* r = _vec_create(L);
	*r = -(*v);
	return 1;
}



//---------------------------------------------------------------------------
// Set the vector length to one.
//---------------------------------------------------------------------------

int _vec_normalize(lua_State* L)
{
	lua4_check_type(L, 1, LUA4_TVECTOR);
	Vector3* v = static_cast<Vector3*>(lua_touserdata(L, 1));
	v->normalize();
	return 1;
}



//---------------------------------------------------------------------------
// Vector dot product.
//---------------------------------------------------------------------------

int _vec_dot(lua_State* L)
{
	lua4_check_type(L, 1, LUA4_TVECTOR);
	lua4_check_type(L, 2, LUA4_TVECTOR);

	Vector3* a = static_cast<Vector3*>(lua_touserdata(L, 1));
	Vector3* b = static_cast<Vector3*>(lua_touserdata(L, 2));

	lua_pushnumber(L, a->dot(*b));
	return 1;
}



//---------------------------------------------------------------------------
// Vector cross product
//---------------------------------------------------------------------------

int _vec_cross(lua_State* L)
{
	lua4_check_type(L, 1, LUA4_TVECTOR);
	lua4_check_type(L, 2, LUA4_TVECTOR);

	Vector3* a = static_cast<Vector3*>(lua_touserdata(L, 1));
	Vector3* b = static_cast<Vector3*>(lua_touserdata(L, 2));

	Vector3* r = _vec_create(L);
	*r = a->cross(*b);
	return 1;
}

