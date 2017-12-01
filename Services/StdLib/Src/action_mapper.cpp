/**
 * @file action_mapper.cpp
 *
 * @author  Jason Perkins (jason@379.com)
 * @date    Copyright (C) 2001 by 379, Inc.
 * @version $Id: action_mapper.cpp,v 1.2 2002/04/22 01:51:56 jason379 Exp $
 *
 * @todo Expose add'n parameters: thresholds, ramp up/down
 *
 */
#include <Core/core.h>
#include <Core/event_listener.h>
#include <System/system.h>
#include <System/click_event.h>
#include <System/key_event.h>
#include <System/time_event.h>
#include <StdLib/action_mapper.h>
#include <StdLib/action.h>
#include <Platform/string.h>
#include <map>
#include <Memory/memory.h>
using namespace f4;

DECLARE_SERVICE(Action)

struct LogicalObject;
struct MapperListener;


struct ActionMapperImpl : public ActionMapper
{
	ActionMapperImpl();
	virtual ~ActionMapperImpl();

	virtual bool createTrigger(const Char* name, Float pressAt, Float relAt);
	virtual bool createToggle(const Char* name, Float pressAt, Float relAt);
	virtual bool createAxis(const Char* name, Float rampUp, Float rampDn);

	virtual bool bind(const Char* physical, const Char* logical);
	virtual bool translate(Event* event);

private:
	// The list of logical input objects
	typedef std::map<std::string, LogicalObject*> Objects;
	Objects m_objects;

	// The physical->logical bindings
	typedef std::map<UInt16, LogicalObject*> Bindings;
	Bindings m_bindings;

	MapperListener* m_listener;
	Ref<SystemService> m_sys;
	Ref<Action> m_action;
	
	UInt16 _decodeObjectString(const Char* object);
	UInt16 _makeId(int device, int type, int object);
	void   _addObject(LogicalObject* object);
	void   _pollObjects(Float timestep);

	DECL_SERVICE;
};

IMPL_SERVICE(ActionMapperImpl, ActionMapper)



//---------------------------------------------------------------------------
// An event listener to catch idle time events, used to update the state
// of virtual axis objects.
//---------------------------------------------------------------------------

struct MapperListener : public EventListener
{
	MapperListener() { }
	MapperListener(ActionMapperImpl* mapper) { m_mapper = mapper; }
	ActionMapperImpl* m_mapper;

	virtual bool receive(Event* event) 	
	{
		return m_mapper->translate(event);
	}

	DECL_SERVICE;
};

IMPL_SERVICE(MapperListener, EventListener)



//---------------------------------------------------------------------------
// Local stuff
//---------------------------------------------------------------------------

enum { KEYBOARD = 1, MOUSE, GAME };
enum { BUTTON, AXIS };

struct LogicalObject
{
	virtual bool update(int objectType, Float* value) = 0;
	virtual bool poll(Float timestep, Float* value) { return false; }
	std::string name;
};

struct Trigger : public LogicalObject
{
	virtual bool update(int objectType, Float* value);
	bool  isDown;
	Float pressThreshold;
	Float releaseThreshold;
};

struct Toggle : public LogicalObject
{
	virtual bool update(int objectType, Float* value);
	bool isDown;
	Float pressThreshold;
	Float releaseThreshold;
};

struct Axis : public LogicalObject
{
	virtual bool update(int objectType, Float* value);
	virtual bool poll(Float timestep, Float* value);
	bool  isDown;
	Float pos;
	Float rampUpTime;
	Float rampDownTime;
};



//---------------------------------------------------------------------------

ActionMapperImpl::ActionMapperImpl()
{
	m_sys = (SystemService*)Core->createA("System.SystemService");
	m_action = CREATEA_LOCAL(Action);

	m_listener = new MapperListener(this);
	Core->addEventListener(m_listener);
}

//---------------------------------------------------------------------------

ActionMapperImpl::~ActionMapperImpl()
{
	Core->removeEventListener(m_listener);
	delete m_listener;

	Objects::iterator i;
	for (i = m_objects.begin(); i != m_objects.end(); ++i)
		delete (*i).second;
}

//---------------------------------------------------------------------------

bool ActionMapperImpl::createTrigger(const Char* name, Float pressAt, Float relAt)
{
	Trigger* t = new Trigger;
	t->name = name;
	t->isDown = false;
	t->pressThreshold = pressAt;
	t->releaseThreshold = relAt;

	_addObject(t);
	return true;
}

//---------------------------------------------------------------------------

bool ActionMapperImpl::createToggle(const Char* name, Float pressAt, Float relAt)
{
	Toggle* t = new Toggle;
	t->name = name;
	t->isDown = false;
	t->pressThreshold = pressAt;
	t->releaseThreshold = relAt;

	_addObject(t);
	return true;
}

//---------------------------------------------------------------------------

bool ActionMapperImpl::createAxis(const Char* name, Float rampUp, Float rampDn)
{
	Axis* a = new Axis;
	a->name = name;
	a->pos = 0.0;
	a->isDown = false;
	a->rampUpTime = rampUp;
	a->rampDownTime = rampDn;

	_addObject(a);
	return true;
}

//---------------------------------------------------------------------------

bool ActionMapperImpl::bind(const Char* physical, const Char* logical)
{
	// Convert the physical object identifier into an integer code
	UInt16 pObj = _decodeObjectString(physical);
	if (!pObj) return false;

	// Locate the logical object
	Objects::iterator lObj = m_objects.find(logical);
	if (lObj == m_objects.end()) return false;

	// Replace the existing binding or create a new one
	Bindings::iterator b = m_bindings.find(pObj);
	if (b != m_bindings.end())
		(*b).second = (*lObj).second;
	else
		m_bindings.insert(Bindings::value_type(pObj, (*lObj).second));

	return true;
}

