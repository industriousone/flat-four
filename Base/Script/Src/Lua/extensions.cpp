/**
 * @file extensions.cpp
 *
 * @author  Jason Perkins (jason@379.com)
 * @date    Copyright (C) 2001 by 379, Inc.
 * @version $Id: extensions.cpp,v 1.3 2002/04/25 01:19:30 jason379 Exp $
 *
 * @todo Big issue: while C++ extends interfaces, script extends implementations.
 *       Problems with recursion when I try to extend an existing C++ impl with
 *       script, need to reconsider entire process.
 *
 * @todo Add provision to call super class
 *
 * @todo The property methods are a bit of a hack. Can I find a way that
 *       doesn't involve mangling the property names? Store a pointer to
 *       the exports entry (ie. &{ "prop", F4_X_PROP_INT, get, set })
 *       as a userdatabox and access that instead?
 *
 * @todo Can I rewrite the reference manager to use object ptrs as keys?
 *       Might be possible to remove scriptData stuff from C++ objects.
 *
 */
#include <string.h>

extern "C"
{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
int passresults (lua_State *L, int status, int oldtop);
}

#include <Core/core.h>
#include <Script/script.h>
#include "extensions.h"
#include <Memory/memory.h>
using namespace f4;


/*
 * The code in this file responds to several different Lua actions, and can
 * be hard to follow as a whole. A quick primer:
 *
 * A new instance of a service is created like so:
 *
 *   i = Package.Service()
 *
 * When the code tries to access an undefined global variable called 
 * 'Package', that triggers a call to _loadPackage(), which tries to find a
 * DLL or directory with the given name. If successful, it creates a table,
 * assigns in LUA4_TPACKAGE, and stores in the global table for future use.
 *
 * When the code tries to access an element in a package table that doesn't
 * exist (the ".Service" part of the code above), that triggers a call to
 * _loadService(), which looks though the list of exported services (stored
 * in the package table) for a match. If successful, it creates a table and
 * then calls _wrapCService(), which handles the details of setting up the
 * new table.
 *
 * When the code tries to call the service table as a function, that calls
 * _createInstance().
 *
 */

  
//---------------------------------------------------------------------------
// Local data
//---------------------------------------------------------------------------

lua_State* _L = 0;

static int _packageEventTable  = 0;
static int _serviceEventTable  = 0;
static int _instanceEventTable = 0;
static int _cdataEventTable    = 0;



//---------------------------------------------------------------------------
// Local functions
//---------------------------------------------------------------------------

static int  _type(lua_State* L);
static int  _loadPackage(lua_State* L);
static int  _loadService(lua_State* L);
static int  _createInstance(lua_State* L);
static int  _collectInstance(lua_State* L);
static int  _getItem(lua_State* L);
static int  _setItem(lua_State* L);
static int  _createNewType(lua_State* L);
static void _wrapService(lua_State* L, ServiceInfo* s);
static void _loadExports(lua_State* L, ExportInfo* e);
static bool _callInitialize(lua_State* L, Service* s);


//---------------------------------------------------------------------------
// Message text
//---------------------------------------------------------------------------

static const char* SERVICE_EXT  = ".lua";
static const char* BAD_BASE_SRV = "invalid argument, expected a service";



//---------------------------------------------------------------------------

