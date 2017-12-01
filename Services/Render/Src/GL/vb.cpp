/**
 * @file vb.cpp
 *
 * @author  Jason Perkins (jason@379.com)
 * @date    Copyright (C) 2001 by 379, Inc.
 * @version $Id: vb.cpp,v 1.3 2002/06/12 23:05:16 jason379 Exp $
 */
#include <assert.h>
#include "opengl.h"

#include <Core/core.h>
#include <Render/iterator.h>
#include <Math/vector.h>
#include <Memory/memory.h>
using namespace f4;


struct VBImpl : public VertexBuffer
{
	VBImpl();
	virtual ~VBImpl();

	virtual bool initialize(int format, int count);
	virtual void setPrimitiveType(int type);
	virtual bool resize(int count);
	virtual int  size() const;

	virtual bool setPosition(int i, Float x, Float y, Float z);
	virtual bool setNormal(int i, Float x, Float y, Float z);
	virtual bool setTexCoord(int i, Float u, Float v);
	virtual bool setColor(int i, Float r, Float g, Float b, Float a);

	virtual const Vector3 getPosition(int i);
	virtual const Vector3 getTexCoord(int i);

	virtual void  execute(VBufIterator* vbi);

	// Geometry interface
	virtual void getExtents(Vector3* min, Vector3* max);
	virtual void execute();

private:
	int    m_format;
	Float* m_buffer;
	int    m_count;
	int    m_maxCount;
	int    m_primitiveType;

	int    _calcSize();
	Float* _ptrTo(int i, int component);

	DECL_SERVICE;
};

IMPL_SERVICE(VBImpl, VertexBuffer)

//---------------------------------------------------------------------------

VBImpl::VBImpl()
: m_format(0),
  m_buffer(0),
  m_count(0),
  m_maxCount(0),
  m_primitiveType(GL_TRIANGLES)
{
}

//---------------------------------------------------------------------------

VBImpl::~VBImpl()
{
	if (m_buffer) free(m_buffer);
}

//---------------------------------------------------------------------------

bool VBImpl::initialize(int format, int count)
{
	if (m_buffer) free(m_buffer);

	m_format   = format | POSITION;
	m_buffer   = (Float*)malloc(_calcSize() * count);
	m_count    = count;
	m_maxCount = count;

	return true;
}

//---------------------------------------------------------------------------

void VBImpl::setPrimitiveType(int type)
{
	switch (type)
	{
	case TRIANGLES:
		m_primitiveType = GL_TRIANGLES;
		break;

	case QUADS:
		m_primitiveType = GL_QUADS;
		break;
	}
}

//---------------------------------------------------------------------------

bool VBImpl::resize(int count)
{
	if (count <= m_maxCount)
	{
		m_count = count;
		return true;
	}

	m_buffer   = (Float*)realloc(m_buffer, _calcSize() * count);
	m_count    = count;
	m_maxCount = count;

	return true;
}

//---------------------------------------------------------------------------

int VBImpl::size() const
{
	return m_count;
}

//---------------------------------------------------------------------------

bool VBImpl::setPosition(int i, Float x, Float y, Float z)
{
	Float* pos = _ptrTo(i, POSITION);
	if (pos)
	{
		pos[0] = x;
		pos[1] = y;
		pos[2] = z;
		return true;
	}

	return false;
}

//---------------------------------------------------------------------------

bool VBImpl::setNormal(int i, Float x, Float y, Float z)
{
	Float* nrm = _ptrTo(i, NORMAL);
	if (nrm)
	{
		nrm[0] = x;
		nrm[1] = y;
		nrm[2] = z;
		return true;
	}

	return false;
}

//---------------------------------------------------------------------------

bool VBImpl::setTexCoord(int i, Float u, Float v)
{
	Float* txc = _ptrTo(i, TEXCOORD);
	if (txc)
	{
		txc[0] = u;
		txc[1] = v;
		return true;
	}

	return false;
}
	
//---------------------------------------------------------------------------

bool VBImpl::setColor(int i, Float r, Float g, Float b, Float a)
{
	UInt8* clr = (UInt8*)_ptrTo(i, COLOR);
	if (clr)
	{
		clr[0] = static_cast<UInt8>(r * 255.0);
		clr[1] = static_cast<UInt8>(g * 255.0);
		clr[2] = static_cast<UInt8>(b * 255.0);
		clr[3] = static_cast<UInt8>(a * 255.0);
		return true;
	}

	return false;
}

//---------------------------------------------------------------------------

