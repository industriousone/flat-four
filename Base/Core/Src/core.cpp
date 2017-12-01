/**
 * @file core.cpp
 *
 * @author  Jason Perkins (jason@379.com)
 * @date    Copyright (C) 2001 by 379, Inc.
 * @version $Id: core.cpp,v 1.4 2002/05/01 01:24:55 jason379 Exp $
 */
#define SHOW_DLERROR 0        // used to debug linux builds

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <deque>
#include <list>
#include <vector>

#include <Core/core.h>
#include <Core/event_listener.h>
#include <Core/event_loop.h>
#include <Core/file_server.h>
#include <Script/script.h>
#include <Platform/directory.h>
#include <Platform/file.h>
#include <Platform/library.h>
#include <Platform/print_debug.h>
#include <Platform/print_alert.h>
#include <Memory/memory.h>
using namespace f4;


struct CoreImpl : public CoreService
{
	CoreImpl();
	virtual ~CoreImpl();

	virtual bool startEngine(int argc, char* argv[]);
	virtual void stopEngine();

	virtual Service* createA(const Char* typeName);

	virtual void run();
	virtual void quit();
	virtual void queueEvent(Event* e);
	virtual void sendEvent(Event* e);
	virtual void scheduleEvent(Double when, EventListener* who, Event* what);

	virtual void error(const Char* msg);
	virtual void die(const Char* msg);

	virtual void print(const Char* msg, ...);

	virtual bool runFile(const Char* filename, Context* context);
	virtual void runBuffer(void* buffer, unsigned size, const Char* name, Context* context);

	virtual const Char* getBinDirectory();
	virtual const Char* getStartDirectory();
	virtual const Char* getExecutableName();

	virtual ScriptService* getScriptService();

	virtual bool loadPackage(ServicesList* services, const char* name);

	virtual void registerEventLoop(EventLoop* el);
	virtual void addEventListener(EventListener* el);
	virtual void removeEventListener(EventListener* el);

	virtual ScriptService* registerFileServer(CoreFileServer* fs);

	virtual Context* beginDispatch(Context* c, Service* s, const Char* method);

private:
	// Execution environment
	char* m_exeName;
	char* m_binDir;
	char* m_startDir;

	// Event Loop stuff
	Ref<EventLoop> m_eventLoop;
	bool m_running;

	typedef std::vector<EventListener*> EventListeners;
	EventListeners m_eventListeners;
	void _dispatch(Event* e);

	struct ScheduledEvent
	{
		Double when;
		Ref<EventListener> who;
		Ref<Event> what;
	};
	typedef std::list<ScheduledEvent> Scheduled;
	Scheduled m_scheduled;

	Double m_systemTime;

	// Hook for extending the basic file services
	CoreFileServer* m_fileServer;

	DECL_SERVICE;
};

IMPL_SINGLETON_SERVICE(CoreImpl, CoreService)


//---------------------------------------------------------------------------

CoreService* f4::Core = 0;
F4_EXPORT CoreService* createCoreService()
{
	return CREATEA_LOCAL(CoreService);
}



//---------------------------------------------------------------------------
// Local data
//---------------------------------------------------------------------------

ScriptService* _script = 0;     // script interface shared with service.cpp
static FILE*   _log    = 0;     // the log file stream

static void _print(const char* msg);



//---------------------------------------------------------------------------
// Message text
//---------------------------------------------------------------------------

static const Char* NO_SCRIPT = "Unable to load the script service.";
static const Char* STARTUP   = "Application started at %s\n";
static const Char* FATAL_ERR = "Fatal Error";


//---------------------------------------------------------------------------

CoreImpl::CoreImpl()
: m_exeName(0),
  m_binDir(0), m_startDir(0),
  m_systemTime(0),
  m_running(false),
  m_fileServer(0)
{
	Core = this;
	reference();
	setMemoryPrintFunc(_print);
}

//---------------------------------------------------------------------------

CoreImpl::~CoreImpl()
{
	if (m_exeName)  free(m_exeName);
	if (m_binDir)   free(m_binDir);
	if (m_startDir) free(m_startDir);
}

//---------------------------------------------------------------------------

