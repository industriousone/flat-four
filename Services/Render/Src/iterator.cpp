/**
 * @file iterator.cpp
 *
 * @author  Jason Perkins (jason@379.com)
 * @date    Copyright (C) 2001 by 379, Inc.
 * @version $Id: iterator.cpp,v 1.3 2002/06/12 23:05:16 jason379 Exp $
 *
 * @todo Clean up the vbi->vb execute call, vb execute code
 *
 */
#include <Core/core.h>
#include <Render/iterator.h>
#include <assert.h>
#include <Memory/memory.h>
using namespace f4;


struct IteratorImpl : public VBufIterator
{
	IteratorImpl();
	virtual ~IteratorImpl();

	virtual bool initialize(VertexBuffer* vb, int size);
	virtual void setPrimitiveType(int type);
	virtual void set(int i, int vertexIndex);
	
	virtual int     size() const;
	virtual int     getPrimitiveType() const;
	virtual UInt32* getData();
	virtual VertexBuffer* getVertexBuffer();

	// Geometry interface
	virtual void getExtents(Vector3* min, Vector3* max);
	virtual void execute();

private:
	Ref<VertexBuffer> m_vBuf;
	UInt32* m_indices;
	UInt32  m_count;
	int     m_primitiveType;
	DECL_SERVICE;
};

IMPL_SERVICE(IteratorImpl, VBufIterator)

//---------------------------------------------------------------------------

IteratorImpl::IteratorImpl()
: m_indices(0),
  m_count(0),
  m_primitiveType(TRIANGLES)
{
}

//---------------------------------------------------------------------------

IteratorImpl::~IteratorImpl()
{
	if (m_indices) free(m_indices);
}

//---------------------------------------------------------------------------

bool IteratorImpl::initialize(VertexBuffer* vb, int size)
{
	assert(size >= 0);

	m_vBuf = vb;

	if (m_indices) free(m_indices);
	m_indices = (UInt32*)malloc(size * sizeof(UInt32));
	m_count = size;
	
	return true;
}

//---------------------------------------------------------------------------

void IteratorImpl::setPrimitiveType(int type)
{
	if (type == TRIANGLES || type == QUADS)
		m_primitiveType = type;
}

//---------------------------------------------------------------------------

void IteratorImpl::set(int i, int vertexIndex)
{
	if (i < 0 || i >= (int)m_count) return;
	m_indices[i] = vertexIndex;
}

//---------------------------------------------------------------------------

int IteratorImpl::size() const
{
	return m_count;
}

//---------------------------------------------------------------------------

int IteratorImpl::getPrimitiveType() const
{
	return m_primitiveType;
}

//---------------------------------------------------------------------------

UInt32* IteratorImpl::getData()
{
	return m_indices;
}

//---------------------------------------------------------------------------

VertexBuffer* IteratorImpl::getVertexBuffer()
{
	return m_vBuf;
}

//---------------------------------------------------------------------------

void IteratorImpl::getExtents(Vector3* min, Vector3* max)
{
	if (m_vBuf) 
		m_vBuf->getExtents(min, max);
	else
		*min = *max = Vector3(0, 0, 0);
}

//---------------------------------------------------------------------------

void IteratorImpl::execute()
{
	if (m_vBuf) m_vBuf->execute(this);
}
