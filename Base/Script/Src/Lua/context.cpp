/**
 * @file context.cpp
 *
 * @author  Jason Perkins (jason@379.com)
 * @date    Copyright (C) 2001 by 379, Inc.
 * @version $Id: context.cpp,v 1.3 2002/04/22 20:21:28 jason379 Exp $
 */
#include <stdio.h>
#include <string.h>

extern "C"
{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
#include "Lua/src/lstate.h"
}

#include <Core/core.h>
#include <Math/math.h>
#include "extensions.h"
#include <Memory/memory.h>
using namespace f4;


//---------------------------------------------------------------------------
// Message text
//---------------------------------------------------------------------------

static const Char* NOT_ENOUGH_ARGS = "not enough arguments";
static const Char* WRONG_SERVICE_TYPE = "expected type %s at position %d";
static const Char* CHECK_CALL_SYNTAX  = "\nDid you use '.' instead of ':'?";



//---------------------------------------------------------------------------

struct ContextImpl : public Context
{
	ContextImpl(lua_State* L);
	virtual ~ContextImpl() { }

	virtual Service*  beginCall(const Char* expectedType, int* args);

	virtual void      beginDispatch(Service* s, const Char* method);
	virtual void      dispatch();
	virtual void      endDispatch();

	virtual bool      asBool(int pos);
	virtual Double    asDouble(int pos);
	virtual Float     asFloat(int pos);
	virtual int       asInt(int pos);
	virtual Matrix44& asMatrix(int pos);
	virtual void*     asPointer(int pos);
	virtual Quaternion& asQuaternion(int pos);
	virtual Service*  asService(int pos, const Char* expectedType);
	virtual const Char* asString(int pos);
	virtual Vector3&  asVector(int pos);

	virtual void      pushBool(bool b);
	virtual void      pushDouble(Double d);
	virtual void      pushFloat(Float f);
	virtual void      pushInt(int i);
	virtual void      pushMatrix(const Matrix44& m);
	virtual void      pushNull();
	virtual void      pushPointer(void* p);
	virtual void      pushQuaternion(const Quaternion& q);
	virtual void      pushService(Service* s);
	virtual void      pushString(const Char* s);
	virtual void      pushString(const std::string& x);
	virtual void      pushVector(const Vector3& v);

	virtual void printContents();

	lua_State* L;

private:
	int m_firstInput;
	int m_lastInput;
};


//---------------------------------------------------------------------------

Context* _createContext(lua_State* L)
{
	return new ContextImpl(L);
}

//---------------------------------------------------------------------------

void _destroyContext(lua_State* L)
{
	ContextImpl* c = static_cast<ContextImpl*>(L->f4Data);
	delete c;
}

//---------------------------------------------------------------------------

lua_State* _asLuaState(Context* c)
{
	return (static_cast<ContextImpl*>(c))->L;
}

//---------------------------------------------------------------------------

Context* _asContext(lua_State* L)
{
	return static_cast<ContextImpl*>(L->f4Data);
}

//---------------------------------------------------------------------------

ContextImpl::ContextImpl(lua_State* L)
{
	this->L = L;
	L->f4Data = this;
}

//---------------------------------------------------------------------------

Service* ContextImpl::beginCall(const Char* expectedType, int* args)
{
	m_firstInput = 1;
	m_lastInput  = lua_gettop(L);

	Service* s = asService(0, expectedType);

	m_firstInput = 2;
	*args = m_lastInput - m_firstInput + 1;
	return s;
}

//---------------------------------------------------------------------------

void ContextImpl::beginDispatch(Service* s, const Char* method)
{
	// Retrieve the function
	lua4_pushinstance(L, s);
	lua_pushstring(L, method);
	lua_gettable(L, -2);

	// If I get a cfunction, *don't* call it. Most likely this means I've
	// gotten a pointer back to the function that's making the dispatch to
	// begin with, if I call it I'll start an endless recursion. This means
	// component methods can only be replaced with Lua code and not C, but
	// it should be easy enough to work around if necessary.
	if (lua_iscfunction(L, -1))
	{
		lua_pop(L, 1);
		lua_pushnil(L);
	}

	// Put the function ahead of the service instance object, which should
	// be the first argument (self)
	lua_insert(L, -2);

	m_firstInput = lua_gettop(L);
}

//---------------------------------------------------------------------------

void ContextImpl::dispatch()
{
	int numArgs = lua_gettop(L) - m_firstInput + 1;

	if (lua_isfunction(L, m_firstInput - 1))
	{
		lua_call(L, numArgs, 1);
	}
	else
	{
		lua_pop(L, numArgs + 1);
		lua_pushnil(L);
	}

	m_firstInput = m_lastInput = lua_gettop(L);
}

//---------------------------------------------------------------------------

void ContextImpl::endDispatch()
{
	lua_pop(L, 1);
}

//---------------------------------------------------------------------------

bool ContextImpl::asBool(int pos)
{
	pos += m_firstInput;
	if (pos < m_firstInput || pos > m_lastInput) lua_error(L, NOT_ENOUGH_ARGS);
	return (lua_istrue(L, pos) != 0);
}

//---------------------------------------------------------------------------

Double ContextImpl::asDouble(int pos)
{
	pos += m_firstInput;
	if (pos < m_firstInput || pos > m_lastInput) lua_error(L, NOT_ENOUGH_ARGS);
	return static_cast<Double>(luaL_check_number(L, pos));
}

//---------------------------------------------------------------------------

