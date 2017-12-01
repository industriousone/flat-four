/**
 * @file simple_space.cpp
 *
 * @author  Jason Perkins (jason@379.com)
 * @date    Copyright (C) 2002 by 379, Inc.
 * @version $Id: simple_space.cpp,v 1.5 2002/07/16 01:11:37 jason379 Exp $
 */
#include <Core/core.h>
#include <Scene/space.h>
#include <Scene/entity.h>
#include <Scene/collision_visitor.h>
#include <Scene/event_visitor.h>
#include <Scene/render_visitor.h>
#include <Scene/update_visitor.h>
#include <vector>
#include <Memory/memory.h>
using namespace f4;


struct SimpleSpace : public Space
{
	SimpleSpace();
	virtual ~SimpleSpace() { }

	// Space interface
	virtual void addEntity(Entity* ent);
	virtual Entity* queryEntity(const Char* name);

	// Attribute interface
	virtual void update(UpdateVisitor* uv);
	virtual void render(RenderVisitor* rv);
	virtual void collide(CollisionVisitor* cv);
	virtual void event(EventVisitor* ev);

private:
	// A simple list of entities
	typedef std::vector< Ref<Entity> > Entities;
	Entities m_entities;

	DECL_SERVICE;
};

IMPL_ANON_SERVICE(SimpleSpace, Space)

//---------------------------------------------------------------------------

SimpleSpace::SimpleSpace()
{
}

//---------------------------------------------------------------------------

void SimpleSpace::addEntity(Entity* ent)
{
	if (ent)
		m_entities.push_back(ent);
}

//---------------------------------------------------------------------------

Entity* SimpleSpace::queryEntity(const Char* name)
{
	Entities::iterator it;
	for (it = m_entities.begin(); it != m_entities.end(); ++it)
	{
		if (strcmp((*it)->getName(), name) == 0)
			return (*it);
	}

	return 0;
}

//---------------------------------------------------------------------------

void SimpleSpace::update(UpdateVisitor* uv)
{
	Entities::iterator it;
	for (it = m_entities.begin(); it != m_entities.end(); ++it)
		uv->visitEntity(*it);
}

//---------------------------------------------------------------------------

void SimpleSpace::render(RenderVisitor* rv)
{
	// This should eventually cull the entities using their bounding volume
	Entities::iterator it;
	for (it = m_entities.begin(); it != m_entities.end(); ++it)
		rv->visitEntity(*it);
}

//---------------------------------------------------------------------------

void SimpleSpace::collide(CollisionVisitor* cv)
{
	// This should eventually cull the entities using their bounding volume
	Entities::iterator it;
	for (it = m_entities.begin(); it != m_entities.end(); ++it)
		cv->visitEntity(*it);
}

//---------------------------------------------------------------------------

void SimpleSpace::event(EventVisitor* cv)
{
	Entities::iterator it;
	for (it = m_entities.begin(); it != m_entities.end(); ++it)
		cv->visitEntity(*it);
}
