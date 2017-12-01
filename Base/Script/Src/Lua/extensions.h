/**
 * @file extensions.h
 *
 * @author  Jason Perkins (jason@379.com)
 * @date    Copyright (C) 2001 by 379, Inc.
 * @version $Id: extensions.h,v 1.3 2002/04/22 20:21:28 jason379 Exp $
 */
#if !defined(_F4_SCRIPT_LUA_EXTENSIONS_H_)
#define _F4_SCRIPT_LUA_EXTENSIONS_H_

// The main Lua state
extern lua_State* _L;     


// new IDs for my custom types
static const int LUA4_TPACKAGE          = -10;
static const int LUA4_TSERVICE          = -11;
static const int LUA4_TINSTANCE         = -12;
static const int LUA4_TVECTOR           = -13;
static const int LUA4_TMATRIX           = -14;
static const int LUA4_TQUAT             = -15;

// indices into "special" tables
static const int EVENTTABLE_TYPE_INDEX  = 0;
static const int PACKAGE_NAME_INDEX     = 0;
static const int PACKAGE_SERVICES_INDEX = 1;
static const int SERVICE_BINDINFO_INDEX = 0;
static const int SERVICE_EXPORTS_INDEX  = 1;
static const int SERVICE_TYPENAME_INDEX = 2;
static const int SERVICE_INSTANCE_INDEX = 0;


void         lua4_init();
void         lua4_term();
int          lua4_type(lua_State* L, int i);
const char*  lua4_typename(lua_State* L, int t);
void         lua4_check_type(lua_State* L, int i, int t);
f4::Service* lua4_newinstance(lua_State* L, int i);
f4::Service* lua4_toinstance(lua_State* L, int i);
void         lua4_pushinstance(lua_State* L, f4::Service* s);
bool         lua4_isinstance(lua_State* L, int i);
bool         lua4_isservice(lua_State* L, int i);
void         lua4_showstack(lua_State* L);
void         lua4_showtable(lua_State* L, int i);



// From refs.cpp:
void _initRefs(lua_State* L);
int  _makeStrongRef(lua_State* L);
int  _makeWeakRef(lua_State* L);
void _dropRef(lua_State* L, int ref);
void _loadRef(lua_State* L, int ref);



// From context.cpp:
f4::Context* _createContext(lua_State* L);
void         _destroyContext(lua_State* L);
lua_State*   _asLuaState(f4::Context* c);
f4::Context* _asContext(lua_State* L);



// From data type sources (vector.cpp, matrix.cpp, etc.)
void _initVector(lua_State* L);
int  _vec_constructor(lua_State* L);
f4::Vector3* _vec_create(lua_State* L);

void _initMatrix(lua_State* L);
int  _mat_constructor(lua_State* L);
f4::Matrix44* _mat_create(lua_State* L);

void _initQuat(lua_State* L);
int  _quat_constructor(lua_State* L);
f4::Quaternion* _quat_create(lua_State* L);


#endif

