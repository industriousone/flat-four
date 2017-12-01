/**
 * @file mass_helper.cpp
 *
 * @author  Jason Perkins (jason@379.com)
 * @date    Copyright (C) 2001 by 379, Inc.
 * @version $Id: mass_helper.cpp,v 1.2 2002/04/22 01:51:56 jason379 Exp $
 */
#include <Core/core.h>
#include <Physics/physics.h>
#include <Physics/mass_helper.h>
#include "impl.h"
#include <Memory/memory.h>
using namespace f4;


struct MassHelperImpl : public MassHelper
{
	MassHelperImpl();
	virtual ~MassHelperImpl() { }

	virtual void setSolidCylinder(Float mass, Float radius, Float width);
	virtual void translate(Float x, Float y, Float z);
	virtual void add(MassHelper* b);

	virtual void* getMassData() { return &m_mass; }

	dMass m_mass;

private:
	DECL_SERVICE;
};

IMPL_SERVICE(MassHelperImpl, MassHelper)

//---------------------------------------------------------------------------

MassHelperImpl::MassHelperImpl()
{
	dMassSetZero(&m_mass);
}

//---------------------------------------------------------------------------

void MassHelperImpl::setSolidCylinder(Float mass, Float radius, Float width)
{
	Float a = radius * radius / 2.0;
	Float b = a + width * width / 12.0;
	dMassSetParameters(&m_mass, mass, 0, 0, 0, a, b, b, 0, 0, 0);
}

//---------------------------------------------------------------------------

void MassHelperImpl::translate(Float x, Float y, Float z)
{
	dMassTranslate(&m_mass, x, y, z);
}

//---------------------------------------------------------------------------

void MassHelperImpl::add(MassHelper* b)
{
	dMassAdd(&m_mass, &((MassHelperImpl*)b)->m_mass);
}