bool CoreImpl::startEngine(int argc, char* argv[])
{
	// Grab some information about me and my environment: who I am, where
	// I am, and where I was started from. When this is done, the engine
	// BIN directory will be the current working directory
	char exeName[64], binDir[1024], curDir[1024];

	getCurrentDirectory(curDir, 1024);
	getExecutableDirectory(argv[0], binDir, 1024);

	makeGenericPath(binDir);
	makeGenericPath(curDir);

	splitPath(argv[0], BASENAME, exeName, 64);

	m_binDir = (char*)malloc(strlen(binDir) + 1);
	strcpy(m_binDir, binDir);

	m_startDir = (char*)malloc(strlen(curDir) + 1);
	strcpy(m_startDir, curDir);

	m_exeName = (char*)malloc(strlen(exeName) + 1);
	strcpy(m_exeName, exeName);

	changeDirectory(binDir);


	// Start a new message log
	strcat(exeName, ".log");
	_log = fopen(exeName, "wt");

	time_t timestamp;
	time(&timestamp);

	print(STARTUP, ctime(&timestamp));


	// Bootstrap the scripting engine
	SharedLibrary hDll = openLibrary("Script");
	if (!hDll) die(NO_SCRIPT);

	ScriptService* (*createScriptService)();
	bindSymbol((void**)&createScriptService, hDll, "createScriptService");
	if (!createScriptService) die(NO_SCRIPT);

	_script = createScriptService();
	if (!_script) die(NO_SCRIPT);


	// core.cfg configures the engine for this particular system
	runFile("core.cfg", 0);


	// Restore the original working directory
	changeDirectory(curDir);

	return true;
}

//---------------------------------------------------------------------------

void CoreImpl::stopEngine()
{
	if (_script) _script->destroy();
	delete static_cast<CoreImpl*>(Core);
	Core = 0;

	printMemoryReport();

	// Shut down the log file - in debug builds messages will continue to
	// be logged to the debug output window or console.
	fclose(_log);
	_log = 0;
}

//---------------------------------------------------------------------------

void CoreImpl::run()
{
	// Can't run without an event loop
	if (!m_eventLoop) return;

	Ref<Event> e;
	Scheduled::iterator i;

	m_running = true;
	while (m_running)
	{
		e = m_eventLoop->getEvent();
		if (e)
		{
			m_systemTime = e->getTimestamp();
			for (i = m_scheduled.begin(); i != m_scheduled.end() && (*i).when < m_systemTime; ++i)
			{
				(*i).what->setTimestamp(m_systemTime);
				if ((*i).who)
					(*i).who->receive((*i).what);
				else
					_dispatch((*i).what);

				++i;
				m_scheduled.pop_front();
			}

			_dispatch(e);
		}
	}
}

//---------------------------------------------------------------------------

void CoreImpl::quit()
{
	m_running = false;
}

//---------------------------------------------------------------------------

void CoreImpl::queueEvent(Event* e)
{
	scheduleEvent(0, 0, e);
}

//---------------------------------------------------------------------------

void CoreImpl::sendEvent(Event* e)
{
	e->setTimestamp(m_systemTime);
	_dispatch(e);
}

//---------------------------------------------------------------------------

void CoreImpl::scheduleEvent(Double when, EventListener* who, Event* what)
{
	when += m_systemTime;

	Scheduled::iterator i;
	for (i = m_scheduled.begin(); i != m_scheduled.end(); ++i)
		if ((*i).when > when) break;

	ScheduledEvent e;
	i = m_scheduled.insert(i, e);
	(*i).when = when;
	(*i).who  = who;
	(*i).what = what;
}

//---------------------------------------------------------------------------

void CoreImpl::print(const Char* msg, ...)
{
	char buffer[1024];

	va_list vargs;
	va_start(vargs, msg);
	vsprintf(buffer, msg, vargs);
	va_end(vargs);

	if (_log)
	{
		fprintf(_log, buffer);
		fflush(_log);
	}

	printDebug(buffer);
}

//---------------------------------------------------------------------------

Service* CoreImpl::createA(const Char* typeName)
{
	return (_script) ? _script->createA(typeName) : 0;
}

//---------------------------------------------------------------------------

void CoreImpl::error(const Char* msg)
{
	die(msg);
}

//---------------------------------------------------------------------------

void CoreImpl::die(const Char* msg)
{
	print("die(): %s\n", msg);
	printAlert(FATAL_ERR, msg);
	stopEngine();
	exit(-1);
}

//---------------------------------------------------------------------------

const Char* CoreImpl::getBinDirectory()
{
	return m_binDir;
}

//---------------------------------------------------------------------------

const Char* CoreImpl::getStartDirectory()
{
	return m_startDir;
}

//---------------------------------------------------------------------------

const Char* CoreImpl::getExecutableName()
{
	return m_exeName;
}

