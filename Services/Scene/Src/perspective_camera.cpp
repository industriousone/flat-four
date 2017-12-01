/**
 * @file perspective_camera.cpp
 *
 * @author  Jason Perkins (jason@379.com)
 * @date    Copyright (C) 2002 by 379, Inc.
 * @version $Id: perspective_camera.cpp,v 1.5 2002/07/16 01:11:37 jason379 Exp $
 */
#include <Core/core.h>
#include <Scene/perspective_camera.h>
#include <Scene/entity.h>
#include <Scene/update_visitor.h>
#include <Scene/render_visitor.h>
#include <Render/render.h>
#include <Math/math.h>
#include <Memory/memory.h>
using namespace f4;


struct PerspectiveImpl : public PerspectiveCamera
{
	PerspectiveImpl();
	virtual ~PerspectiveImpl() { }

	virtual void setPosition(Float x, Float y, Float z);
	virtual void setRotation(const Quaternion& quat);
	virtual void setEulerRotation(Float h, Float p, Float r);
	virtual void lookAt(Float atX, Float atY, Float atZ, Float upX, Float upY, Float upZ);

	virtual void setFieldOfView(Float fov);
	virtual void setAspectRatio(Float aspect);
	virtual void setNearClipDistance(Float z);
	virtual void setFarClipDistance(Float z);

	// Camera interface
	virtual void apply(RenderVisitor* rv);

	// Attribute interface
	virtual void update(UpdateVisitor* uv);

private:
	Vector3    m_position;
	Quaternion m_rotation;
	Matrix44   m_worldSpace;

	Float m_fov;
	Float m_aspect;
	Float m_nearClip;
	Float m_farClip;

	DECL_SERVICE;
};

IMPL_SERVICE(PerspectiveImpl, PerspectiveCamera)

//---------------------------------------------------------------------------

PerspectiveImpl::PerspectiveImpl()
: m_position(0, 0, 0),
  m_rotation(1, 0, 0, 0),
  m_fov(60),
  m_aspect(1.3333),
  m_nearClip(0.1),
  m_farClip(1000)
{
	m_worldSpace.setIdentity();
}

//---------------------------------------------------------------------------

void PerspectiveImpl::setPosition(Float x, Float y, Float z)
{
	m_position.set(-x, -y, -z);
}

//---------------------------------------------------------------------------

void PerspectiveImpl::setRotation(const Quaternion& quat)
{
	m_rotation = quat;
}

//---------------------------------------------------------------------------

void PerspectiveImpl::setEulerRotation(Float h, Float p, Float r)
{
	m_rotation.setEulerRotation(h, p, r);
}

//---------------------------------------------------------------------------

void PerspectiveImpl::lookAt(Float atX, Float atY, Float atZ, Float upX, Float upY, Float upZ)
{
	// Build a rotation matrix from position, target, and up
	Vector3 target(atX, atY, atZ);
	Vector3 up(upX, upY, upZ);

	Vector3 z = (-m_position) - target;
	z.normalize();

	Vector3 x = up.cross(z);
	x.normalize();

	Vector3 y = z.cross(x);
	y.normalize();

	Matrix44 m;
	m[0][0] = x.x;   m[0][1] = y.x;   m[0][2] = z.x;   m[0][3] = 0.0;
	m[1][0] = x.y;   m[1][1] = y.y;   m[1][2] = z.y;   m[1][3] = 0.0;
	m[2][0] = x.z;   m[2][1] = y.z;   m[2][2] = z.z;   m[2][3] = 0.0;
	m[3][0] = 0.0f;  m[3][1] = 0.0f;  m[3][2] = 0.0;   m[3][3] = 1.0;

	// Convert it to a quaternion
	m_rotation.setMatrixRotation(m);
	m_rotation = m_rotation.inverse();
}

//---------------------------------------------------------------------------

void PerspectiveImpl::setFieldOfView(Float fov)
{
	m_fov = fov;
}

//---------------------------------------------------------------------------

void PerspectiveImpl::setAspectRatio(Float aspect)
{
	m_aspect = aspect;
}

//---------------------------------------------------------------------------

void PerspectiveImpl::setNearClipDistance(Float z)
{
	m_nearClip = z;
}

//---------------------------------------------------------------------------

void PerspectiveImpl::setFarClipDistance(Float z)
{
	m_farClip = z;
}

//---------------------------------------------------------------------------

void PerspectiveImpl::apply(RenderVisitor* rv)
{
	// Build the inverse of the camera's local transform
	Matrix44 m;
	m.setIdentity();
	m.setQuaternion(m_rotation.inverse());
	m.setTranslation(m * m_position);

	// Apply state to the renderer
	RenderService* rs = rv->getRenderService();
	rs->setProjection3D(m_fov, m_aspect, m_nearClip, m_farClip);
	rs->setViewMatrix(m_worldSpace.inverse() * m);
}

//---------------------------------------------------------------------------

void PerspectiveImpl::update(UpdateVisitor* uv)
{
	m_worldSpace = uv->getWorldMatrix();
}

