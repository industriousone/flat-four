/**
 * @file cube.cpp
 *
 * @author  Jason Perkins (jason@379.com)
 * @date    Copyright (C) 2001 by 379, Inc.
 * @version $Id: cube.cpp,v 1.2 2002/04/22 01:51:56 jason379 Exp $
 */
#include <Core/core.h>
#include <StdLib/cube.h>
#include <Render/vb.h>
#include <Math/vector.h>
#include <Memory/memory.h>
using namespace f4;


struct CubeImpl : public Cube
{
	virtual bool initialize(Float x, Float y, Float z);

	// Geometry interface
	virtual void getExtents(Vector3* min, Vector3* max);
	virtual void execute();

private:
	Ref<VertexBuffer> m_vb;
	DECL_SERVICE;
};

IMPL_SERVICE(CubeImpl, Cube)

//---------------------------------------------------------------------------

bool CubeImpl::initialize(Float x, Float y, Float z)
{
	m_vb = (VertexBuffer*)Core->createA("Render.VertexBuffer");
	if (!m_vb) return false;

	m_vb->initialize(VertexBuffer::NORMAL | VertexBuffer::TEXCOORD, 24);
	m_vb->setPrimitiveType(VertexBuffer::QUADS);

	Float halfX = x / 2;
	Float halfY = y / 2;
	Float halfZ = z / 2;

	m_vb->setPosition(0, -halfX, halfY, halfZ);
	m_vb->setNormal  (0, 0, 0, 1);
	m_vb->setTexCoord(0, 0, 1);

	m_vb->setPosition(1, -halfX, -halfY, halfZ);
	m_vb->setNormal  (1, 0, 0, 1);
	m_vb->setTexCoord(1, 0, 0);

	m_vb->setPosition(2, halfX, -halfY, halfZ);
	m_vb->setNormal  (2, 0, 0, 1);
	m_vb->setTexCoord(2, 1, 0);

	m_vb->setPosition(3, halfX, halfY, halfZ);
	m_vb->setNormal  (3, 0, 0, 1);
	m_vb->setTexCoord(3, 1, 1);

	m_vb->setPosition(4, halfX, halfY, halfZ);
	m_vb->setNormal  (4, 1, 0, 0);
	m_vb->setTexCoord(4, 0, 1);

	m_vb->setPosition(5, halfX, -halfY, halfZ);
	m_vb->setNormal  (5, 1, 0, 0);
	m_vb->setTexCoord(5, 0, 0);

	m_vb->setPosition(6, halfX, -halfY, -halfZ);
	m_vb->setNormal  (6, 1, 0, 0);
	m_vb->setTexCoord(6, 1, 0);

	m_vb->setPosition(7, halfX, halfY, -halfZ);
	m_vb->setNormal  (7, 1, 0, 0);
	m_vb->setTexCoord(7, 1, 1);

	m_vb->setPosition(8, halfX, halfY, -halfZ);
	m_vb->setNormal  (8, 0, 0, -1);
	m_vb->setTexCoord(8, 0, 1);

	m_vb->setPosition(9, halfX, -halfY, -halfZ);
	m_vb->setNormal  (9, 0, 0, -1);
	m_vb->setTexCoord(9, 0, 0);

	m_vb->setPosition(10, -halfX, -halfY, -halfZ);
	m_vb->setNormal  (10, 0, 0, -1);
	m_vb->setTexCoord(10, 1, 0);

	m_vb->setPosition(11, -halfX, halfY, -halfZ);
	m_vb->setNormal  (11, 0, 0, -1);
	m_vb->setTexCoord(11, 1, 1);

	m_vb->setPosition(12, -halfX, halfY, -halfZ);
	m_vb->setNormal  (12, 1, 0, 0);
	m_vb->setTexCoord(12, 0, 1);

	m_vb->setPosition(13, -halfX, -halfY, -halfZ);
	m_vb->setNormal  (13, 1, 0, 0);
	m_vb->setTexCoord(13, 0, 0);

	m_vb->setPosition(14, -halfX, -halfY, halfZ);
	m_vb->setNormal  (14, 1, 0, 0);
	m_vb->setTexCoord(14, 1, 0);

	m_vb->setPosition(15, -halfX, halfY, halfZ);
	m_vb->setNormal  (15, 1, 0, 0);
	m_vb->setTexCoord(15, 1, 1);

	m_vb->setPosition(16, -halfX, halfY, halfZ);
	m_vb->setNormal  (16, 0, 1, 0);
	m_vb->setTexCoord(16, 0, 1);

	m_vb->setPosition(17, halfX, halfY, halfZ);
	m_vb->setNormal  (17, 0, 1, 0);
	m_vb->setTexCoord(17, 0, 0);

	m_vb->setPosition(18, halfX, halfY, -halfZ);
	m_vb->setNormal  (18, 0, 1, 0);
	m_vb->setTexCoord(18, 1, 0);

	m_vb->setPosition(19, -halfX, halfY, -halfZ);
	m_vb->setNormal  (19, 0, 1, 0);
	m_vb->setTexCoord(19, 1, 1);

	m_vb->setPosition(20, -halfX, -halfY, -halfZ);
	m_vb->setNormal  (20, 0, -1, 0);
	m_vb->setTexCoord(20, 0, 1);

	m_vb->setPosition(21, -halfX, -halfY, halfZ);
	m_vb->setNormal  (21, 0, -1, 0);
	m_vb->setTexCoord(21, 0, 0);

	m_vb->setPosition(22, halfX, -halfY, halfZ);
	m_vb->setNormal  (22, 0, -1, 0);
	m_vb->setTexCoord(22, 1, 0);

	m_vb->setPosition(23, halfX, -halfY, -halfZ);
	m_vb->setNormal  (23, 0, -1, 0);
	m_vb->setTexCoord(23, 1, 1);

	return true;
}

//---------------------------------------------------------------------------

void CubeImpl::getExtents(Vector3* min, Vector3* max)
{
	if (m_vb)
		m_vb->getExtents(min, max);
	else
		*min = *max = Vector3(0, 0, 0);
}

//---------------------------------------------------------------------------

void CubeImpl::execute()
{
	if (m_vb) m_vb->execute(0);
}
