/**
 * @file update_visitor.cpp
 *
 * @author  Jason Perkins (jason@379.com)
 * @date    Copyright (C) 2002 by 379, Inc.
 * @version $Id: update_visitor.cpp,v 1.2 2002/07/20 02:00:33 jason379 Exp $
 */
#include <Core/core.h>
#include <Scene/update_visitor.h>
#include <Scene/entity.h>
#include <Scene/pose.h>
#include <Physics/physics.h>
#include <vector>
#include <Memory/memory.h>
using namespace f4;


struct iUpdateVisitor : public UpdateVisitor
{
	iUpdateVisitor();
	virtual ~iUpdateVisitor() { }

	virtual void update(Entity* root, Float elapsedTime);

	virtual Float getElapsedTime() const;
	virtual void  setPhysicsRate(Float rate);
	virtual void  setPose(Pose* pose);

	virtual const Matrix44& getWorldMatrix() const;

	// Visitor interface
	virtual void visitEntity(Entity* ent);
	virtual void visitAttribute(Attribute* at);

private:
	Ref<PhysicsService> m_ps;

	Float m_currentTime;
	Float m_elapsedTime;

	Float m_physicsRate;
	Float m_physicsTime;

	// This stack maintains a state for each level of the tree
	struct State
	{
		State() { }

		State(const State& s)
		{
			matrix = s.matrix;
		}

		const Matrix44* matrix;
	};
	
	typedef std::vector<State> StateStack;
	StateStack m_state;

	DECL_SERVICE;
};

IMPL_SERVICE(iUpdateVisitor, UpdateVisitor)

//---------------------------------------------------------------------------

iUpdateVisitor::iUpdateVisitor()
: m_currentTime(0),
  m_elapsedTime(0),
  m_physicsRate(0.01),
  m_physicsTime(0)
{
	m_ps = (PhysicsService*)Core->createA("Physics.PhysicsService");
}

//---------------------------------------------------------------------------

void iUpdateVisitor::update(Entity* root, Float elapsedTime)
{
	// Prepare the new pass
	m_elapsedTime  = elapsedTime;
	m_currentTime += elapsedTime;

	// Advance the physics objects first
	if (m_physicsRate > 0 && m_ps && elapsedTime > 0) 
	{
		while (m_physicsTime < m_currentTime)
		{
			m_ps->advance(m_physicsRate);
			m_physicsTime += m_physicsRate;
		}
	}

	// Seed the state stack
	static Matrix44 identity = Matrix44::identity();

	State s;
	s.matrix = &identity;

	m_state.clear();
	m_state.push_back(s);

	// Run the update pass
	visitEntity(root);
}

//---------------------------------------------------------------------------

Float iUpdateVisitor::getElapsedTime() const
{
	return m_elapsedTime;
}

//---------------------------------------------------------------------------

void iUpdateVisitor::setPhysicsRate(Float rate)
{
	if (rate < 0.0) 
		rate = 0.01;

	m_physicsRate = rate;
}

//---------------------------------------------------------------------------

void iUpdateVisitor::setPose(Pose* pose)
{
	m_state.back().matrix = &pose->getWorldMatrix();
}

//---------------------------------------------------------------------------

const Matrix44& iUpdateVisitor::getWorldMatrix() const
{
	return *m_state.back().matrix;
}

//---------------------------------------------------------------------------

void iUpdateVisitor::visitEntity(Entity* ent)
{
	// Make a new copy of the current matrix at the top of the stack
	m_state.push_back(m_state.back());

	ent->visit(this);

	m_state.pop_back();
}

//---------------------------------------------------------------------------

void iUpdateVisitor::visitAttribute(Attribute* at)
{
	at->update(this);
}
