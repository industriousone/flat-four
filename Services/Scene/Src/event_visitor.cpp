/**
 * @file event_visitor.cpp
 *
 * @author  Jason Perkins (jason@379.com)
 * @date    Copyright (C) 2002 by 379, Inc.
 * @version $Id: event_visitor.cpp,v 1.1 2002/07/16 01:14:10 jason379 Exp $
 */
#include <Core/core.h>
#include <Scene/event_visitor.h>
#include <Scene/entity.h>
#include <Memory/memory.h>
using namespace f4;


struct iEventVisitor : public EventVisitor
{
	iEventVisitor();
	virtual ~iEventVisitor() { }

	void   dispatch(Entity* root, Event* event);
	Event* getEvent();

	// Visitor interface
	virtual void visitEntity(Entity* ent);
	virtual void visitAttribute(Attribute* at);

private:
	Ref<Event> m_event;
	
	DECL_SERVICE;
};

IMPL_SERVICE(iEventVisitor, EventVisitor)

//---------------------------------------------------------------------------

iEventVisitor::iEventVisitor()
: m_event(NULL)
{
}

//---------------------------------------------------------------------------

void iEventVisitor::dispatch(Entity* root, Event* event)
{
	m_event = event;
	visitEntity(root);
}

//---------------------------------------------------------------------------

Event* iEventVisitor::getEvent()
{
	return m_event;
}

//---------------------------------------------------------------------------

void iEventVisitor::visitEntity(Entity* ent)
{
	ent->visit(this);
}

//---------------------------------------------------------------------------

void iEventVisitor::visitAttribute(Attribute* at)
{
	at->event(this);
}

