/**
 * @file bindings.h
 *
 * @author  Jason Perkins (jason@379.com)
 * @date    Copyright (C) 2001 by 379, Inc.
 * @version $Id: bindings.h,v 1.3 2002/06/02 18:55:02 jason379 Exp $
 */
#if !defined(_F4_CORE_BINDINGS_H_)
#define _F4_CORE_BINDINGS_H_

ENTER_NAMESPACE(f4);

struct Service;


/**
 * The types of exports supported by the binding system.
 * @ingroup f4_core
 */
enum ExportType {
	F4_X_INTERFACE,        ///< An interface referencing a sublist of exports.
	F4_X_METHOD,           ///< A class method.
	F4_X_CONST_INT,        ///< A constant integer value
	F4_X_PROP_BOOL,        ///< A boolean property
	F4_X_PROP_DOUBLE,      ///< A double precision numeric property
	F4_X_PROP_FLOAT,       ///< A single precision numeric property
	F4_X_PROP_INT,         ///< An integer property
	F4_X_PROP_PTR,         ///< A C pointer property
	F4_X_PROP_SERVICE,     ///< A service instance property
	F4_X_PROP_STRING,      ///< A string property
	F4_X_PROP_VECTOR       ///< A vector property
};



/**
 * Information about a service entry point.
 * @ingroup f4_core
 */
struct ExportInfo
{
	const char* name;      ///< The name of the entry point
	ExportType  type;      ///< INTERFACE, METHOD, etc.
	void*       addr1;     ///< The memory address of the entry point.
	void*       addr2;     ///< A second address, used by properties
};




/**
 * Information about a service contained in a package.
 * @ingroup f4_core
 */
struct ServiceInfo
{
	const char* name;      ///< The name of the service.
	Service* (*factory)(); ///< The service's factory function.
	ExportInfo* x;         ///< A list entry points.
	void* scriptData;      ///< Scripting description of service
};




/**
 * A list of services, used to return the contents of a DLL package.
 * @ingroup f4_core
 */
typedef ServiceInfo** ServicesList;



//---------------------------------------------------------------------------
// Macros take the busy work out of using the binding objects
//---------------------------------------------------------------------------

#define DECL_SERVICE \
			virtual ServiceInfo* getTypeInfo() const; \
			virtual const Char*  getTypeName();