void lua4_init()
{
	// Create the global scripting context
	_L = lua_open(0);
	_createContext(_L);


	// Load some of the standard Lua libraries
	lua_baselibopen(_L);
	lua_strlibopen(_L);
	lua_mathlibopen(_L);


	// Initialize my object reference system and new data types
	_initRefs(_L);
	_initVector(_L);
	_initMatrix(_L);
	_initQuat(_L);


	// Replace the 'type' function with one that understands my types
	lua_pushcfunction(_L, _type);
	lua_setglobal(_L, "type");


	// Route the global table's "index" to the package loading function
	lua_getglobals(_L);

	lua_newtable(_L);
	lua_pushstring(_L, "index");
	lua_pushcfunction(_L, _loadPackage);
	lua_settable(_L, -3);

	lua_seteventtable(_L, -2);

	
	// Create an event table for packages
	lua_newtable(_L);
	lua_pushstring(_L, "index");
	lua_pushcfunction(_L, _loadService);
	lua_settable(_L, -3);
	lua_pushnumber(_L, LUA4_TPACKAGE);
	lua_rawseti(_L, -2, EVENTTABLE_TYPE_INDEX);
	_packageEventTable = _makeStrongRef(_L);


	// Create an event table for C++ services
	lua_newtable(_L);
	lua_pushstring(_L, "call");
	lua_pushcfunction(_L, _createInstance);
	lua_settable(_L, -3);
	lua_pushnumber(_L, LUA4_TSERVICE);
	lua_rawseti(_L, -2, EVENTTABLE_TYPE_INDEX);
	_serviceEventTable = _makeStrongRef(_L);


	// Create an event table for instances
	lua_newtable(_L);
	lua_pushstring(_L, "gettable");
	lua_pushcfunction(_L, _getItem);
	lua_settable(_L, -3);
	lua_pushstring(_L, "settable");
	lua_pushcfunction(_L, _setItem);
	lua_settable(_L, -3);
	lua_pushnumber(_L, LUA4_TINSTANCE);
	lua_rawseti(_L, -2, EVENTTABLE_TYPE_INDEX);
	_instanceEventTable = _makeStrongRef(_L);

	
	// Create an event table for the C++ service instance
	lua_newtable(_L);
	lua_pushstring(_L, "gc");
	lua_pushcfunction(_L, _collectInstance);
	lua_settable(_L, -3);
	_cdataEventTable = _makeStrongRef(_L);

	
	// Create a global "core" to access the CoreService interface
	lua4_pushinstance(_L, Core);
	lua_setglobal(_L, "core");


	// Add a registration function for new scripted services
	lua_pushcfunction(_L, _createNewType);
	lua_setglobal(_L, "Service");
}

//---------------------------------------------------------------------------

void lua4_term()
{
	_destroyContext(_L);
	lua_close(_L);
}

//---------------------------------------------------------------------------

int lua4_type(lua_State* L, int i)
{
	int t = lua_type(L, i);
	if (t == LUA_TUSERDATA || t == LUA_TTABLE)
	{
		lua_geteventtable(L, i);
		if (!lua_isnil(L, -1))
		{
			lua_rawgeti(L, -1, EVENTTABLE_TYPE_INDEX);
			if (!lua_isnil(L, -1)) t = static_cast<int>(lua_tonumber(L, -1));
			lua_pop(L, 1);
		}

		lua_pop(L, 1);
	}

	return t;
}

	
//---------------------------------------------------------------------------

const Char* lua4_typename(lua_State* L, int t)
{
	if (t < LUA_TNONE)
	{
		switch (t)
		{
		case LUA4_TPACKAGE:   return "Package";
		case LUA4_TSERVICE:   return "Service";
		case LUA4_TINSTANCE:  return "Instance";
		case LUA4_TVECTOR:    return "Vector";
		case LUA4_TMATRIX:    return "Matrix";
		case LUA4_TQUAT:      return "Quaternion";
		default:              return "Unknown";
		}
	}
	else
	{
		return lua_typename(L, t);
	}
}

//---------------------------------------------------------------------------

void lua4_check_type(lua_State* L, int i, int t)
{
	if (lua4_type(L, i) != t)
		luaL_typerror(L, i, lua4_typename(L, t)); 
}

//---------------------------------------------------------------------------

Service* lua4_newinstance(lua_State* L, int i)
{
	// Make sure the referenced object is a service
	if (!lua4_isservice(L, i)) return 0;

	// I need to know if the service was written in C++ or Lua, so I know
	// how to create the new instance. Only C++ services will have the info
	// pointer, all Lua service will get it through inheritance.
	ServiceInfo* info = 0;
	lua_rawgeti(L, i, SERVICE_BINDINFO_INDEX);
	if (!lua_isnil(L, -1))
		info = static_cast<ServiceInfo*>(lua_touserdata(L, -1));
	lua_pop(L, 1);

	// C++ services can call a factory function, Lua services must call the
	// service table as a function
	Service* s;
	if (info)
	{
		s = info->factory();
	}
	else
	{
		lua_pushvalue(L, i);
		lua_call(L, 0, 1);
		s = lua4_isinstance(L,-1) ? static_cast<Service*>(lua_touserdata(L,-1)) : 0;
	}

	return s;
}

//---------------------------------------------------------------------------

Service* lua4_toinstance(lua_State* L, int i)
{
	if (lua4_type(L, i) != LUA4_TINSTANCE) return 0;
	lua_rawgeti(L, i, SERVICE_INSTANCE_INDEX);
	Service* s = static_cast<Service*>(lua_touserdata(L, -1));
	lua_pop(L, 1);
	return s;
}