//---------------------------------------------------------------------------

ScriptService* CoreImpl::getScriptService()
{
	return _script;
}

//---------------------------------------------------------------------------

#if SHOW_DLERROR
#include <dlfcn.h>
#endif

bool CoreImpl::loadPackage(ServicesList* services, const char* name)
{
	// Try to find a DLL package first
	SharedLibrary hDll = openLibrary(name);
	if (hDll)
	{
		// Get the list of services out of the DLL
		ServicesList (*getServices)();
		if (bindSymbol((void**)&getServices, hDll, "_f4_packlist"))
		{
			*services = getServices();
			return true;
		}

		closeLibrary(hDll);
	}
#if SHOW_DLERROR
	else
	{
		print("%s: %s\n", name, dlerror());
	}
#endif

	// If the DLL could not be loaded, or was not a valid package DLL, try
	// to find a directory with the given name. Be sure to restore the 
	// original working directory before returning
	*services = 0;
	if (m_fileServer)
	{
		return m_fileServer->pathExists(name);
	}
	else
	{
		char buffer[1024];
		getCurrentDirectory(buffer, 1024);
		bool exists = changeDirectory(name);
		changeDirectory(buffer);
		return exists;
	}
}

//---------------------------------------------------------------------------

bool CoreImpl::runFile(const Char* filename, Context* context)
{
	if (_script)
	{
		if (m_fileServer)
		{
			return m_fileServer->runFile(filename, context);
		}
		else
		{
			// chdir() to the directory which contains the script and run
			// it from there. Restore the original working directory after
			// the script has completed.
			char oldDir[512], buffer[512];
			getCurrentDirectory(oldDir, 512);
			splitPath(filename, DIRECTORY, buffer, 512);
			bool result = (*buffer != 0) ? changeDirectory(buffer) : true;
			if (result)
			{
				// Load the file contents in a memory buffer
				splitPath(filename, FILENAME, buffer, 512);
				File file = fileOpen(buffer, "in");
				if (!file) return false;

				fileSeek(file, 0, FILE_END);
				int size = fileTell(file);
				fileSeek(file, 0, FILE_START);

				char* script = (char*)malloc(size);
				size = fileRead(file, script, size);

				fileClose(file);

				// Execute the buffer
				result = _script->runBuffer(script, size, buffer, context);

				free(script);
			}

			changeDirectory(oldDir);
			return result;
		}
	}

	return false;
}

//---------------------------------------------------------------------------

void CoreImpl::runBuffer(void* buffer, unsigned size, const Char* name, Context* context)
{
	if (_script) _script->runBuffer(buffer, size, name, context);
}

//---------------------------------------------------------------------------

void CoreImpl::registerEventLoop(EventLoop* el)
{
	m_eventLoop = el;
}

//---------------------------------------------------------------------------

void CoreImpl::addEventListener(EventListener* el)
{
	m_eventListeners.push_back(el);
}

//---------------------------------------------------------------------------

void CoreImpl::removeEventListener(EventListener* el)
{
	EventListeners::iterator i;
	for (i = m_eventListeners.begin(); i != m_eventListeners.end(); ++i)
	{
		if ((*i) == el)
		{
			m_eventListeners.erase(i);
			break;
		}
	}
}

//---------------------------------------------------------------------------

ScriptService* CoreImpl::registerFileServer(CoreFileServer* fs)
{
	m_fileServer = fs;
	return _script;
}

//---------------------------------------------------------------------------

Context* CoreImpl::beginDispatch(Context* c, Service* s, const Char* method)
{
	if (!_script) return 0;

	if (!c) c = _script->getDefaultContext();
	c->beginDispatch(s, method);
	return c;
}

//---------------------------------------------------------------------------

void CoreImpl::_dispatch(Event* e)
{
	EventListeners::iterator i;
	for (i = m_eventListeners.begin(); i != m_eventListeners.end(); ++i)
		(*i)->receive(e);
}

//---------------------------------------------------------------------------

/*
 * C function used to access Core::print() method. Used by the memory manager,
 * which doesn't know about CoreService.
 */
static void _print(const char* msg)
{
	if (Core)
		Core->print(msg);
	else
	{
		if (_log)
		{
			fprintf(_log, msg);
			fflush(_log);
		}
		printDebug(msg);
	}
}

//---------------------------------------------------------------------------

/*
 * The default file finder, used by runFile()
 */
void _fileFinder(Char* path, const Char* filename)
{
	*path = 0;
}