#define IMPL_SERVICE(I,S) \
			static Service* _f4factory_##S() { return new I; } \
			ServiceInfo _f4service_##S = { #S, _f4factory_##S, _f4exports_##S, 0 }; \
            ServiceInfo* I::getTypeInfo() const { return &_f4service_##S; } \
            const Char* I::getTypeName() { return _f4service_##S.name; }

#define IMPL_ANON_SERVICE(I,S) \
            EXPORTS(I) INTERFACE(S) END_EXPORTS \
			static Service* _f4factory_##I() { return new I; } \
			ServiceInfo _f4service_##I = { #I, _f4factory_##I, _f4exports_##I, 0 }; \
            ServiceInfo* I::getTypeInfo() const { return &_f4service_##I; } \
            const Char* I::getTypeName() { return _f4service_##I.name; }

#define IMPL_SINGLETON_SERVICE(I,S) \
			static Service* _f4factory_##S() { static Service* s=0; if (!s) s=new I; return s; } \
			ServiceInfo _f4service_##S = { #S, _f4factory_##S, _f4exports_##S, 0 } ; \
            ServiceInfo* I::getTypeInfo() const { return &_f4service_##S; } \
            const Char* I::getTypeName() { return _f4service_##S.name; }

#define DECLARE_SERVICE(S) \
			extern ServiceInfo _f4service_##S;

#define CREATEA_LOCAL(S) \
			(S*)_f4service_##S.factory();


#define PACKING_LIST       F4_EXPORT f4::ServiceInfo** _f4_packlist() { static f4::ServiceInfo* p[] = {
#define END_PACKING_LIST   0}; return p; }
#define SERVICE(S)         &_f4service_##S,

#define EXPORTS(S)         static f4::ExportInfo _f4exports_##S[] = {
#define END_EXPORTS        {0}};

#define INTERFACE(S)            { #S, F4_X_INTERFACE, (void*)_f4exports_##S },
#define METHOD(S,M)             { #M, F4_X_METHOD, (void*)_f4export_##S##_##M, 0 },
#define FUNCTION(S,F)           { #F, F4_X_METHOD, (void*)_f4export_##S##_##F, 0 },
#define CONST_INT(S,C)          { #C, F4_X_CONST_INT, (void*)S::C, 0 },

#define RO_PROP_BOOL(S,P,R)     { #P, F4_X_PROP_BOOL, (void*)_f4export_##S##_##R, 0 },
#define WO_PROP_BOOL(S,P,W)     { #P, F4_X_PROP_BOOL, 0, (void*)_f4export_##S##_##W },
#define RW_PROP_BOOL(S,P,R,W)   { #P, F4_X_PROP_BOOL, (void*)_f4export_##S##_##R, (void*)_f4export_##S##_##W },

#define RO_PROP_DOUBLE(S,P,R)   { #P, F4_X_PROP_DOUBLE, (void*)_f4export_##S##_##R, 0 },
#define WO_PROP_DOUBLE(S,P,W)   { #P, F4_X_PROP_DOUBLE, 0, (void*)_f4export_##S##_##W },
#define RW_PROP_DOUBLE(S,P,R,W) { #P, F4_X_PROP_DOUBLE, (void*)_f4export_##S##_##R, (void*)_f4export_##S##_##W },

#define RO_PROP_FLOAT(S,P,R)    { #P, F4_X_PROP_FLOAT, (void*)_f4export_##S##_##R, 0 },
#define WO_PROP_FLOAT(S,P,W)    { #P, F4_X_PROP_FLOAT, 0, (void*)_f4export_##S##_##W },
#define RW_PROP_FLOAT(S,P,R,W)  { #P, F4_X_PROP_FLOAT, (void*)_f4export_##S##_##R, (void*)_f4export_##S##_##W },

#define RO_PROP_INT(S,P,R)      { #P, F4_X_PROP_INT, (void*)_f4export_##S##_##R, 0 },
#define WO_PROP_INT(S,P,W)      { #P, F4_X_PROP_INT, 0, (void*)_f4export_##S##_##W },
#define RW_PROP_INT(S,P,R,W)    { #P, F4_X_PROP_INT, (void*)_f4export_##S##_##R, (void*)_f4export_##S##_##W },

#define RO_PROP_PTR(S,P,R)      { #P, F4_X_PROP_PTR, (void*)_f4export_##S##_##R, 0 },
#define WO_PROP_PTR(S,P,W)      { #P, F4_X_PROP_PTR, 0, (void*)_f4export_##S##_##W },
#define RW_PROP_PTR(S,P,R,W)    { #P, F4_X_PROP_PTR, (void*)_f4export_##S##_##R, (void*)_f4export_##S##_##W },

#define RO_PROP_SERVICE(S,P,R)   { #P, F4_X_PROP_SERVICE, (void*)_f4export_##S##_##R, 0 },
#define WO_PROP_SERVICE(S,P,W)   { #P, F4_X_PROP_SERVICE, 0, (void*)_f4export_##S##_##W },
#define RW_PROP_SERVICE(S,P,R,W) { #P, F4_X_PROP_SERVICE, (void*)_f4export_##S##_##R, (void*)_f4export_##S##_##W },

#define RO_PROP_STRING(S,P,R)   { #P, F4_X_PROP_STRING, (void*)_f4export_##S##_##R, 0 },
#define WO_PROP_STRING(S,P,W)   { #P, F4_X_PROP_STRING, 0, (void*)_f4export_##S##_##W },
#define RW_PROP_STRING(S,P,R,W) { #P, F4_X_PROP_STRING, (void*)_f4export_##S##_##R, (void*)_f4export_##S##_##W },

#define RO_PROP_VECTOR(S,P,R)   { #P, F4_X_PROP_VECTOR, (void*)_f4export_##S##_##R, 0 },
#define WO_PROP_VECTOR(S,P,W)   { #P, F4_X_PROP_VECTOR, 0, (void*)_f4export_##S##_##W },
#define RW_PROP_VECTOR(S,P,R,W) { #P, F4_X_PROP_VECTOR, (void*)_f4export_##S##_##R, (void*)_f4export_##S##_##W },



//---------------------------------------------------------------------------
// Macros for C++/script integration
//
// IMPORTANT: The SCRIPT_METHOD argument extraction code relies on the fact
// that the compiler evaluates a function's arguments in REVERSE order. So
// far this seems okay, but the code will need to be tweaked slightly if I
// find a compiler that uses the direct ordering.
//---------------------------------------------------------------------------

#define SCRIPT_METHOD(S,M,R) \
			static int _f4export_##S##_##M(Context** c) { int i; \
			S* T = (S*)(*c)->beginCall(#S, &i); \
			R T->M(

#define END_METHOD      )); return 1; }

#define SCRIPT_FUNCTION_NAMED(S,F,N,R) \
			static int _f4export_##S##_##N(Context** c) { int i; \
			(*c)->beginCall(#S, &i); \
			R (F(

#define END_FUNCTION    )); return 1; }

#define RETURNS_BOOL    (*c)->pushBool(
#define RETURNS_DOUBLE  (*c)->pushDouble(
#define RETURNS_FLOAT   (*c)->pushFloat(
#define RETURNS_INT     (*c)->pushInt(
#define RETURNS_MATRIX  (*c)->pushMatrix(
#define RETURNS_PTR     (*c)->pushPointer(
#define RETURNS_QUAT    (*c)->pushQuaternion(
#define RETURNS_SERVICE (*c)->pushService(
#define RETURNS_STRING  (*c)->pushString(
#define RETURNS_VECTOR  (*c)->pushVector(
#define RETURNS_VOID    (*c)->pushNull(); (

#define ARG_BOOL        (*c)->asBool(--i)
#define ARG_DOUBLE      (*c)->asDouble(--i)
#define ARG_ENUM(T)     (T)(*c)->asInt(--i)
#define ARG_FLOAT       (*c)->asFloat(--i)
#define ARG_INT         (*c)->asInt(--i)
#define ARG_MATRIX      (*c)->asMatrix(--i)
#define ARG_PTR(T)      (T*)(*c)->asPointer(--i)
#define ARG_QUAT        (*c)->asQuaternion(--i)
#define ARG_SERVICE(T)  (T*)(*c)->asService(--i, #T)
#define ARG_STRING      (*c)->asString(--i)
#define ARG_VECTOR      (*c)->asVector(--i)

#define DISPATCH_METHOD(M,C) \
	Context* _c = Core->beginDispatch(C, this, M); \
	if (_c) { \
	
#define DISPATCH_ARG_BOOL(x)        _c->pushBool(x);
#define DISPATCH_ARG_DOUBLE(x)      _c->pushDouble(x);
#define DISPATCH_ARG_FLOAT(x)		_c->pushFloat(x);
#define DISPATCH_ARG_INT(x)         _c->pushInt(x);
#define DISPATCH_ARG_MATRIX(x)      _c->pushMatrix(x);
#define DISPATCH_ARG_SERVICE(x)		_c->pushService(x);
#define DISPATCH_ARG_STRING(x)      _c->pushString(x);
#define DISPATCH_ARG_VECTOR(x)      _c->pushVector(x);

#define DISPATCH_RETURNS_BOOL(x)    _c->dispatch(); x = _c->asBool(0);      _c->endDispatch(); }
#define DISPATCH_RETURNS_DOUBLE(x)  _c->dispatch(); x = _c->asDouble(0);    _c->endDispatch(); }
#define DISPATCH_RETURNS_INT(x)     _c->dispatch(); x = _c->asInt(0);       _c->endDispatch(); }
#define DISPATCH_RETURNS_MATRIX(c)  _c->dispatch(); x = _c->asMatrix(0);    _c->endDispatch(); }
#define DISPATCH_RETURNS_SERVICE(x) _c->dispatch(); x = _c->asService(0,0); _c->endDispatch(); }
#define DISPATCH_RETURNS_STRING(x)  _c->dispatch(); x = _c->asString(0);    _c->endDispatch(); }
#define DISPATCH_RETURNS_VECTOR(x)  _c->dispatch(); x = _c->asVector(0);    _c->endDispatch(); }
#define DISPATCH_RETURNS_VOID		_c->dispatch(); _c->endDispatch(); }


EXIT_NAMESPACE;
#endif
