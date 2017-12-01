/**
 * @file surface.cpp
 *
 * @author  Jason Perkins (jason@379.com)
 * @date    Copyright (C) 2002 by 379, Inc.
 * @version $Id: surface.cpp,v 1.2 2002/06/19 01:39:59 jason379 Exp $
 */
#include <Core/core.h>
#include <Scene/surface.h>
#include <Scene/update_visitor.h>
#include <Scene/collision_visitor.h>
#include <Collider/shape.h>
#include <vector>
#include <Memory/memory.h>
using namespace f4;


struct SurfaceImpl : public Surface
{
	virtual void addBoundingShape(BoundingShape* bs);
	virtual int  countBoundingShapes() const;
	virtual BoundingShape* getBoundingShape(int i);

	virtual void setContactPhysics(ContactPhysics* cp);
	virtual ContactPhysics* getContactPhysics();

	// Attribute interface
	virtual void update(UpdateVisitor* uv);
	virtual void collide(CollisionVisitor* cv);

private:
	// The list of bounding shapes
	typedef std::vector< Ref<BoundingShape> > Shapes;
	Shapes m_shapes;

	Ref<ContactPhysics> m_physics;

	DECL_SERVICE;
};

IMPL_SERVICE(SurfaceImpl, Surface)

//---------------------------------------------------------------------------

void SurfaceImpl::addBoundingShape(BoundingShape* bs)
{
	m_shapes.push_back(bs);
}

//---------------------------------------------------------------------------

int SurfaceImpl::countBoundingShapes() const
{
	return m_shapes.size();
}

//---------------------------------------------------------------------------

BoundingShape* SurfaceImpl::getBoundingShape(int i)
{
	if (i < 0 || i >= (int)m_shapes.size()) return 0;
	return m_shapes[i];
}

//---------------------------------------------------------------------------

void SurfaceImpl::setContactPhysics(ContactPhysics* cp)
{
	m_physics = cp;
}

//---------------------------------------------------------------------------

ContactPhysics* SurfaceImpl::getContactPhysics()
{
	return m_physics;
}

//---------------------------------------------------------------------------

void SurfaceImpl::update(UpdateVisitor* uv)
{
	Shapes::iterator it;
	for (it = m_shapes.begin(); it != m_shapes.end(); ++it)
		(*it)->setMatrix(uv->getWorldMatrix());
}

//---------------------------------------------------------------------------

void SurfaceImpl::collide(CollisionVisitor* cv)
{
	cv->test(this);
}
