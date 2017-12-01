/**
 * @file contact_physics.cpp
 *
 * @author  Jason Perkins (jason@379.com)
 * @date    Copyright (C) 2001 by 379, Inc.
 * @version $Id: contact_physics.cpp,v 1.3 2002/05/24 19:05:31 jason379 Exp $
 */
#include <Core/core.h>
#include <Physics/physics.h>
#include <Physics/contact_physics.h>
#include <Math/math.h>
#include <vector>
#include <assert.h>
#include "impl.h"
#include <Memory/memory.h>
using namespace f4;


struct ContactImpl : public ContactPhysics
{
	ContactImpl();
	virtual ~ContactImpl() { }

	virtual void  setFriction(Float r);
	virtual Float getFriction() const;

	virtual void  setRestitution(Float r);
	virtual Float getRestitution() const;

private:
	Float m_friction;
	Float m_restitution;

	DECL_SERVICE;
};

IMPL_SERVICE(ContactImpl, ContactPhysics)

//---------------------------------------------------------------------------

ContactImpl::ContactImpl()
: m_friction(0),
  m_restitution(0)
{
}

//---------------------------------------------------------------------------

void ContactImpl::setFriction(Float mu)
{
	if (mu < 0 && mu != INFINITY) mu = 0.0;
	m_friction = mu;
}

//---------------------------------------------------------------------------

Float ContactImpl::getFriction() const
{
	return m_friction;
}

//---------------------------------------------------------------------------

void ContactImpl::setRestitution(Float r)
{
	if (r < 0.0) r = 0.0;
	if (r > 1.0) r = 1.0;
	m_restitution = r;
}

//---------------------------------------------------------------------------

Float ContactImpl::getRestitution() const
{
	return m_restitution;
}
