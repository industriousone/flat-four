/**
 * @file physics.cpp
 *
 * @author  Jason Perkins (jason@379.com)
 * @date    Copyright (C) 2001 by 379, Inc.
 * @version $Id: physics.cpp,v 1.5 2002/06/19 01:40:08 jason379 Exp $
 */
#include <Core/core.h>
#include <Physics/physics.h>
#include <Physics/contact_physics.h>
#include <Physics/rigid_body.h>
#include "impl.h"
#include <Memory/memory.h>
using namespace f4;


struct PhysicsImpl : public PhysicsService
{
	PhysicsImpl();
	virtual ~PhysicsImpl();

	virtual void advance(Float timestep);

	virtual void setGravity(Float x, Float y, Float z);
	virtual Vector3 getGravity() const;

	virtual void resolveCollision(PhysicsObject* p1, PhysicsObject* p2, ContactList* cl, ContactPhysics* cp);

private:
	dJointGroupID m_joints;
	dJointGroupID m_contacts;

	DECL_SERVICE;
};

IMPL_SINGLETON_SERVICE(PhysicsImpl, PhysicsService)



//---------------------------------------------------------------------------
// Local stuff
//---------------------------------------------------------------------------

PhysicsService* _ps    = 0;
dWorldID        _world = 0;



//---------------------------------------------------------------------------

PhysicsImpl::PhysicsImpl()
{
	Core->print("Initializing Physics service\n");
	Core->print("  Open Dynamics Engine - Copyright (C) 2001 Russell L. Smith\n");

	_ps = this;
	_world = dWorldCreate();

	m_joints   = dJointGroupCreate(1000000);
	m_contacts = dJointGroupCreate(1000000);
}

//---------------------------------------------------------------------------

PhysicsImpl::~PhysicsImpl()
{
	dJointGroupDestroy(m_contacts);
	dJointGroupDestroy(m_joints);
	dWorldDestroy(_world);

	_ps = 0;
	_world = 0;
}

//---------------------------------------------------------------------------

void PhysicsImpl::advance(Float timestep)
{
	if (timestep > 0.0)
	{
		dWorldStep(_world, timestep);
		dJointGroupEmpty(m_contacts);
	}
}

//---------------------------------------------------------------------------

void PhysicsImpl::setGravity(Float x, Float y, Float z)
{
	dWorldSetGravity(_world, x, y, z);
}

//---------------------------------------------------------------------------

Vector3 PhysicsImpl::getGravity() const
{
	dVector3 gravity;
	dWorldGetGravity(_world, gravity);
	return Vector3(gravity[0], gravity[1], gravity[2]);
}

//---------------------------------------------------------------------------

void PhysicsImpl::resolveCollision(PhysicsObject* p1, PhysicsObject* p2, ContactList* cl, ContactPhysics* cp)
{
	dBodyID body1 = (p1) ? static_cast<dBodyID>(p1->getBodyData()) : 0;
	dBodyID body2 = (p2) ? static_cast<dBodyID>(p2->getBodyData()) : 0;

	if (body1 == body2)      // invalid state!
		return;

	bool swapped = false;
	if (!body1 && body2)
	{
		body1 = body2;
		body2 = 0;
		swapped = true;
	}

	int flags = 0;
	Float bounce = cp->getRestitution();
	if (bounce > 0.0) flags |= dContactBounce;

	Float friction = cp->getFriction();
	if (friction == ContactPhysics::INFINITY) friction = dInfinity;

	dContact info;
	info.surface.mode       = flags;
	info.surface.mu         = friction;
	info.surface.bounce     = bounce;
	info.surface.bounce_vel = 0.1;

	int numContacts = cl->size();
	for (int i = 0; i < numContacts; ++i)
	{
		const Vector3& p = cl->getPosition(i);
		info.geom.pos[0] = p.x;
		info.geom.pos[1] = p.y;
		info.geom.pos[2] = p.z;

		Vector3 n = (swapped) ? -cl->getNormal(i) : cl->getNormal(i);
		info.geom.normal[0] = n.x;
		info.geom.normal[1] = n.y;
		info.geom.normal[2] = n.z;

		info.geom.depth = cl->getDepth(i);

		dJointID j = dJointCreateContact(_world, m_contacts, &info);
		dJointAttach(j, body1, body2);
	}
}



