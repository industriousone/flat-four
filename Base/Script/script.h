/**
 * @file script.h
 *
 * @author  Jason Perkins (jason@379.com)
 * @date    Copyright (C) 2001 by 379, Inc.
 * @version $Id: script.h,v 1.4 2002/05/01 01:25:22 jason379 Exp $
 *
 * @todo Add hooks for an external interactive debugger service.
 *
 */
#if !defined(_F4_SCRIPT_H_)
#define _F4_SCRIPT_H_

ENTER_NAMESPACE(f4);


/**
 * A generic interface to the scripting environment. I've tried to keep the
 * interface as implementation neutral as possible to allow people to try
 * out different scripting languages.
 *
 * This interface is intended only to expose the scripting environment to
 * the Core services. With very few exceptions, you should not be calling this
 * interface directly. Rather, use the calls available in CoreService instead.
 *
 * @ingroup f4_script
 */
struct ScriptService
{

	/**
	 * Retrieve the default scripting context.
	 */
	virtual Context* getDefaultContext() = 0;


	/**
	 * Retrieve the value of a global variable.
	 */
	virtual const Char* getGlobal(const Char* name) = 0;


	/**
	 * Execute script code contained in a memory buffer.
	 *
	 * @param buffer  The memory containing the script code.
	 * @param size    The size of the script code, in bytes.
	 * @param name    A name for the buffer which is shown if errors occur.
	 * @param context A script execution context (optional).
	 *
	 * @return True if the code ran successfully, false on errors.
	 */
	virtual bool runBuffer(void* buffer, unsigned size, const Char* name, Context* context = 0) = 0;


	/**
	 * Locate a service and create a new instance.
	 */
	virtual Service* createA(const Char* typeName) = 0;


	/**
	 * Return the type name of a scripted service. Returns @c NULL if no
	 * type name is available.
	 */
	virtual const Char* getTypeName(Service* s) = 0;


	/**
	 * Determine if a scripted service implements a particular type.
	 */
	virtual bool isA(Service* s, const Char* type) = 0;


	/**
	 * Prevent this service instance from being garbage collected.
	 */
	virtual void reference(Service* s, void* data) = 0;


	/**
	 * Allow this instance to be garbage collected (when the script system
	 * is done with it).
	 */
	virtual void release(Service* s, void* data) = 0;


	/**
	 * Destroy this instance, called as part of the application exit code.
	 */
	virtual void destroy() = 0;

};


EXIT_NAMESPACE;
#endif

