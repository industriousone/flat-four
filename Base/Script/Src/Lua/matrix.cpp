/**
 * @file matrix.cpp
 *
 * @author  Jason Perkins (jason@379.com)
 * @date    Copyright (C) 2001 by 379, Inc.
 * @version $Id: matrix.cpp,v 1.3 2002/04/22 20:21:28 jason379 Exp $
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
// Local functions
//---------------------------------------------------------------------------

static int _mat_metatable;

static int _mat_setRotation(lua_State* L);
static int _mat_setPosition(lua_State* L);
static int _mat_getPosition(lua_State* L);
static int _mat_mul(lua_State* L);


//---------------------------------------------------------------------------
// Message text
//---------------------------------------------------------------------------

static const Char* BAD_MULTIPLIER = "unable to multiply with matrix";


//---------------------------------------------------------------------------
// Install the new type 'Matrix' into Lua
//---------------------------------------------------------------------------

void _initMatrix(lua_State* L)
{
	lua_pushcfunction(L, _mat_constructor);
	lua_setglobal(L, "Matrix");

	// Create a matrix metatable
	lua_newtable(L);

		// Add a type identifier
		lua_pushnumber(L, LUA4_TMATRIX);
		lua_rawseti(L, -2, EVENTTABLE_TYPE_INDEX);

		// Tag methods
		lua_pushstring(L, "mul");
		lua_pushcfunction(L, _mat_mul);
		lua_settable(L, -3);

		// Create a table of matrix methods
		lua_pushstring(L, "gettable");
		lua_newtable(L);

		lua_pushstring(L, "setRotation");
		lua_pushcfunction(L, _mat_setRotation);
		lua_settable(L, -3);

		lua_pushstring(L, "setPosition");
		lua_pushcfunction(L, _mat_setPosition);
		lua_settable(L, -3);

		lua_pushstring(L, "getPosition");
		lua_pushcfunction(L, _mat_getPosition);
		lua_settable(L, -3);

		lua_settable(L, -3);

	// Remember the metatable so I can attach to objects later
	_mat_metatable = _makeStrongRef(L);
}



//---------------------------------------------------------------------------
// Creates a Lua userdata object for a new quaternion, leaves it on top
// of the stack. 
//---------------------------------------------------------------------------

Matrix44* _mat_create(lua_State* L)
{
	Matrix44* m = static_cast<Matrix44*>(lua_newuserdata(L, sizeof(Matrix44)));

	_loadRef(L, _mat_metatable);
	lua_seteventtable(L, -2);
	return m;
}


//---------------------------------------------------------------------------
// Called by the "Matrix" method in Lua, creates a returns a new matrix 
// instance, initialized with the values on the stack.
//---------------------------------------------------------------------------

int _mat_constructor(lua_State* L)
{
	int top = lua_gettop(L);

	Matrix44* m = _mat_create(L);
	
	// Copy constructor?
	if (top == 1 && lua4_type(L, 1) == LUA4_TMATRIX)
	{
		*m = *(static_cast<Matrix44*>(lua_touserdata(L, 1)));
	}

	// Initializing constructor?
	else if (top == 2 && lua4_type(L,1) == LUA4_TQUAT && lua4_type(L,2) == LUA4_TVECTOR)
	{
		Quaternion* q = static_cast<Quaternion*>(lua_touserdata(L, 1));
		Vector3*    v = static_cast<Vector3*>(lua_touserdata(L, 2));

		m->setIdentity();
		m->setQuaternion(*q);
		m->setTranslation(v->x, v->y, v->z);
	}

	// Identity matrix
	else
	{
		m->setIdentity();
	}

	return 1;
}



//---------------------------------------------------------------------------
// Set the rotation component of a matrix.
//---------------------------------------------------------------------------

int _mat_setRotation(lua_State* L)
{
	lua4_check_type(L, 1, LUA4_TMATRIX);
	Matrix44* m = static_cast<Matrix44*>(lua_touserdata(L, 1));

	int top = lua_gettop(L);
	if (top == 2)
	{
		lua4_check_type(L, 2, LUA4_TQUAT);
		Quaternion* q = static_cast<Quaternion*>(lua_touserdata(L, 2));
		m->setQuaternion(*q);
	}

	else if (top == 3)
	{
		Float   angle = luaL_check_number(L, 2);
		lua4_check_type(L, 3, LUA4_TVECTOR);
		Vector3* axis = static_cast<Vector3*>(lua_touserdata(L, 3));
		m->setAngleAxisRotation(angle, axis->x, axis->y, axis->z);
	}

	else if (top == 4)
	{
		Float h = luaL_check_number(L, 2);
		Float p = luaL_check_number(L, 3);
		Float r = luaL_check_number(L, 4);
		m->setEulerRotation(h, p, r);
	}

	else if (top == 5)
	{
		Float a = luaL_check_number(L, 2);
		Float x = luaL_check_number(L, 3);
		Float y = luaL_check_number(L, 4);
		Float z = luaL_check_number(L, 5);
		m->setAngleAxisRotation(a, x, y, z);
	}

	lua_settop(L, 1);
	return 1;
}



//---------------------------------------------------------------------------
// Set the translational part of a matrix.
//---------------------------------------------------------------------------

int _mat_setPosition(lua_State* L)
{
	lua4_check_type(L, 1, LUA4_TMATRIX);
	Matrix44* m = static_cast<Matrix44*>(lua_touserdata(L, 1));

	if (lua_gettop(L) == 2 && lua4_type(L, 2) == LUA4_TVECTOR)
	{
		Vector3* v = static_cast<Vector3*>(lua_touserdata(L, 2));
		m->setTranslation(*v);
	}
	else
	{
		Float x = luaL_check_number(L, 2);
		Float y = luaL_check_number(L, 3);
		Float z = luaL_check_number(L, 4);
		m->setTranslation(x, y, z);
	}

	lua_settop(L, 1);
	return 1;
}


//---------------------------------------------------------------------------
// Get the translation part of the matrix as a vector.
//---------------------------------------------------------------------------

int _mat_getPosition(lua_State* L)
{
	lua4_check_type(L, 1, LUA4_TMATRIX);
	Matrix44* m = static_cast<Matrix44*>(lua_touserdata(L, 1));

	lua_settop(L, 0);
	_vec_constructor(L);
	Vector3* v = static_cast<Vector3*>(lua_touserdata(L, -1));
	v->x = (*m)[3][0];
	v->y = (*m)[3][1];
	v->z = (*m)[3][2];
	return 1;
}


//---------------------------------------------------------------------------
// Multiply a matrix by another matrix.
//---------------------------------------------------------------------------

int _mat_mul(lua_State* L)
{
	lua4_check_type(L, 1, LUA4_TMATRIX);
	Matrix44* m = static_cast<Matrix44*>(lua_touserdata(L, 1));

	int t2 = lua4_type(L, 2);
	if (t2 == LUA4_TMATRIX)
	{
		Matrix44* r = _mat_create(L);
		Matrix44* b = static_cast<Matrix44*>(lua_touserdata(L, 2));
		Mat44Multiply(r, m, b);
		return 1;
	}

	else if (t2 == LUA4_TVECTOR)
	{
		Vector3* r = _vec_create(L);
		Vector3* b = static_cast<Vector3*>(lua_touserdata(L, 2));
		Mat44Transform(r, m, b);
		return 1;
	}

	else 
	{
		lua_error(L, BAD_MULTIPLIER);
		return 0;
	}
}

