/**
 * @file rigid_body.cpp
 *
 * @author  Jason Perkins (jason@379.com)
 * @date    Copyright (C) 2001 by 379, Inc.
 * @version $Id: rigid_body.cpp,v 1.2 2002/04/22 01:51:56 jason379 Exp $
 */
#include <Core/core.h>
#include <Physics/physics.h>
#include <Physics/rigid_body.h>
#include <Physics/mass_helper.h>
#include "impl.h"
#include <Memory/memory.h>
using namespace f4;

DECLARE_SERVICE(PhysicsService)

struct RigidBodyImpl : public RigidBody
{
	RigidBodyImpl();
	virtual ~RigidBodyImpl();

	virtual void setMassFromHelper(MassHelper* helper);
	virtual void setMass(Float mass);
	virtual void setInertiaAsSolidBox(Float x, Float y, Float z);
	virtual void setInertiaAsSolidCylinder(Float radius, Float width);
	virtual void setInertiaAsSolidSphere(Float radius);

	virtual Float   getMass() const;
	virtual Vector3 getInertia() const;

	virtual void setPosition(Float x, Float y, Float z);
	virtual void setRotation(const Quaternion& q);
	virtual void setAngleAxisRotation(Float angle, Float ax, Float ay, Float az);
	virtual void setEulerRotation(Float h, Float p, Float r);
	
	virtual Vector3    getPosition() const;
	virtual Quaternion getRotation() const;

	virtual void setAngularVelocity(Float x, Float y, Float z);
	virtual void setLinearVelocity(Float x, Float y, Float z);
	virtual void zeroVelocity();

	virtual Vector3 getAngularVelocity() const;
	virtual Vector3 getLinearVelocity() const;

	virtual void applyForce(Float fx, Float fy, Float fz);
	virtual void applyLocalForceAtLocalPos(Float fx, Float fy, Float fz, Float x, Float y, Float z);
	virtual void applyTorque(Float tx, Float ty, Float tz);

	virtual Matrix44 getMatrix() const;

	virtual void* getBodyData() { return m_body; }

private:
	dBodyID m_body;
	DECL_SERVICE;
};

IMPL_SERVICE(RigidBodyImpl, RigidBody)

//-------------------------------------------------------------------------

RigidBodyImpl::RigidBodyImpl()
{
	// Make sure the main service is running
	if (!_ps) CREATEA_LOCAL(PhysicsService);
	_ps->reference();

	// Create a body and assign default parameters
	m_body = dBodyCreate(_world);
	dBodySetData(m_body, this);

	dMass m;
	dMassSetSphere(&m, 1.0, 1.0);
	dBodySetMass(m_body, &m);
}

//---------------------------------------------------------------------------

RigidBodyImpl::~RigidBodyImpl()
{
	dBodyDestroy(m_body);
	_ps->release();
}

//---------------------------------------------------------------------------

void RigidBodyImpl::setMassFromHelper(MassHelper* helper)
{
	dMass* m = static_cast<dMass*>(helper->getMassData());
	dBodySetMass(m_body, m);
}

//---------------------------------------------------------------------------

void RigidBodyImpl::setMass(Float mass)
{
	dMass m;
	dBodyGetMass(m_body, &m);
	dMassAdjust(&m, mass);
	dBodySetMass(m_body, &m);
}

//---------------------------------------------------------------------------

void RigidBodyImpl::setInertiaAsSolidBox(Float x, Float y, Float z)
{
	dMass m;
	dBodyGetMass(m_body, &m);
	Float mass = m.mass;

	dMassSetBox(&m, 1, x, y, z);
	dMassAdjust(&m, mass);
	dBodySetMass(m_body, &m);
}

//---------------------------------------------------------------------------

void RigidBodyImpl::setInertiaAsSolidCylinder(Float radius, Float width)
{
	dMass m;
	dBodyGetMass(m_body, &m);
	Float mass = m.mass;

	Float a = radius * radius / 2.0;
	Float b = a + width * width / 12.0;
	dMassSetParameters(&m, mass, 0, 0, 0, a, b, b, 0, 0, 0);
	dBodySetMass(m_body, &m);
}

//---------------------------------------------------------------------------

void RigidBodyImpl::setInertiaAsSolidSphere(Float radius)
{
	dMass m;
	dBodyGetMass(m_body, &m);
	Float mass = m.mass;

	dMassSetSphere(&m, 1, radius);
	dMassAdjust(&m, mass);
	dBodySetMass(m_body, &m);
}

//---------------------------------------------------------------------------

Float RigidBodyImpl::getMass() const
{
	dMass m;
	dBodyGetMass(m_body, &m);
	return m.mass;
}