//---------------------------------------------------------------------------

bool ActionMapperImpl::translate(Event* event)
{
	int   device, type, object;
	Float value;

	if (event->isA("TimeEvent"))
	{
		TimeEvent* te = static_cast<TimeEvent*>(event);
		_pollObjects(te->getElapsed());
		return false;
	}

	else if (event->isA("KeyEvent"))
	{
		KeyEvent* ke = static_cast<KeyEvent*>(event);
		device = KEYBOARD;
		type   = BUTTON;
		object = ke->getCode();
		value  = ke->isPressed() ? 1.0 : 0.0;
	}

	else if (event->isA("ClickEvent"))
	{
		ClickEvent* ce = static_cast<ClickEvent*>(event);
		device = MOUSE;
		type   = BUTTON;
		object = ce->getButton();
		value  = ce->isPressed() ? 1.0 : 0.0;

		Core->print("click event: %d, %f\n", object, value);
	}

	else
		return false;

	// Find the binding associated with this object, if one exists
	Bindings::iterator b = m_bindings.find(_makeId(device, type, object));
	if (b == m_bindings.end()) return false;

	// Update the logical input object and send an event if needed
	if ((*b).second->update(type, &value))
	{
		m_action->initialize((*b).second->name.c_str(), value);
		Core->queueEvent(m_action);
		return true;
	}

	return false;
}

//---------------------------------------------------------------------------

UInt16 ActionMapperImpl::_decodeObjectString(const Char* object)
{
	int devId, typeId, objId;

	// Extract the device name. If there is no device, use "key"
	char device[16];
	const char* ptr = strchr(object, '.');
	if (ptr)
	{
		int len = ptr - object;
		if (len > 15) len = 15;
		strncpy(device, object, len);
		device[len] = 0;
		stringLower(device);
		++ptr;
	}
	else
	{
		strcpy(device, "key");
		ptr = object;
	}

	// Keyboard button?
	if (strcmp(device, "key") == 0)
	{
		int code = m_sys->getKeyCode(ptr);
		return (code) ? _makeId(KEYBOARD, BUTTON, code) : 0;
	}

	// Not a keyboard, identify the device
	if (strcmp(device, "mouse") == 0)
		devId = MOUSE;
	else if (strncmp(device, "ctrl", 4) == 0)
		devId = GAME + atoi(device + 4);
	else
		return 0;

	// Identify the object type
	if (strncmp(ptr, "btn", 3) == 0)
	{
		typeId = BUTTON;
		objId  = atoi(ptr + 3);
	}

	else if (strncmp(ptr + 1, "axis", 4) == 0)
	{
		typeId = AXIS;
		int axis = atoi(ptr + 5) * 2;

		// + or - half of the axis?
		if (*ptr == '+')
			objId = axis;
		else if (*ptr == '-')
			objId = axis + 1;
		else
			return 0;
	}
	
	else return 0;

	return _makeId(devId, typeId, objId);
}

//---------------------------------------------------------------------------

UInt16 ActionMapperImpl::_makeId(int device, int type, int object)
{
	return (device << 12) | (type << 8) | object;
}

//---------------------------------------------------------------------------

void ActionMapperImpl::_addObject(LogicalObject* object)
{
	// If an object with this name already exists replace it
	Objects::iterator i;
	i = m_objects.find(object->name);
	if (i != m_objects.end())
		(*i).second = object;
	else
		m_objects.insert(Objects::value_type(object->name, object));
}

//---------------------------------------------------------------------------

void ActionMapperImpl::_pollObjects(Float timestep)
{
	Float value;
	Objects::iterator i;
	for (i = m_objects.begin(); i != m_objects.end(); ++i)
	{
		if ((*i).second->poll(timestep, &value))
		{
			m_action->initialize((*i).second->name.c_str(), value);
			Core->queueEvent(m_action);
		}
	}
}



//---------------------------------------------------------------------------
// Trigger
//---------------------------------------------------------------------------

bool Trigger::update(int objectType, Float* value)
{
	bool wasDown = isDown;

	if (objectType == BUTTON)
	{
		isDown = (*value == 1.0);
	}
	else
	{
		if (*value > pressThreshold)   isDown = true;
		if (*value < releaseThreshold) isDown = false;
		*value = (isDown) ? 1.0 : 0.0;
	}

	return (isDown && !wasDown);
}




//---------------------------------------------------------------------------
// Toggle
//---------------------------------------------------------------------------

bool Toggle::update(int objectType, Float* value)
{
	bool wasDown = isDown;

	if (objectType == BUTTON)
	{
		isDown = (*value == 1.0);
	}
	else
	{
		if (*value > pressThreshold)   isDown = true;
		if (*value < releaseThreshold) isDown = false;
		*value = (isDown) ? 1.0 : 0.0;
	}

	return (isDown != wasDown);
}



//---------------------------------------------------------------------------
// Axis
//---------------------------------------------------------------------------

bool Axis::update(int objectType, Float* value)
{
	if (objectType == BUTTON)
	{
		isDown = (*value == 1.0);
		return false;
	}
	else
	{
		pos = *value;
		return true;
	}
}

bool Axis::poll(Float timestep, Float* value)
{
	// Ramping up?
	if (isDown && pos < 1.0)
	{
		pos += timestep / rampUpTime;
		if (pos > 1.0) pos = 1.0;
		*value = pos;
		return true;
	}

	// Ramping down?
	if (!isDown && pos > 0.0)
	{
		pos -= timestep / rampDownTime;
		if (pos < 0.0) pos = 0.0;
		*value = pos;
		return true;
	}

	return false;
}