const Vector3 VBImpl::getPosition(int i)
{
	assert(i >= 0 && i < m_count);

	Vector3* pos = (Vector3*)_ptrTo(i, POSITION);
	return *pos;
}

//---------------------------------------------------------------------------

const Vector3 VBImpl::getTexCoord(int i)
{
	assert(i >= 0 && i < m_count);

	Vector2* tc = (Vector2*)_ptrTo(i, TEXCOORD);
	return Vector3(tc->x, tc->y, 0);
}

//---------------------------------------------------------------------------

void VBImpl::getExtents(Vector3* min, Vector3* max)
{
	if (m_count == 0)
	{
		min->x = 0;
		min->y = 0;
		min->z = 0;
		max->x = 0;
		max->y = 0;
		max->z = 0;
	}
	else
	{
		int size = _calcSize() / sizeof(Float);
		Float* ptr = m_buffer;

		min->x = max->x = ptr[0];    // seed the extents from the first point
		min->y = max->y = ptr[1];
		min->z = max->z = ptr[2];
		ptr += size;

		for (int i = 1; i < m_count; ++i)
		{
			if (ptr[0] < min->x) min->x = ptr[0];
			if (ptr[0] > max->x) max->x = ptr[0];
			if (ptr[1] < min->y) min->y = ptr[1];
			if (ptr[1] > max->x) max->y = ptr[1];
			if (ptr[2] < min->z) min->z = ptr[2];
			if (ptr[2] > max->z) max->z = ptr[2];

			ptr += size;
		}
	}
}

//---------------------------------------------------------------------------

void VBImpl::execute()
{
	execute(0);
}

//---------------------------------------------------------------------------

void VBImpl::execute(VBufIterator* vbi)
{
	int   size = _calcSize();
	Float* ptr = m_buffer;

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, size, ptr);
	ptr += 3;

	if (m_format & NORMAL)
	{
		glEnableClientState(GL_NORMAL_ARRAY);
		glNormalPointer(GL_FLOAT, size, ptr);
		ptr += 3;
	}
	else
	{
		glDisableClientState(GL_NORMAL_ARRAY);
	}

	if (m_format & TEXCOORD)
	{
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glTexCoordPointer(2, GL_FLOAT, size, ptr);
		ptr += 2;
	}
	else
	{
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	}

	if (m_format & COLOR)
	{
		glEnableClientState(GL_COLOR_ARRAY);
		glColorPointer(4, GL_UNSIGNED_BYTE, size, ptr);
		ptr += 1;
	}
	else
	{
		glDisableClientState(GL_COLOR_ARRAY);
	}

	if (vbi)
	{
		int type = vbi->getPrimitiveType();
		if (type == VBufIterator::TRIANGLES)
			type = GL_TRIANGLES;
		else if (type == VBufIterator::QUADS)
			type = GL_QUADS;

/*
		This code is handy for debugging vbuf setup problems.

		glBegin(type);

		UInt32* data = vbi->getData();
		for (int i = 0; i < vbi->size(); ++i)
		{
			Float* vv = (Float*)((UInt8*)m_buffer + (data[i] * size));
			
			Float* vp = 0;
			Float* tp = 0;

			vp = vv;
			vv += 3;

			if (m_format & TEXCOORD)
			{
				tp = vv;
				vv += 2;
			}

			if (tp)
				glTexCoord2f(tp[0], tp[1]);

			glVertex3f(vp[0], vp[1], vp[2]);
		}

		glEnd();
*/
		glDrawElements(type, vbi->size(), GL_UNSIGNED_INT, vbi->getData());
	}
	else
		glDrawArrays(m_primitiveType, 0, m_count);
}

//---------------------------------------------------------------------------

int VBImpl::_calcSize()
{
	int size = sizeof(Vector3);           // position is always included
	if (m_format & NORMAL)   size += sizeof(Vector3);
	if (m_format & TEXCOORD) size += sizeof(Vector2);
	if (m_format & COLOR)    size += sizeof(UInt32);

	return size;
}

//---------------------------------------------------------------------------

Float* VBImpl::_ptrTo(int i, int component)
{
	if (i < 0 || i >= m_count) return 0;
	if ((m_format & component) == 0) return 0;

	int size = _calcSize() / 4;

	Float* vertex = m_buffer + size * i;

	if (component == POSITION) return vertex;
	vertex += 3;

	if (component == NORMAL) return vertex;
	if (m_format & NORMAL) vertex += 3;

	if (component == TEXCOORD) return vertex;
	if (m_format & TEXCOORD) vertex += 2;

	if (component == COLOR) return vertex;

	return 0;
}

