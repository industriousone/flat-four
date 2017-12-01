/**
 * @file event_listener.cpp
 *
 * @author  Jason Perkins (jason@379.com)
 * @date    Copyright (C) 2001 by 379, Inc.
 * @version $Id: event_listener.cpp,v 1.2 2002/04/22 01:51:55 jason379 Exp $
 */
#include <string.h>

#include <Core/core.h>
#include <Core/event_listener.h>
#include <Core/event.h>
#include <Memory/memory.h>
using namespace f4;


struct EventListenerImpl : public EventListener
{
	virtual bool receive(Event* e);

private:
	DECL_SERVICE;
};

IMPL_SERVICE(EventListenerImpl, EventListener)

//---------------------------------------------------------------------------

bool EventListenerImpl::receive(Event* e)
{
	// Try to dispatch to 'on[event]', where event = the name of the event
	Char method[128];
	strcpy(method, "on");
	strcat(method, e->getTypeName());

	bool handled = false;
	DISPATCH_METHOD(method, 0)
		DISPATCH_ARG_SERVICE(e)
	DISPATCH_RETURNS_BOOL(handled)

	// If the event wasn't handled there, send it to receive()
	if (!handled)
	{
		DISPATCH_METHOD("receive", 0)
			DISPATCH_ARG_SERVICE(e)
		DISPATCH_RETURNS_BOOL(handled)
	}

	return handled;
}