//---------------------------------------------------------------------------

bool lua4_isinstance(lua_State* L, int i)
{
	int t = lua4_type(L, i);
	return (t == LUA4_TINSTANCE);
}

//---------------------------------------------------------------------------

bool lua4_isservice(lua_State* L, int i)
{
	int t = lua4_type(L, i);
	return (t == LUA4_TSERVICE);
}

//---------------------------------------------------------------------------

void lua4_pushinstance(lua_State* L, Service* s)
{
	// If it's already wrapped...
	if (s->getScriptData())
	{
		_loadRef(L, (int)s->getScriptData());
		return;
	}

	lua_newtable(L);

	// Create a Lua object to represent the C instance pointer, and attach the
	// instance metatable to it (which handles the "gc" tag). Finally, store
	// the C pointer in the new instance table
	lua_newuserdatabox(L, s);
	_loadRef(L, _cdataEventTable);
	lua_seteventtable(L, -2);
	lua_rawseti(L, -2, SERVICE_INSTANCE_INDEX);

	// Store a reference to the service table. Rather than rely on a separate
	// metatable for every type of service, I implement my own inheritance.
	_wrapService(L, s->getTypeInfo());
	lua_rawseti(L, -2, SERVICE_EXPORTS_INDEX);

	// Attach the instance event table, which sets "gettable" and "settable"
	// tags for looking up property methods
	_loadRef(L, _instanceEventTable);
	lua_seteventtable(L, -2);

	// Store a reference to the instance table in the C++ instance so I can
	// quickly access it when I need to pass the instance back to Lua. The
	// lua_pushvalue() leaves a reference to the object on the stack when
	// I'm done so it can be returned to the caller
	lua_pushvalue(L, -1);
	s->reference();
	int ref = (s->countReferences() > 1) ? _makeStrongRef(L) : _makeWeakRef(L);
	s->setScriptData((void*)ref);
}

//---------------------------------------------------------------------------

const char* lua4_asstring(lua_State* L, int i)
{
	if (i < 0 && i > LUA_REGISTRYINDEX) --i;

	lua_getglobal(L, "tostring");
	lua_pushvalue(L, i);
	lua_rawcall(L, 1, 1);
	const char* s = lua_tostring(L, -1);
	lua_pop(L, 1);

	return s;
}

//---------------------------------------------------------------------------

void lua4_showstack(lua_State* L)
{
	for (int i = 1; i <= lua_gettop(L); ++i)
	{
		Core->print("[%02d] ", i);

		lua_getglobal(L, "tostring");
		lua_pushvalue(L, i);
		lua_rawcall(L, 1, 1);
		const char* s = lua_tostring(L, -1);
		lua_pop(L, 1);

		switch (lua4_type(L, i))
		{
		case LUA_TSTRING:
			Core->print("\"%s\"", s);
			break;

		case LUA4_TINSTANCE:
			{
				lua_rawgeti(L, i, SERVICE_INSTANCE_INDEX);
				Service* s = static_cast<Service*>(lua_touserdata(L, -1));
				lua_pop(L, 1);
				Core->print("Instance (%s)", s->getTypeName());
			}
			break;

		case LUA4_TSERVICE:
			{
				lua_pushnumber(L, SERVICE_BINDINFO_INDEX);
				lua_gettable(L, i);
				ServiceInfo* s = static_cast<ServiceInfo*>(lua_touserdata(L, -1));
				lua_pop(L, 1);
				Core->print("Service (%s)", s->name);
			}
			break;

		case LUA4_TPACKAGE:
			Core->print("Package");
			break;

		case LUA4_TVECTOR:
			Core->print("Vector");
			break;

		case LUA4_TMATRIX:
			Core->print("Matrix");
			break;

		default:
			Core->print(s);
			break;
		}

		Core->print("\n");
	}
}

//---------------------------------------------------------------------------

void lua4_showtable(lua_State* L, int i)
{
	if (i < 0 && i > LUA_REGISTRYINDEX) --i;

	lua_pushnil(L);
	while (lua_next(L, i))
	{
		Core->print("[%s]=%s\n", lua4_asstring(L, -2), lua4_asstring(L, -1));
		lua_pop(L, 1);
	}
}



//---------------------------------------------------------------------------
// Lua tag methods
//---------------------------------------------------------------------------

/*
 * A replacement for the Lua 'type' command which handles my new types.
 */
