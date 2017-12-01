/**
 * @file sprite.cpp
 *
 * @author  Jason Perkins (jason@379.com)
 * @date    Copyright (C) 2001 by 379, Inc.
 * @version $Id: sprite.cpp,v 1.2 2002/04/22 01:51:56 jason379 Exp $
 */
#include "opengl.h"

#include <Core/core.h>
#include <Render/sprite.h>
#include <Math/math.h>
#include <Memory/memory.h>
using namespace f4;


struct SpriteImpl : public Sprite
{
	SpriteImpl();
	virtual ~SpriteImpl() { }

	virtual bool initialize(Float width, Float height);

	virtual void  setWidth(Float width);
	virtual Float getWidth() const;
	virtual void  setHeight(Float height);
	virtual Float getHeight() const;
	
	virtual void flipX();
	virtual void flipY();
	virtual bool isXFlipped();
	virtual bool isYFlipped();

	// Geometry interface
	virtual void getExtents(Vector3* min, Vector3* max);
	virtual void execute();

private:
	Float m_halfWidth, m_halfHeight;
	DECL_SERVICE;
};

IMPL_SERVICE(SpriteImpl, Sprite)

//---------------------------------------------------------------------------

SpriteImpl::SpriteImpl()
: m_halfWidth(0.5),
  m_halfHeight(0.5)
{
}

//---------------------------------------------------------------------------

bool SpriteImpl::initialize(Float width, Float height)
{
	m_halfWidth = width * 0.5;
	m_halfHeight = height * 0.5;
	return true;
}

//---------------------------------------------------------------------------

void SpriteImpl::setWidth(Float width)
{
	if (m_halfWidth < 0) width = -width;
	m_halfWidth = width / 2;
}

//---------------------------------------------------------------------------

Float SpriteImpl::getWidth() const
{
	return (m_halfWidth < 0) ? -m_halfWidth : m_halfWidth;
}

//---------------------------------------------------------------------------

void SpriteImpl::setHeight(Float height)
{
	if (m_halfHeight < 0) height = -height;
	m_halfHeight = height / 2;
}

//---------------------------------------------------------------------------

Float SpriteImpl::getHeight() const
{
	return (m_halfHeight < 0) ? -m_halfHeight : m_halfHeight;
}

//---------------------------------------------------------------------------

void SpriteImpl::flipX()
{
	m_halfWidth = -m_halfWidth;
}

//---------------------------------------------------------------------------

void SpriteImpl::flipY()
{
	m_halfHeight = -m_halfHeight;
}

//---------------------------------------------------------------------------

bool SpriteImpl::isXFlipped()
{
	return (m_halfWidth < 0);
}

//---------------------------------------------------------------------------

bool SpriteImpl::isYFlipped()
{
	return (m_halfHeight < 0);
}

//---------------------------------------------------------------------------

void SpriteImpl::getExtents(Vector3* min, Vector3* max)
{
	if (m_halfWidth > 0)
	{
		min->x = -m_halfWidth;
		max->x =  m_halfWidth;
	}
	else
	{
		min->x =  m_halfWidth;
		max->x = -m_halfWidth;
	}

	if (m_halfHeight > 0)
	{
		min->y = -m_halfHeight;
		max->y =  m_halfHeight;
	}
	else
	{
		min->y =  m_halfHeight;
		max->y = -m_halfHeight;
	}

	min->z = 0;
	max->z = 0;
}

//---------------------------------------------------------------------------

void SpriteImpl::execute()
{
	glNormal3f(0, 0, -1);

	glBegin(GL_QUADS);
		glTexCoord2f(0, 1);   
		glVertex2f(-m_halfWidth,  m_halfHeight);

		glTexCoord2f(0, 0);   
		glVertex2f(-m_halfWidth, -m_halfHeight);
	
		glTexCoord2f(1, 0);
		glVertex2f( m_halfWidth, -m_halfHeight);

		glTexCoord2f(1, 1);
		glVertex2f( m_halfWidth,  m_halfHeight);
	glEnd();
}