//---------------------------------------------------------------------------

Vector3 RigidBodyImpl::getInertia() const
{
	dMass m;
	dBodyGetMass(m_body, &m);
	return Vector3(m.I[0], m.I[5], m.I[10]);
}

//---------------------------------------------------------------------------

void RigidBodyImpl::setPosition(Float x, Float y, Float z)
{
	dBodySetPosition(m_body, x, y, z);
}

//---------------------------------------------------------------------------

void RigidBodyImpl::setRotation(const Quaternion& q)
{
	dQuaternion dq;
	dq[0] = -q.w;
	dq[1] = q.x;
	dq[2] = q.y;
	dq[3] = q.z;
	dBodySetQuaternion(m_body, dq);
}

//---------------------------------------------------------------------------

void RigidBodyImpl::setAngleAxisRotation(Float angle, Float ax, Float ay, Float az)
{
	angle = degreesToRadians(-angle) / 2;
	Float scale = sin(angle);

	dQuaternion dq;
	dq[0] = -cos(angle);
	dq[1] = ax * scale;
	dq[2] = ay * scale;
	dq[3] = az * scale;
	dBodySetQuaternion(m_body, dq);
}

//---------------------------------------------------------------------------

void RigidBodyImpl::setEulerRotation(Float h, Float p, Float r)
{
	Quaternion q;
	q.setEulerRotation(h, p, r);

	dQuaternion dq;
	dq[0] = -q.w;
	dq[1] = q.x;
	dq[2] = q.y;
	dq[3] = q.z;
	dBodySetQuaternion(m_body, dq);
}

//---------------------------------------------------------------------------

Vector3 RigidBodyImpl::getPosition() const
{
	const dReal* p = dBodyGetPosition(m_body);
	return Vector3(p[0], p[1], p[2]);
}

//---------------------------------------------------------------------------

Quaternion RigidBodyImpl::getRotation() const
{
	const dReal* q = dBodyGetQuaternion(m_body);
	return Quaternion(-q[0], q[1], q[2], q[3]);
}

//---------------------------------------------------------------------------

void RigidBodyImpl::setAngularVelocity(Float x, Float y, Float z)
{
	dBodySetAngularVel(m_body, x, y, z);
}

//---------------------------------------------------------------------------

void RigidBodyImpl::setLinearVelocity(Float x, Float y, Float z)
{
	dBodySetLinearVel(m_body, x, y, z);
}

//---------------------------------------------------------------------------

void RigidBodyImpl::zeroVelocity()
{
	dBodySetAngularVel(m_body, 0, 0, 0);
	dBodySetLinearVel(m_body, 0, 0, 0);
}

//---------------------------------------------------------------------------

Vector3 RigidBodyImpl::getAngularVelocity() const
{
	const dReal* vel = dBodyGetAngularVel(m_body);
	return Vector3(vel[0], vel[1], vel[2]);
}

//---------------------------------------------------------------------------

Vector3 RigidBodyImpl::getLinearVelocity() const
{
	const dReal* vel = dBodyGetLinearVel(m_body);
	return Vector3(vel[0], vel[1], vel[2]);
}

//---------------------------------------------------------------------------

void RigidBodyImpl::applyForce(Float fx, Float fy, Float fz)
{
	dBodyAddForce(m_body, fx, fy, fz);
}

//---------------------------------------------------------------------------

void RigidBodyImpl::applyLocalForceAtLocalPos(Float fx, Float fy, Float fz, Float x, Float y, Float z)
{
	dBodyAddRelForceAtRelPos(m_body, fx, fy, fz, x, y, z);
}

//---------------------------------------------------------------------------

void RigidBodyImpl::applyTorque(Float tx, Float ty, Float tz)
{
	dBodyAddTorque(m_body, tx, ty, tz);
}

//---------------------------------------------------------------------------

Matrix44 RigidBodyImpl::getMatrix() const
{
	const dReal* p = dBodyGetPosition(m_body);
	const dReal* r = dBodyGetRotation(m_body);
	
	Matrix44 m;
	m[0][0] = r[0];
	m[0][1] = r[4];
	m[0][2] = r[8];
	m[0][3] = 0;
	m[1][0] = r[1];
	m[1][1] = r[5];
	m[1][2] = r[9];
	m[1][3] = 0;
	m[2][0] = r[2];
	m[2][1] = r[6];
	m[2][2] = r[10];
	m[2][3] = 0;
	m[3][0] = p[0];
	m[3][1] = p[1];
	m[3][2] = p[2];
	m[3][3] = 1;

	return m;
}