Float ContextImpl::asFloat(int pos)
{
	pos += m_firstInput;
	if (pos < m_firstInput || pos > m_lastInput) lua_error(L, NOT_ENOUGH_ARGS);
	return static_cast<Float>(luaL_check_number(L, pos));
}

//---------------------------------------------------------------------------

int ContextImpl::asInt(int pos)
{
	pos += m_firstInput;
	if (pos < m_firstInput || pos > m_lastInput) lua_error(L, NOT_ENOUGH_ARGS);
	return static_cast<int>(luaL_check_number(L, pos));
}

//---------------------------------------------------------------------------

Matrix44& ContextImpl::asMatrix(int pos)
{
	pos += m_firstInput;
	if (pos < m_firstInput || pos > m_lastInput) lua_error(L, NOT_ENOUGH_ARGS);
	lua4_check_type(L, pos, LUA4_TMATRIX);
	Matrix44* m = static_cast<Matrix44*>(lua_touserdata(L, pos));
	return *m;
}

//---------------------------------------------------------------------------

void* ContextImpl::asPointer(int pos)
{
	pos += m_firstInput;
	if (pos < m_firstInput || pos > m_lastInput) lua_error(L, NOT_ENOUGH_ARGS);
	if (lua_isnil(L, pos)) return NULL;
	luaL_check_type(L, pos, LUA_TUSERDATA);
	return lua_touserdata(L, pos);
}

//---------------------------------------------------------------------------

Quaternion& ContextImpl::asQuaternion(int pos)
{
	pos += m_firstInput;
	if (pos < m_firstInput || pos > m_lastInput) lua_error(L, NOT_ENOUGH_ARGS);
	lua4_check_type(L, pos, LUA4_TQUAT);
	Quaternion* q = static_cast<Quaternion*>(lua_touserdata(L, pos));
	return *q;
}

//---------------------------------------------------------------------------

Service* ContextImpl::asService(int pos, const Char* expectedType)
{
	pos += m_firstInput;
	if (pos < m_firstInput || pos > m_lastInput) lua_error(L, NOT_ENOUGH_ARGS);

	Service* s = 0;

	// A 'nil' is equivalent to NULL in C++
	if (lua_isnil(L, pos) )
		return 0;

	// Make sure it's really a service instance
	if (!lua4_isinstance(L, pos))
		goto failure;

	lua_rawgeti(L, pos, SERVICE_INSTANCE_INDEX);
	s = static_cast<Service*>(lua_touserdata(L, -1));
	lua_pop(L, 1);

	// Make sure the type is compatible
	if (!s->isA(expectedType))
		goto failure;

	return s;

failure:
	char msg[512];
	sprintf(msg, WRONG_SERVICE_TYPE, expectedType, pos);
	if (pos == 1) strcat(msg, CHECK_CALL_SYNTAX);
	lua_error(L, msg);
	return 0;
}

//---------------------------------------------------------------------------

const Char* ContextImpl::asString(int pos)
{
	pos += m_firstInput;
	if (pos < m_firstInput || pos > m_lastInput) lua_error(L, NOT_ENOUGH_ARGS);
	if (lua_isnil(L, pos)) return NULL;
	return luaL_check_string(L, pos);
}

//---------------------------------------------------------------------------

Vector3& ContextImpl::asVector(int pos)
{
	pos += m_firstInput;
	if (pos < m_firstInput || pos > m_lastInput) lua_error(L, NOT_ENOUGH_ARGS);
	lua4_check_type(L, pos, LUA4_TVECTOR);
	Vector3* v = static_cast<Vector3*>(lua_touserdata(L, pos));
	return *v;
}

//---------------------------------------------------------------------------

void ContextImpl::pushBool(bool b)
{
	if (b)
		lua_pushboolean(L, 1);
	else
		lua_pushnil(L);
}

//---------------------------------------------------------------------------

void ContextImpl::pushDouble(Double d)
{
	lua_pushnumber(L, d);
}

//---------------------------------------------------------------------------

void ContextImpl::pushFloat(Float f)
{
	lua_pushnumber(L, f);
}

//---------------------------------------------------------------------------

void ContextImpl::pushInt(int i)
{
	lua_pushnumber(L, i);
}

//---------------------------------------------------------------------------

void ContextImpl::pushMatrix(const Matrix44& m)
{
	Matrix44* mat = _mat_create(L);
	*mat = m;
}

//---------------------------------------------------------------------------

void ContextImpl::pushNull()
{
	lua_pushnil(L);
}

//---------------------------------------------------------------------------

void ContextImpl::pushPointer(void* p)
{
	lua_newuserdatabox(L, p);
}

//---------------------------------------------------------------------------

void ContextImpl::pushQuaternion(const Quaternion& q)
{
	Quaternion* quat = _quat_create(L);
	*quat = q;
}

//---------------------------------------------------------------------------

void ContextImpl::pushService(Service* s)
{
	if (s)
		lua4_pushinstance(L, s);
	else
		lua_pushnil(L);
}

//---------------------------------------------------------------------------

void ContextImpl::pushString(const Char* s)
{
	lua_pushstring(L, s);
}

//---------------------------------------------------------------------------

void ContextImpl::pushString(const std::string& s)
{
	lua_pushstring(L, s.c_str());
}

//---------------------------------------------------------------------------

void ContextImpl::pushVector(const Vector3& v)
{
	Vector3* vec = _vec_create(L);
	*vec = v;
}

//---------------------------------------------------------------------------

void ContextImpl::printContents()
{
	lua4_showstack(L);
}
