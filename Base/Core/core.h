/**
 * @file core.h
 *
 * @author  Jason Perkins (jason@379.com)
 * @date    Copyright (C) 2001 by 379, Inc.
 * @version $Id: core.h,v 1.5 2002/06/19 01:40:08 jason379 Exp $
 *
 * @todo Can I get rid of this service entirely? Supply a few C
 *       functions in a DLL as a component system instead, move
 *       startup code into app? Need message queue somewhere.
 *
 */
#if !defined(_F4_CORE_H_)
#define _F4_CORE_H_

#if !defined(CORE_API)
#define CORE_API F4_IMPORT
#endif

#include <Platform/platform.h>
#include "bindings.h"
#include "context.h"
#include "service.h"
#include "ref.h"
ENTER_NAMESPACE(f4);

struct CoreFileServer;
struct Event;
struct EventListener;
struct EventLoop;
struct ScriptService;


/**
 * Script and service management.
 * @ingroup f4_core
 */
struct CoreService : public Service
{

	/**
	 * Initialize the core service and bring the major subsystems online.
	 * Called by the shell to bootstrap the engine on startup.
	 */
	virtual bool startEngine(int argc, char* argv[]) = 0;


	/**
	 * Shut down all engine subsystems.
	 */
	virtual void stopEngine() = 0;


	/**
	 * Start the event loop. An event loop implementation must have been
	 * registered via registerEventLoop() or this function will return
	 * immediately.
	 */
	virtual void run() = 0;


	/**
	 * Quit out of the event loop.
	 */
	virtual void quit() = 0;


	/**
	 * Add a message to the event queue and returns immediately.
	 */
	virtual void queueEvent(Event* e) = 0;


	/**
	 * Dispatch an event and wait for processing to finish before returning.
	 */
	virtual void sendEvent(Event* e) = 0;


	/**
	 * Schedule an event to be dispatched in the future.
	 *
	 * @param when   How long to wait before sending the message, in seconds.
	 * @param who    A listener to receive the event. If @c NULL, the event
	 *               will be sent to all listeners.
	 * @param what   The event to send.
	 */
	virtual void scheduleEvent(Double when, EventListener* who, Event* what) = 0;


	/**
	 * Write a message to the execution log file.
	 */
	virtual void print(const Char* msg, ...) = 0;


	/**
	 * Create a new instance of a service.
	 *
	 * @param typeName  The service's unique ID, which looks like 
	 *                  <CODE>"package.service"</CODE>.
	 */
	virtual Service* createA(const Char* typeName) = 0;


	/**
	 * Run a script file.
	 *
	 * @param filename The name of the script file to run.
	 * @param context  The context in which the script should be run. If this
	 *                 is @c NULL the global context will be used.
	 *
	 * @return True if the script was executed successfully, false if the file
	 *         does not exist. 
	 */
	virtual bool runFile(const Char* filename, Context* context = 0) = 0;


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
	virtual void runBuffer(void* buffer, unsigned size, const Char* name, Context* context = 0) = 0;


	/**
	 * Report a potentially recoverable error condition. Intended for use by
	 * the scripting system, which calls this function to report syntax and
	 * similar errors. Triggers the onError() event on all registered listeners
	 * (see addListener(), below).
	 */
	virtual void error(const Char* msg) = 0;


	/**
	 * Report an unrecoverable error condition. Displays the supplied message
	 * and then exits the application, performing as much cleanup as possible
	 * on the way out.
	 */
	virtual void die(const Char* msg) = 0;


	/**
	 * Get the directory which was current when the application started.
	 */
	virtual const Char* getStartDirectory() = 0;


	/**
	 * Get the directory containing the application executable.
	 */
	virtual const Char* getBinDirectory() = 0;


	/**
	 * Get the name of the application executable.
	 */
	virtual const Char* getExecutableName() = 0;


	/**
	 * Look for and load a package. This is called by the script service when
	 * a new package is requested. 
	 *
	 * @param services  If a DLL package is found, this returns a pointer to
	 *                  the list of services that it contains. Returns NULL 
	 *                  if a DLL package was not found.
	 * @param name      The name of the package to load.
	 *
	 * @return True if a package, either a DLL or a directory, was found.
	 */
	virtual bool loadPackage(ServicesList* services, const char* name) = 0;


	/**
	 * Register the application event loop.
	 */
	virtual void registerEventLoop(EventLoop* el) = 0;


	/**
	 * Add a new event listener.
	 */
	virtual void addEventListener(EventListener* el) = 0;


	/**
	 * Remove an event listener.
	 */
	virtual void removeEventListener(EventListener* el) = 0;


	/**
	 * Register a file service, to be used by runFile() to locate the 
	 * requested script.
	 */
	virtual ScriptService* registerFileServer(CoreFileServer* fs) = 0;


	/**
	 * Retrieve the scripting service interface.
	 */
	virtual ScriptService* getScriptService() = 0;


	/**
	 * Prepare a scripting context for a function call. You should generally 
	 * use the DISPATCH_METHOD() macro instead.
	 */
	virtual Context* beginDispatch(Context* c, Service* s, const Char* method) = 0;


};


// Unlike other F4 services, the Core service is linked directly into all the
// other packages. Since just about every service will need access to Core,
// this saves all of the code needed to manage those pointers.
CORE_API CoreService* Core;


// --- begin scripting interface ---

SCRIPT_METHOD(CoreService, addEventListener, RETURNS_VOID)
	ARG_SERVICE(EventListener)
END_METHOD

SCRIPT_METHOD(CoreService, getBinDirectory, RETURNS_STRING)
END_METHOD

SCRIPT_METHOD(CoreService, getExecutableName, RETURNS_STRING)
END_METHOD

SCRIPT_METHOD(CoreService, getStartDirectory, RETURNS_STRING)
END_METHOD

SCRIPT_METHOD(CoreService, queueEvent, RETURNS_VOID)
	ARG_SERVICE(Event)
END_METHOD

SCRIPT_METHOD(CoreService, quit, RETURNS_VOID)
END_METHOD

SCRIPT_METHOD(CoreService, removeEventListener, RETURNS_VOID)
	ARG_SERVICE(EventListener)
END_METHOD

SCRIPT_METHOD(CoreService, scheduleEvent, RETURNS_VOID)
	ARG_DOUBLE, ARG_SERVICE(EventListener), ARG_SERVICE(Event)
END_METHOD

SCRIPT_METHOD(CoreService, sendEvent, RETURNS_VOID)
	ARG_SERVICE(Event)
END_METHOD

EXPORTS(CoreService)
	METHOD(CoreService, addEventListener)
	METHOD(CoreService, getBinDirectory)
	METHOD(CoreService, getExecutableName)
	METHOD(CoreService, getStartDirectory)
	METHOD(CoreService, queueEvent)
	METHOD(CoreService, quit)
	METHOD(CoreService, removeEventListener)
	METHOD(CoreService, scheduleEvent)
	METHOD(CoreService, sendEvent)
END_EXPORTS

// --- end scripting interface ---


EXIT_NAMESPACE;
#endif

