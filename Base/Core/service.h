/**
 * @file service.h
 *
 * @author  Jason Perkins (jason@379.com)
 * @date    Copyright (C) 2001 by 379, Inc.
 * @version $Id: service.h,v 1.3 2002/06/12 23:05:15 jason379 Exp $
 *
 * @todo Service::isA() is too inefficient. It must search through all of the
 *       method and property listings as well as the interfaces. The interface
 *       hierarchy information should be separate.
 *
 */
#if !defined(_F4_CORE_SERVICE_H_)
#define _F4_CORE_SERVICE_H_

ENTER_NAMESPACE(f4);

struct Service;
struct ServiceInfo;

// --- C interface ---
CORE_API void _serviceReference(Service* s);
CORE_API void _serviceRelease(Service* s);
CORE_API const Char* _serviceTypeName(Service* s);
CORE_API bool _serviceIsA(Service* s, const Char* i);
// --- end C ---



/**
 * The base class for all engine services. Provides reference counting and
 * placeholders for the scripting environment.
 * @ingroup f4_core
 */
struct Service
{
	Service()
	{
		m_referenceCount = 0;
		m_scriptData = 0;
	}


	/**
	 * Hold a reference against an instance of this service.
	 */
	void reference()
	{
		_serviceReference(this);
	}


	/**
	 * Release a reference held against an instance of the component.
	 */
	void release()
	{
		_serviceRelease(this);
	}


	/**
	 * Return the number of references held against this object.
	 */
	int countReferences()
	{
		return m_referenceCount;
	}


	/**
	 * Retrieve the name of the service type.
	 */
	const Char* getTypeName()
	{
		return _serviceTypeName(this);
	}


	/**
	 * Retrieve some information about this service type.
	 */
	virtual ServiceInfo* getTypeInfo() const = 0;


	/**
	 * Determine if this service implements a particular interface.
	 */
	virtual bool isA(const Char* service)
	{
		return _serviceIsA(this, service);
	}


	/**
	 * Attach script environment data to this service.
	 */
	void setScriptData(void* data)
	{
		m_scriptData = data;
	}


	/**
	 * Retrieve the script environment data associated with this object.
	 */
	void* getScriptData() const
	{
		return m_scriptData;
	}


	// The reference counting mechanism is not compatible with stack objects.
	// The protected destructor prevents stack instances of Service.
protected:
	virtual ~Service() { }

private:
	int   m_referenceCount;
	void* m_scriptData;

	friend void _serviceReference(Service*);
	friend void _serviceRelease(Service*);
	friend bool _serviceIsA(Service*, const Char*);
	friend const Char* _serviceTypeName(Service*);
};



// --- begin scripting interface ---

SCRIPT_METHOD(Service, isA, RETURNS_BOOL)
	ARG_STRING
END_METHOD

EXPORTS(Service)
	METHOD(Service, isA)
END_EXPORTS

// --- end scripting interface ---


EXIT_NAMESPACE;
#endif
