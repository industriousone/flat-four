/**
 * @file body.cpp
 *
 * @author  Jason Perkins (jason@379.com)
 * @date    Copyright (C) 2002 by 379, Inc.
 * @version $Id: body.cpp,v 1.3 2002/06/19 01:39:59 jason379 Exp $
 */
#include <Core/core.h>
#include <Scene/body.h>
#include <Scene/entity.h>
#include <Scene/render_visitor.h>
#include <vector>
#include <Memory/memory.h>
using namespace f4;

struct BodyImpl : public Body
{
	virtual void addVisual(Visual* viz);
	virtual int  countVisuals() const;
	virtual Visual* getVisual(int i);

	// Attribute interface
	void render(RenderVisitor* rv);

private:
	typedef std::vector< Ref<Visual> > Visuals;
	Visuals m_visuals;

	DECL_SERVICE;
};

IMPL_SERVICE(BodyImpl, Body)

//---------------------------------------------------------------------------

void BodyImpl::addVisual(Visual* visual)
{
	if (visual) 
		m_visuals.push_back(visual);
}

//---------------------------------------------------------------------------

int BodyImpl::countVisuals() const
{
	return m_visuals.size();
}

//---------------------------------------------------------------------------

Visual* BodyImpl::getVisual(int i)
{
	if (i < 0 || i >= (int)m_visuals.size()) return 0;
	return m_visuals[i];
}

//---------------------------------------------------------------------------

void BodyImpl::render(RenderVisitor* rv)
{
	Visuals::iterator it;
	for (it = m_visuals.begin(); it != m_visuals.end(); ++it)
		rv->queue(*it);
}

