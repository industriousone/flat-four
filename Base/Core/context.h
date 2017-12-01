/**
 * @file context.h
 *
 * @author  Jason Perkins (jason@379.com)
 * @date    Copyright (C) 2001 by 379, Inc.
 * @version $Id: context.h,v 1.2 2002/04/22 01:51:55 jason379 Exp $
 */
#if !defined(_F4_CONTEXT_H_)
#define _F4_CONTEXT_H_

#include <string>
ENTER_NAMESPACE(f4);

struct Matrix44;
struct Quaternion;
struct Vector3;


/**
 * The context manages a script execution environment. Each context 
 * represents a thread of execution, allowing multiple scripts to appear
 * to run simultaneously.
 *
 * @ingroup f4_core
 */
struct Context
{

	/**
	 * Prepare the context to retrieve function call arguments. Returns
	 * the target service object (the "this" pointer).
	 */
	virtual Service* beginCall(const Char* expectedType, int* args) = 0;


	/**
	 * Prepare for a call to a script service method.
	 */
	virtual void beginDispatch(Service* s, const Char* method) = 0;


	/**
	 * Make a call to a script service method.
	 */
	virtual void dispatch() = 0;


	/**
	 * Clean up after a script service method call.
	 */
	virtual void endDispatch() = 0;


	/**
	 * Retrieve a boolean value from the call stack.
	 */
	virtual bool asBool(int pos) = 0;


	/**
	 * Retrieve a double precision number from the call stack.
	 */
	virtual Double asDouble(int pos) = 0;


	/**
	 * Retrieve a floating point number from the call stack.
	 */
	virtual Float asFloat(int pos) = 0;


	/**
	 * Retrieve an integer number from the call stack.
	 */
	virtual int asInt(int pos) = 0;


	/**
	 * Retrieve a Matrix44 from the call stack.
	 */
	virtual Matrix44& asMatrix(int pos) = 0;


	/**
	 * Retrieves a C pointer from the call stack.
	 */
	virtual void* asPointer(int pos) = 0;


	/**
	 * Retrieve a quaternion from the call stack.
	 */
	virtual Quaternion& asQuaternion(int pos) = 0;


	/**
	 * Retrieve a service instance from the call stack.
	 */
	virtual Service* asService(int pos, const Char* expectedType) = 0;


	/**
	 * Retrieve a string value from the call stack.
	 */
	virtual const Char* asString(int pos) = 0;


	/**
	 * Retrieve a Vector3 from the call stack.
	 */
	virtual Vector3& asVector(int pos) = 0;


	/**
	 * Push a boolean value onto the call stack.
	 */
	virtual void pushBool(bool b) = 0;


	/**
	 * Push a double precision number onto the call stack.
	 */
	virtual void pushDouble(Double d) = 0;


	/**
	 * Push a floating point value onto the call stack.
	 */
	virtual void pushFloat(Float f) = 0;


	/**
	 * Push an integer onto the call stack.
	 */
	virtual void pushInt(int i) = 0;


	/**
	 * Push a Matrix44 onto the call stack.
	 */
	virtual void pushMatrix(const Matrix44& m) = 0;


	/**
	 * Push a NULL value onto the call stack.
	 */
	virtual void pushNull() = 0;


	/**
	 * Push a C pointer onto the call stack.
	 */
	virtual void pushPointer(void* p) = 0;


	/**
	 * Push a quaternion onto the call stack.
	 */
	virtual void pushQuaternion(const Quaternion& q) = 0;


	/**
	 * Push a service instance onto the call stack.
	 */
	virtual void pushService(Service* s) = 0;


	/**
	 * Push a C string onto the call stack.
	 */
	virtual void pushString(const Char* s) = 0;


	/**
	 * Push the contents of an STL string onto the call stack.
	 */
	virtual void pushString(const std::string& x) = 0;

	
	/**
	 * Push a Vector3 onto the call stack.
	 */
	virtual void pushVector(const Vector3& v) = 0;


	/**
	 * Dump the contents of the context to the execution log (for debugging).
	 */
	virtual void printContents() = 0;

};


EXIT_NAMESPACE;
#endif
