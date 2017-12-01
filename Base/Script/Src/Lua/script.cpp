/**
 * @file script.cpp
 *
 * @author  Jason Perkins (jason@379.com)
 * @date    Copyright (C) 2001 by 379, Inc.
 * @version $Id: script.cpp,v 1.4 2002/05/01 01:25:22 jason379 Exp $
 *
 * @todo Get the 5.0-alpha release.
 *
 */
extern "C"
{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
#include "../src/lstate.h"
}

#include <Core/core.h>
#include <Script/script.h>
#include "extensions.h"
#include <Memory/memory.h>
using namespace f4;


struct ScriptImpl : public ScriptService
{
	ScriptImpl();
	virtual ~ScriptImpl();

	virtual Context* getDefaultContext();
	virtual const Char* getGlobal(const Char* name);
	virtual bool runBuffer(void* buffer, unsigned size, const Char* name, Context* context);

	virtual Service* createA(const Char* typeName);
	virtual const Char* getTypeName(Service* s);
	virtual bool isA(Service* s, const Char* type);
	virtual void reference(Service* s, void* data);
	virtual void release(Service* s, void* data);

	virtual void destroy();
};

// Global instance of the script service
static ScriptService* Script = 0;


//---------------------------------------------------------------------------
// Message text
//---------------------------------------------------------------------------

static const Char* START_MSG   = "Initializing Script service\n";
static const Char* STOP_MSG    = "Closing Script service\n";



//---------------------------------------------------------------------------

F4_EXPORT ScriptService* createScriptService()
{
	Script = new ScriptImpl;
	return Script;
}

//---------------------------------------------------------------------------

ScriptImpl::ScriptImpl()
{
	Core->print(START_MSG);
	Core->print("  %s - %s\n", LUA_VERSION, LUA_COPYRIGHT);
	Core->print("  %s\n", LUA_AUTHORS);

	lua4_init();
}

//---------------------------------------------------------------------------

ScriptImpl::~ScriptImpl()
{
	Core->print(STOP_MSG);
	lua4_term();
}

//---------------------------------------------------------------------------

Context* ScriptImpl::getDefaultContext()
{
	return _asContext(_L);
}

//---------------------------------------------------------------------------

const Char* ScriptImpl::getGlobal(const Char* name)
{
	char cmd[512];
	strcpy(cmd, "return ");
	strcat(cmd, name);
	
	int top = lua_gettop(_L);
	lua_dostring(_L, cmd);
	
	const Char* value = (lua_gettop(_L) > top) ? lua_tostring(_L, top + 1) : 0;
	
	lua_settop(_L, top);
	return value;
}

//---------------------------------------------------------------------------

bool ScriptImpl::runBuffer(void* buffer, unsigned size, const Char* name, Context* context)
{
	lua_State* L = (context) ? _asLuaState(context) : _L;
	return (lua_dobuffer(L, (const char*)buffer, size, name) == 0);
}

//---------------------------------------------------------------------------

Service* ScriptImpl::createA(const Char* typeName)
{
	// Split out the package and service names
	char package[128];
	char service[128];

	const char* ptr = strchr(typeName, '.');
	if (ptr)
	{
		int len = ptr - typeName;
		strncpy(package, typeName, len);
		package[len] = 0;
		strcpy(service, ptr + 1);
	}
	else
	{
		*package = 0;
		strcpy(service, typeName);
	}

	// Remember the state of Lua's stack, restore it when done
	int topOfStack = lua_gettop(_L);

	// Load service table via the package table
	if (*package)
	{
		lua_getglobal(_L, package);
		if (!lua_isnil(_L, -1))
		{
			lua_pushstring(_L, service);
			lua_gettable(_L, -2);
		}
	}
	else
	{
		lua_getglobal(_L, service);
	}

	if (lua_isnil(_L, -1) || !lua4_isservice(_L, -1))
	{
		lua_settop(_L, topOfStack);
		return 0;
	}

	// Create a new instance of the service at the top of the stack
	Service* s = lua4_newinstance(_L, -1);

	lua_settop(_L, topOfStack);
	return s;
}

//---------------------------------------------------------------------------

const Char* ScriptImpl::getTypeName(Service* s)
{
	// Get the service associated with this instance
	_loadRef(_L, (int)s->getScriptData());
	lua_rawgeti(_L, -1, SERVICE_EXPORTS_INDEX);

	// Get the type name from the service
	lua_pushnumber(_L, SERVICE_TYPENAME_INDEX);
	lua_gettable(_L, -2);

	const char* n = lua_isstring(_L, -1) ? lua_tostring(_L, -1) : 0;

	lua_pop(_L, 3);
	return n;
}

//---------------------------------------------------------------------------

bool ScriptImpl::isA(Service* s, const Char* type)
{
	lua_pushstring(_L, "index");

	// Retrieve the service table from the instance
	_loadRef(_L, (int)s->getScriptData());
	lua_rawgeti(_L, -1, SERVICE_EXPORTS_INDEX);

	// Scan down the inheritance hierarchy
	while (!lua_isnil(_L, -1))
	{
		lua_rawgeti(_L, -1, SERVICE_TYPENAME_INDEX);
		if (!lua_isnil(_L, -1))
		{
			if (strcmp(type, lua_tostring(_L, -1)) == 0)
			{
				lua_pop(_L, 4);
				return true;
			}
		}

		lua_pop(_L, 1);
		
		lua_geteventtable(_L, -1);
		lua_pushvalue(_L, -4);
		lua_gettable(_L, -2);
		lua_insert(_L, -3);
		lua_pop(_L, 2);
	}

	lua_pop(_L, 3);
	return false;
}

//---------------------------------------------------------------------------

void ScriptImpl::reference(Service* s, void* data)
{
	// Switch from an unlocked reference to a locked reference
	int oldRef = (int)s->getScriptData();
	_loadRef(_L, oldRef);
	int newRef = _makeStrongRef(_L);

	_dropRef(_L, oldRef);
	s->setScriptData((void*)newRef);
}

//---------------------------------------------------------------------------

void ScriptImpl::release(Service* s, void* data)
{
	// Switch from a locked reference to an unlocked reference
	int oldRef = (int)s->getScriptData();
	_loadRef(_L, oldRef);
	int newRef = _makeWeakRef(_L);

	_dropRef(_L, oldRef);
	s->setScriptData((void*)newRef);
}

//---------------------------------------------------------------------------

void ScriptImpl::destroy()
{
	delete this;
}

