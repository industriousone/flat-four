/**
 * @file physics.cpp
 *
 * @author  Jason Perkins (jason@379.com)
 * @date    Copyright (C) 2002 by 379, Inc.
 * @version $Id: physics.cpp,v 1.5 2002/07/16 01:11:37 jason379 Exp $
 *
 * @todo Come up with a more intelligent way to check for motion. I should
 *       only force updates to children when the body has moved more than
 *       some threshold value.
 *
 */
#include <Core/core.h>
#include <Scene/physics.h>
#include <Scene/entity.h>
#include <Scene/update_visitor.h>
#include <Scene/render_visitor.h>
#include <Scene/collision_visitor.h>
#include <Math/math.h>
#include <Memory/memory.h>
using namespace f4;


struct PhysicsImpl : public Physics
{
	PhysicsImpl();
	virtual ~PhysicsImpl() { }

	virtual RigidBody* getBody();

	// Pose interface
	virtual void setPosition(Float x, Float y, Float z);
	virtual Vector3 getPosition() const;
	virtual void setEulerRotation(Float h, Float r, Float p);
	virtual void setAngleAxisRotation(Float a, Float x, Float y, Float z);
	virtual void setRotation(const Quaternion& quat);
	virtual Quaternion getRotation() const;
	virtual Matrix44 getMatrix() const;
	virtual const Matrix44& getWorldMatrix() const;

	// Attribute interface
	virtual void update(UpdateVisitor* uv);
	virtual void render(RenderVisitor* rv);
	virtual void collide(CollisionVisitor* cv);
	virtual void touch(CollisionVisitor* cv);

private:
	Ref<RigidBody> m_body;
	Matrix44 m_worldSpace;
	DECL_SERVICE;
};

IMPL_SERVICE(PhysicsImpl, Physics)

//---------------------------------------------------------------------------

PhysicsImpl::PhysicsImpl()
{
	m_body = (RigidBody*)Core->createA("Physics.RigidBody");
	m_worldSpace.setIdentity();
}

//---------------------------------------------------------------------------

RigidBody* PhysicsImpl::getBody()
{
	return m_body;
}

//---------------------------------------------------------------------------

void PhysicsImpl::setPosition(Float x, Float y, Float z)
{
	m_body->setPosition(x, y, z);
}

//---------------------------------------------------------------------------

Vector3 PhysicsImpl::getPosition() const
{
	return m_body->getPosition();
}

//---------------------------------------------------------------------------

void PhysicsImpl::setEulerRotation(Float h, Float r, Float p)
{
	m_body->setEulerRotation(h, p, r);
}

//---------------------------------------------------------------------------

void PhysicsImpl::setAngleAxisRotation(Float a, Float x, Float y, Float z)
{
	m_body->setAngleAxisRotation(a, x, y, z);
}

//---------------------------------------------------------------------------

void PhysicsImpl::setRotation(const Quaternion& quat)
{
	m_body->setRotation(quat);
}

//---------------------------------------------------------------------------

Quaternion PhysicsImpl::getRotation() const
{
	return m_body->getRotation();
}

//---------------------------------------------------------------------------

Matrix44 PhysicsImpl::getMatrix() const
{
	return m_body->getMatrix();
}

//---------------------------------------------------------------------------

const Matrix44& PhysicsImpl::getWorldMatrix() const
{
	return m_worldSpace;
}

//---------------------------------------------------------------------------

void PhysicsImpl::update(UpdateVisitor* uv)
{
	// Compute a new complete world space xform
	m_worldSpace = m_body->getMatrix() * uv->getWorldMatrix();

	// Make this the current pose for all subsequent updates
	uv->setPose(this);
}

//---------------------------------------------------------------------------

void PhysicsImpl::render(RenderVisitor* rv)
{
	rv->setPose(this);
}

//---------------------------------------------------------------------------

void PhysicsImpl::collide(CollisionVisitor* cv)
{
	// Until I can tell if the body has been enabled/disabled, I will
	// just assume that all rigid bodies move each frame.
	cv->hasMoved();
}

//---------------------------------------------------------------------------

void PhysicsImpl::touch(CollisionVisitor* cv)
{
	cv->setPhysics(m_body);
}
