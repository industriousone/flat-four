/**
 * @file visual.cpp
 *
 * @author  Jason Perkins (jason@379.com)
 * @date    Copyright (C) 2001 by 379, Inc.
 * @version $Id: visual.cpp,v 1.2 2002/04/22 01:51:56 jason379 Exp $
 */
#include <Core/core.h>
#include <Render/visual.h>
#include <Memory/memory.h>
using namespace f4;


struct VisualImpl : public Visual
{
	virtual ~VisualImpl() { }

	virtual bool initialize(Geometry* geom, Shader* shader);
	
	virtual Geometry* getGeometry();
	virtual Shader*   getShader();

	virtual void execute();

private:
	Ref<Geometry> m_geom;
	Ref<Shader>   m_shader;

	DECL_SERVICE;
};

IMPL_SERVICE(VisualImpl, Visual)

//---------------------------------------------------------------------------

bool VisualImpl::initialize(Geometry* geom, Shader* shader)
{
	m_geom = geom;
	m_shader = shader;
	return true;
}

//---------------------------------------------------------------------------

Geometry* VisualImpl::getGeometry()
{
	return m_geom;
}

//---------------------------------------------------------------------------

Shader* VisualImpl::getShader()
{
	return m_shader;
}

//---------------------------------------------------------------------------

void VisualImpl::execute()
{
	if (m_shader) m_shader->execute();
	if (m_geom)   m_geom->execute();
}
