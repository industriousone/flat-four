/**
 * @file pose.cpp
 *
 * @author  Jason Perkins (jason@379.com)
 * @date    Copyright (C) 2002 by 379, Inc.
 * @version $Id: pose.cpp,v 1.5 2002/07/16 01:11:37 jason379 Exp $
 */
#include <Core/core.h>
#include <Scene/pose.h>
#include <Scene/entity.h>
#include <Scene/update_visitor.h>
#include <Scene/render_visitor.h>
#include <Scene/collision_visitor.h>
#include <Memory/memory.h>
using namespace f4;


struct PoseImpl : public Pose
{
	PoseImpl();
	virtual ~PoseImpl() { }

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

private:
	Vector3    m_position;
	Quaternion m_rotation;
	Matrix44   m_worldSpace;

	bool m_hasMoved;

	DECL_SERVICE;
};

IMPL_SERVICE(PoseImpl, Pose)

//---------------------------------------------------------------------------

PoseImpl::PoseImpl()
: m_position(0, 0, 0),
  m_rotation(1, 0, 0, 0),
  m_hasMoved(false)
{
	m_worldSpace.setIdentity();
}

//---------------------------------------------------------------------------

void PoseImpl::setPosition(Float x, Float y, Float z)
{
	m_position.set(x, y, z);
	m_hasMoved = true;
}

//---------------------------------------------------------------------------

Vector3 PoseImpl::getPosition() const
{
	return m_position;
}

//---------------------------------------------------------------------------

void PoseImpl::setEulerRotation(Float h, Float r, Float p)
{
	m_rotation.setEulerRotation(h, r, p);
	m_hasMoved = true;
}

//---------------------------------------------------------------------------

void PoseImpl::setAngleAxisRotation(Float a, Float x, Float y, Float z)
{
	m_rotation.setAngleAxisRotation(a, x, y, z);
	m_hasMoved = true;
}

//---------------------------------------------------------------------------

void PoseImpl::setRotation(const Quaternion& quat)
{
	m_rotation = quat;
	m_hasMoved = true;
}

//---------------------------------------------------------------------------

Quaternion PoseImpl::getRotation() const
{
	return m_rotation;
}

//---------------------------------------------------------------------------

Matrix44 PoseImpl::getMatrix() const
{
	Matrix44 m;
	m.setQuaternion(m_rotation);
	m.setTranslation(m_position);

	m[0][3] = 0;
	m[1][3] = 0;
	m[2][3] = 0;
	m[3][3] = 1;

	return m;
}

//---------------------------------------------------------------------------

const Matrix44& PoseImpl::getWorldMatrix() const
{
	return m_worldSpace;
}

//---------------------------------------------------------------------------

void PoseImpl::update(UpdateVisitor* uv)
{
	// Compute a local->parent space xform
	Matrix44 m;
	m.setQuaternion(m_rotation);
	m.setTranslation(m_position);

	m[0][3] = 0;
	m[1][3] = 0;
	m[2][3] = 0;
	m[3][3] = 1;

	// Compute a new complete world space xform
	m_worldSpace = m * uv->getWorldMatrix();

	// Make this the current pose for all subsequent updates
	uv->setPose(this);
}

//---------------------------------------------------------------------------

void PoseImpl::render(RenderVisitor* rv)
{
	rv->setPose(this);
}

//---------------------------------------------------------------------------

void PoseImpl::collide(CollisionVisitor* cv)
{
	if (m_hasMoved)
		cv->hasMoved();
}
