/**
 * @file visual.cpp
 *
 * @author  Jason Perkins (jason@379.com)
 * @date    Copyright (C) 2001 by 379, Inc.
 * @version $Id: visual.cpp,v 1.2 2002/04/22 01:51:56 jason379 Exp $
 *
 * @todo The text looks kind of uneven. Tweak the generated coordinates to
 *       make it pixel perfect on 1:1 2D coordinates.
 *
 */
#include <Core/core.h>
#include <Text/visual.h>
#include <Text/texture_font.h>
#include <Render/vb.h>
#include <Math/vector.h>
#include "glyph.h"
#include <Memory/memory.h>
using namespace f4;


struct TextVisualImpl : public TextVisual
{
	TextVisualImpl();
	virtual ~TextVisualImpl() { }

	virtual bool initialize(TextureFont* font, Shader* shader);
	virtual void setText(const Char* text);
	virtual void setAlignment(int horiz, int vert);

	// Visual interface
	virtual bool      initialize(Geometry* geom, Shader* shader);
	virtual Geometry* getGeometry();
	virtual Shader*   getShader();
	virtual void      execute();

private:
	Ref<TextureFont> m_font;
	Ref<Visual>      m_viz;
	int m_horizAlign, m_vertAlign;

	void _adjustAlignment(Float w, Float h, int oldHoriz, int oldVert);

	DECL_SERVICE;
};

IMPL_SERVICE(TextVisualImpl, TextVisual)


//---------------------------------------------------------------------------
// Local stuff
//---------------------------------------------------------------------------

struct Glyph
{
	int x, y;
	int width, height;
	int advance;
	int left, bottom;
};



//---------------------------------------------------------------------------

TextVisualImpl::TextVisualImpl()
: m_horizAlign(LEFT),
  m_vertAlign(BOTTOM)
{
}

//---------------------------------------------------------------------------

bool TextVisualImpl::initialize(TextureFont* font, Shader* shader)
{
	m_font = font;

	VertexBuffer* vb = (VertexBuffer*)Core->createA("Render.VertexBuffer");
	vb->initialize(VertexBuffer::TEXCOORD, 0);
	vb->setPrimitiveType(VertexBuffer::QUADS);

	m_viz = (Visual*)Core->createA("Render.Visual");
	m_viz->initialize(vb, shader);
	return true;
}

//---------------------------------------------------------------------------

void TextVisualImpl::setText(const Char* text)
{
	int length = strlen(text);

	VertexBuffer* vb = static_cast<VertexBuffer*>(m_viz->getGeometry());
	vb->resize(length * 4);

	Texture* tex = m_viz->getShader()->getTexture();
	Float texWidth = tex->getWidth();
	Float texHeight = tex->getHeight();

	Float adjust = 1 / (texHeight * 2);

	int i = 0;
	int x = 0;
	int y = 0;
	Glyph g;
	for (const Char* ch = text; *ch; ++ch)
	{
		if (m_font->getGlyphInfo(*ch, (int*)&g))
		{
			Float u1 = (Float)g.x / texWidth;
			Float v1 = (Float)g.y / texHeight;
			Float u2 = u1 + (Float)g.width / texWidth + adjust;
			Float v2 = v1 + (Float)g.height / texHeight + adjust;

			Float left   = x + g.left;
			Float bottom = y + g.bottom;

			vb->setPosition(i, left, bottom - g.height, 0);
			vb->setTexCoord(i, u1, v2);
			++i;

			vb->setPosition(i, left, bottom, 0);
			vb->setTexCoord(i, u1, v1);
			++i;

			vb->setPosition(i, left + g.width, bottom, 0);
			vb->setTexCoord(i, u2, v1);
			++i;

			vb->setPosition(i, left + g.width, bottom - g.height, 0);
			vb->setTexCoord(i, u2, v2);
			++i;

			x += g.advance;
		}
	}

	// Correct the coordinates for the current alignment
	_adjustAlignment(x, y + m_font->getHeight(), LEFT, BOTTOM);
}

//---------------------------------------------------------------------------

void TextVisualImpl::setAlignment(int horiz, int vert)
{
	int oldHoriz = m_horizAlign;
	int oldVert  = m_vertAlign;

	if (horiz == LEFT || horiz == CENTER || horiz == RIGHT)
		m_horizAlign = horiz;

	if (vert == TOP || vert == CENTER || vert == BOTTOM)
		m_vertAlign = vert;

	Vector3 min, max;
	VertexBuffer* vb = static_cast<VertexBuffer*>(m_viz->getGeometry());
	vb->getExtents(&min, &max);
	
	_adjustAlignment(max.x - min.x, max.y - min.y, oldHoriz, oldVert);
}

//---------------------------------------------------------------------------

bool TextVisualImpl::initialize(Geometry* geom, Shader* shader)
{
	return m_viz->initialize(geom, shader);
}

//---------------------------------------------------------------------------

Geometry* TextVisualImpl::getGeometry()
{
	return m_viz->getGeometry();
}

//---------------------------------------------------------------------------

Shader* TextVisualImpl::getShader()
{
	return m_viz->getShader();
}

//---------------------------------------------------------------------------

void TextVisualImpl::execute()
{
	m_viz->execute();
}

//---------------------------------------------------------------------------

void TextVisualImpl::_adjustAlignment(Float w, Float h, int oldHoriz, int oldVert)
{
	Float xOffset = 0;
	Float yOffset = 0;

	switch (oldHoriz)
	{
	case LEFT:
		if (m_horizAlign == CENTER)
			xOffset = -(w / 2);
		else if (m_horizAlign == RIGHT)
			xOffset = -w;
		break;

	case CENTER:
		if (m_horizAlign == LEFT)
			xOffset = w / 2;
		else if (m_horizAlign == RIGHT)
			xOffset = -(w / 2);
		break;

	case RIGHT:
		if (m_horizAlign == LEFT)
			xOffset = w;
		else if (m_horizAlign == CENTER)
			xOffset = w / 2;
		break;
	}

	switch (oldVert)
	{
	case TOP:
		if (m_vertAlign == CENTER)
			yOffset = h / 2;
		else if (m_vertAlign == BOTTOM)
			yOffset = h;
		break;

	case CENTER:
		if (m_vertAlign == TOP)
			yOffset = -(h / 2);
		else if (m_vertAlign == BOTTOM)
			yOffset = h / 2;
		break;

	case BOTTOM:
		if (m_vertAlign == TOP)
			yOffset = -h;
		else if (m_vertAlign == CENTER)
			yOffset = -(h / 2);
		break;
	}

	if (xOffset == 0 && yOffset == 0) return;

	VertexBuffer* vb = static_cast<VertexBuffer*>(m_viz->getGeometry());
	for (int i = 0; i < vb->size(); ++i)
	{
		const Vector3& pos = vb->getPosition(i);
		vb->setPosition(i, pos.x + xOffset, pos.y + yOffset, pos.z);
	}
}
