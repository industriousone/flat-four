/**
 * @file sphere.cpp
 *
 * @author  Jason Perkins (jason@379.com)
 * @date    Copyright (C) 2001 by 379, Inc.
 * @version $Id: sphere.cpp,v 1.3 2002/05/15 10:40:04 jason379 Exp $
 */
#include <ode/ode.h>

#include <Core/core.h>
#include <Collider/sphere.h>
#include <Math/matrix.h>
#include <Memory/memory.h>
using namespace f4;


struct SphereImpl : public BoundingSphere
{
	SphereImpl();
	virtual ~SphereImpl();

	virtual bool  initialize(Float radius);
	virtual void  setRadius(Float radius);
	virtual Float getRadius();

	// BoundingShape interface
	virtual void* getShapeData() { return &m_geom; }
	virtual void  setMatrix(const Matrix44& m);

private:
	dGeomID m_geom;
	DECL_SERVICE;
};

IMPL_SERVICE(SphereImpl, BoundingSphere)

//---------------------------------------------------------------------------

SphereImpl::SphereImpl()
{
	m_geom = dCreateSphere(0, 0.5);
}

//---------------------------------------------------------------------------

SphereImpl::~SphereImpl()
{
	dGeomDestroy(m_geom);
}

//---------------------------------------------------------------------------

bool SphereImpl::initialize(Float radius)
{
	dGeomSphereSetRadius(m_geom, radius);
	return true;
}

//---------------------------------------------------------------------------

void SphereImpl::setRadius(Float radius)
{
	dGeomSphereSetRadius(m_geom, radius);
}

//---------------------------------------------------------------------------

Float SphereImpl::getRadius()
{
	return dGeomSphereGetRadius(m_geom);
}

//---------------------------------------------------------------------------

void SphereImpl::setMatrix(const Matrix44& m)
{
	dGeomSetPosition(m_geom, m[3][0], m[3][1], m[3][2]);
}