int _type(lua_State* L)
{
	luaL_check_any(L, 1);
	
	int t = lua4_type(L, 1);

	if (lua_isnull(L, 2))
	{
		if (lua4_isinstance(L, 1))
		{
			lua_rawgeti(L, 1, SERVICE_INSTANCE_INDEX);
			Service* s = static_cast<Service*>(lua_touserdata(L, -1));
			lua_pushstring(L, s->getTypeName());
		}
		else
		{
			lua_pushstring(L, lua4_typename(L,t));
		}
	}
	else
	{
		if (lua4_isinstance(L, 1))
		{
			lua_rawgeti(L, 1, SERVICE_INSTANCE_INDEX);
			Service* s = static_cast<Service*>(lua_touserdata(L, -1));
			lua_pushboolean(L, s->isA(luaL_check_string(L, 2)));
		}
		else
		{
			lua_pushboolean(L, (strcmp(lua4_typename(L,t), luaL_check_string(L, 2)) == 0));
		}
	}

	return 1;
}

//---------------------------------------------------------------------------

/*
 * Hooked to "index" on the globals table.
 *
 * Tries to locate and load a service package with the given name. If no such
 * package exists, or if it could not be loaded, 'nil' is returned.
 */
int _loadPackage(lua_State* L)
{
	// Call the Core service to try and find the package. If a DLL is found,
	// this function will return a list of the services it contains.
	const char* packageName = lua_tostring(L, 2);
	ServicesList services;
	if (!Core->loadPackage(&services, packageName))
		return 0;

	// Create a table to represent the package in Lua. Store the name of the
	// package in the table (this will be used as a directory name to load
	// script services) as well as a pointer to the list of C++ services. Also 
	// set the event table for the package, so that it may be indexed to load 
	// the services it contains
	lua_newtable(L);

	lua_pushstring(L, packageName);
	lua_rawseti(L, -2, PACKAGE_NAME_INDEX);

	lua_newuserdatabox(L, services);
	lua_rawseti(L, -2, PACKAGE_SERVICES_INDEX);

	_loadRef(L, _packageEventTable);
	lua_seteventtable(L, -2);

	// Before returning, add this package to the globals table. This will
	// prevent "index" from getting called again for this same package.
	// The reference code keeps the table on the stack so I can return it.
	lua_pushvalue(L, -1);
	lua_setglobal(L, packageName);

	return 1;
}

//---------------------------------------------------------------------------

/*
 * Hooked to "index" on package tables.
 *
 * Tries to locate and load a service from within a package. If no such
 * service exists, or if it could not be created, 'nil' is returned.
 */
int _loadService(lua_State* L)
{
	// Get all the info: requested service name is on the stack, the package
	// name and list of C++ services are in the package table
	const char* serviceName = lua_tostring(L, 2);

	lua_rawgeti(L, 1, PACKAGE_NAME_INDEX);
	const char* packageName = lua_tostring(L, -1);

	lua_rawgeti(L, 1, PACKAGE_SERVICES_INDEX);
	ServicesList services = (ServicesList)lua_touserdata(L, -1);

	// Load the service. Look for a scripted version of the service first,
	// which is simply a file with the name "Package/Service.lua" which
	// returns a table of functions. If a scripted version can't be found,
	// then look in the list of C++ package exports for a matching name. 
	// If a name is found, load it's list of exported symbols into a table.
	// Either way I end up a table of symbols which represents the service.

	char scriptName[512];
	sprintf(scriptName, "*/%s/%s%s", packageName, serviceName, SERVICE_EXT);
	if (Core->runFile(scriptName, _asContext(L)))
	{
		// Script should have returned a service table
		if (!lua4_isservice(L, -1)) return 0;
	}
	else if (services)
	{
		for (int i = 0; services && services[i]; ++i)
		{
			if (!strcmp(services[i]->name, serviceName))
			{
				_wrapService(L, services[i]);
				break;
			}
		}
	}

	if (lua_gettop(L) > 4)
	{
		lua_settop(L, 5);

		// Store the service in the package table so "index" won't be called 
		// again the next time the service is needed
		lua_pushstring(L, serviceName);
		lua_pushvalue(L, -2);
		lua_settable(L, 1);

		return 1;
	}

	return 0;      // No service was found
}

//---------------------------------------------------------------------------

/*
 * Hooked to "function" on service tables.
 *
 * A factory function for Lua, analogous to CoreService::createA() in C++.
 * Creates a new instance of the referenced service.
 */
