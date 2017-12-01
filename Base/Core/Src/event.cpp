/**
 * @file event.cpp
 *
 * @author  Jason Perkins (jason@379.com)
 * @date    Copyright (C) 2001 by 379, Inc.
 * @version $Id: event.cpp,v 1.2 2002/04/22 01:51:55 jason379 Exp $
 */
#include <Core/core.h>
#include <Core/event.h>
#include <Memory/memory.h>
using namespace f4;

struct ScriptedEvent : public Event
{
private:
	DECL_SERVICE;
};

IMPL_SERVICE(ScriptedEvent, Event)
