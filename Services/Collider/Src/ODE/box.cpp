/**
 * @file box.cpp
 *
 * @author  Jason Perkins (jason@379.com)
 * @date    Copyright (C) 2001 by 379, Inc.
 * @version $Id: box.cpp,v 1.3 2002/05/15 10:40:04 jason379 Exp $
 */
#include <ode/ode.h>

#include <Core/core.h>
#include <Collider/box.h>
#include <Math/matrix.h>
#include <Memory/memory.h>
using namespace f4;


struct BoxImpl : public BoundingBox
{
	BoxImpl();
	virtual ~BoxImpl();

	virtual bool  initialize(Float x, Float y, Float z);

	// BoundingShape interface
	virtual void* getShapeData() { return &m_geom; }
	virtual void  setMatrix(const Matrix44& m);

private:
	dGeomID m_geom;
	DECL_SERVICE;
};

IMPL_SERVICE(BoxImpl, BoundingBox)

//---------------------------------------------------------------------------

BoxImpl::BoxImpl()
{
	m_geom = dCreateBox(0, 1.0, 1.0, 1.0);
}

//---------------------------------------------------------------------------

BoxImpl::~BoxImpl()
{
	dGeomDestroy(m_geom);
}

//---------------------------------------------------------------------------

bool BoxImpl::initialize(Float x, Float y, Float z)
{
	dGeomBoxSetLengths(m_geom, x, y, z);
	return true;
}

//---------------------------------------------------------------------------

void BoxImpl::setMatrix(const Matrix44& m)
{
	dGeomSetPosition(m_geom, m[3][0], m[3][1], m[3][2]);

	dMatrix4 r;
	r[0]  = m[0][0];
	r[1]  = m[1][0];
	r[2]  = m[2][0];
	r[3]  = 0;
	r[4]  = m[0][1];
	r[5]  = m[1][1];
	r[6]  = m[2][1];
	r[7]  = 0;
	r[8]  = m[0][2];
	r[9]  = m[1][2];
	r[10] = m[2][2];
	r[11] = 0;
	dGeomSetRotation(m_geom, r);
}