int _createInstance(lua_State* L)
{
	// Create an instance of the service's C++ interface
 	lua_pushnumber(L, SERVICE_BINDINFO_INDEX);
	lua_gettable(L, 1);

	ServiceInfo* info = static_cast<ServiceInfo*>(lua_touserdata(L, -1));
	Ref<Service> s = info->factory();
	lua_pop(L, 1);

	// Wrap the instance pointer in a Lua table and set it's metatable
	lua4_pushinstance(L, s);

	// pushinstance() points the instance at the C++ service table, replace
	// that with the service table I have here. If it's an instance of a C++
	// service it will be the same, but if it's a Lua service it won't be
	lua_pushvalue(L, 1);
	lua_rawseti(L, -2, SERVICE_EXPORTS_INDEX);

	// Call the initialize() method and done
	if (!_callInitialize(L, s))
		return 0;

	return 1;
}
 
//---------------------------------------------------------------------------

/*
 * Hooked to "gc" on service instances.
 *
 * Drops the script references stored by the instance and decrements the
 * service's reference count. Since the table can't be gc-ed unless Lua
 * holds the only reference, this will cause the instance to be destroyed.
 */
int _collectInstance(lua_State* L)
{
	Service* s = static_cast<Service*>(lua_touserdata(L, 1));

	int ref = (int)s->getScriptData();
	_dropRef(L, ref);
	s->setScriptData(0);

	s->release();
	return 0;
}

//---------------------------------------------------------------------------

/*
 * Hooked to "gettable" on service export tables.
 *
 * Adds the support code necessary to transparently read from a "property",
 * which is a get...() function that looks like a Lua variable.
 */
int _getItem(lua_State* L)
{
	// If the value already exists in the table, return it quick
	lua_pushvalue(L, 2);
	lua_rawget(L, 1);
	if (!lua_isnil(L, -1))
		return 1;

	// The export table only contains strings. If this isn't a string, then
	// it should be safe to say that it doesn't exist and I can return <nil>
	if (!lua_isstring(L, 2))
		return 1;

	// Does this item exist in the service table?
	lua_rawgeti(L, 1, SERVICE_EXPORTS_INDEX);
	lua_pushvalue(L, 2);
	lua_gettable(L, -2);
	if (!lua_isnil(L, -1)) return 1;
	lua_pop(L, 1);

	// Look for a readable property in the exports table
	char mangledName[128];
	strcpy(mangledName, "__r");
	strcat(mangledName, lua_tostring(L, 2));
	lua_pushstring(L, mangledName);
	
	lua_rawget(L, -2);
	if (lua_isnil(L, -1))
		return 1;

	// Found a property function, call it
	lua_CFunction func = lua_tocfunction(L, -1);
	lua_settop(L, 1);
	return func(L);
}

//---------------------------------------------------------------------------

/*
 * Hooked to "settable" on service export tables.
 *
 * Adds the support code necessary to transparently write to a "property",
 * which is a set...() function that looks like a Lua variable.
 */
int _setItem(lua_State* L)
{
	if (lua_isstring(L, 2))
	{
		// Look for a writable property
		char mangledName[128];
		strcpy(mangledName, "__w");
		strcat(mangledName, lua_tostring(L, 2));
		
		lua_rawgeti(L, 1, SERVICE_EXPORTS_INDEX);
		lua_pushstring(L, mangledName);
		lua_rawget(L, -2);

		if (!lua_isnil(L, -1))
		{
			// Property function found, rearrange the stack so I can call it
			lua_CFunction func = lua_tocfunction(L, -1);
			lua_settop(L, 3);
			lua_remove(L, 2);
			return func(L);
		}

		lua_pop(L, 2);
	}

	// No property function, set it directly
	lua_rawset(L, 1);
	return 0;
}

//---------------------------------------------------------------------------

/*
 * Hooked to the global "Service".
 *
 * Creates a new scripted service type.
 */
int _createNewType(lua_State* L)
{
	// Make sure that the base class is really a service description
	if (!lua4_isservice(L, 1))
	{
		lua_error(L, BAD_BASE_SRV);
		return 0;
	}
	
	// Create a table to represent the new type
	lua_newtable(L);

	// Create a metatable for the service. Calling the service table as a
	// function creates a new instance, all other references are defered
	// to the inherited service
	lua_newtable(L);
	lua_pushstring(L, "call");
	lua_pushcfunction(L, _createInstance);
	lua_settable(L, -3);
	lua_pushstring(L, "index");
	lua_pushvalue(L, 1);
	lua_settable(L, -3);
	lua_pushnumber(_L, LUA4_TSERVICE);
	lua_rawseti(_L, -2, EVENTTABLE_TYPE_INDEX);
	lua_seteventtable(L, -2);


	// Was a type name specified?
	if (lua_gettop(L) > 1 && lua_isstring(L, 2))
	{
		lua_pushvalue(L, 2);
		lua_rawseti(L, -2, SERVICE_TYPENAME_INDEX);
	}

	return 1;
}





//---------------------------------------------------------------------------
// Support functions
//---------------------------------------------------------------------------

/*
 * Create a Lua table populated with a service's exported symbols. Returns
 * the new table on the Lua stack.
 */
void _wrapService(lua_State* L, ServiceInfo* s)
{
	// If it's already wrapped
	if (s->scriptData)
	{
		_loadRef(L, (int)s->scriptData);
		return;
	}

	// Create a table containing all of the service exports
	lua_newtable(L);
	_loadExports(L, s->x);

	// Store the service description in the table as well. I'll need it to
	// create new instances of the service (it contains the factory function)
	lua_newuserdatabox(L, s);
	lua_rawseti(L, -2, SERVICE_BINDINFO_INDEX);

	// Attach the standard service metatable. This redirects the "call" tag 
	// to a factory function. Since the metatable also contains a type ID, I
	// have to choose the right one for this service type
	_loadRef(L, _serviceEventTable);
	lua_seteventtable(L, -2);

	// Finally, cache a reference to the table on the C++ side of the wall,
	// so I can get the table back when I need it
	lua_pushvalue(L, -1);
	s->scriptData = (void*)_makeStrongRef(L);
}

//---------------------------------------------------------------------------

/*
 * Loads a list of service exports into an event table. 
 */
void _loadExports(lua_State* L, ExportInfo* e)
{
	for (int i = 0; e[i].name; ++i)
	{
		switch (e[i].type)
		{
		case F4_X_INTERFACE:
			_loadExports(L, static_cast<ExportInfo*>(e[i].addr1));
			break;

		case F4_X_METHOD:
			lua_pushstring(L, e[i].name);
			lua_pushcfunction(L, (lua_CFunction)e[i].addr1);
			lua_settable(L, -3);
			break;

		case F4_X_CONST_INT:
			lua_pushstring(L, e[i].name);
			lua_pushnumber(L, (int)e[i].addr1);
			lua_settable(L, -3);
			break;

		case F4_X_PROP_BOOL:
		case F4_X_PROP_DOUBLE:
		case F4_X_PROP_FLOAT:
		case F4_X_PROP_INT:
		case F4_X_PROP_PTR:
		case F4_X_PROP_SERVICE:
		case F4_X_PROP_STRING:
		case F4_X_PROP_VECTOR:
			if (e[i].addr1)
			{
				lua_pushstring(L, "__r");
				lua_pushstring(L, e[i].name);
				lua_concat(L, 2);
				lua_pushcfunction(L, (lua_CFunction)e[i].addr1);
				lua_settable(L, -3);
			}
			if (e[i].addr2)
			{
				lua_pushstring(L, "__w");
				lua_pushstring(L, e[i].name);
				lua_concat(L, 2);
				lua_pushcfunction(L, (lua_CFunction)e[i].addr2);
				lua_settable(L, -3);
			}
			break;
			
		}
	}
}

//---------------------------------------------------------------------------

/*
 * Calls the initialize() method on a newly created instance. Expects the
 * service instance table to be at the top of the stack, and leaves the
 * stack the same way.
 */
bool _callInitialize(lua_State* L, Service* s)
{
	bool result = true;
	int numArgs = lua_gettop(L) - 2;

	// Look for an initialize method in the service table
	lua_pushstring(L, "initialize");
	lua_gettable(L, 1);

	// Always call initialize() on script instances, but only call C++ if
	// parameters have been supplied to the call
	if (lua_isfunction(L, -1) || (lua_iscfunction(L, -1) && numArgs > 0))
	{
		// Set up the stack to make the call
		lua_pushvalue(L, -2);
		for (int i = 2; i < numArgs + 2; ++i)
			lua_pushvalue(L, i);
	
		lua_call(L, numArgs + 1, 1);
		result = (lua_toboolean(L, -1) != 0);
	}

	lua_pop(L, 1);
	return result;
}
