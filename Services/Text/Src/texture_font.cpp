/**
 * @file texture_font.cpp
 *
 * @author  Jason Perkins (jason@379.com)
 * @date    Copyright (C) 2001 by 379, Inc.
 * @version $Id: texture_font.cpp,v 1.2 2002/04/22 01:51:56 jason379 Exp $
 */
#include <Core/core.h>
#include <map>
#include <Text/texture_font.h>
#include <Render/vb.h>
#include "glyph.h"
#include <Memory/memory.h>
using namespace f4;

DECLARE_SERVICE(TextVisual)


struct TexFontImpl : public TextureFont
{
	TexFontImpl();
	virtual ~TexFontImpl() { }

	virtual bool initialize(int height, Texture* texture);

	virtual TextVisual* createVisual();

	virtual void setGlyph(int code, int x, int y, int w, int h, int advance, int left, int bottom);
	virtual int  countGlyphs() const;
	virtual void getGlyphCodes(int* codes);
	virtual bool getGlyphInfo(int code, int* glyph);

	virtual int      getHeight() const;
	virtual Texture* getTexture();

private:
	int m_height;

	struct Glyph
	{
		Int16 x, y;
		Int16 width, height;
		Int16 advance;
		Int8  left, bottom;
	};

	typedef std::map<UInt16, Glyph> Glyphs;
	Glyphs m_glyphs;

	Ref<Texture>  m_texture;
	DECL_SERVICE;
};

IMPL_SERVICE(TexFontImpl, TextureFont)

//---------------------------------------------------------------------------

TexFontImpl::TexFontImpl()
: m_height(0)
{
}

//---------------------------------------------------------------------------

bool TexFontImpl::initialize(int height, Texture* texture)
{
	m_height = height;
	m_texture = texture;
	return true;
}

//---------------------------------------------------------------------------

TextVisual* TexFontImpl::createVisual()
{
	Shader* shader = (Shader*)Core->createA("Render.Shader");
	shader->setTexture(m_texture);
	shader->enableAlphaBlend();

	TextVisual* viz = CREATEA_LOCAL(TextVisual);
	viz->initialize(this, shader);
	return viz;
}

//---------------------------------------------------------------------------

void TexFontImpl::setGlyph(int code, int x, int y, int w, int h, int advance, int left, int bottom)
{
	Glyph g;
	g.x = x;
	g.y = y;
	g.width = w;
	g.height = h;
	g.advance = advance;
	g.left = left;
	g.bottom = bottom;

	m_glyphs.insert(Glyphs::value_type(code, g));
}

//---------------------------------------------------------------------------

int TexFontImpl::countGlyphs() const
{
	return m_glyphs.size();
}

//---------------------------------------------------------------------------

void TexFontImpl::getGlyphCodes(int* codes)
{
	Glyphs::iterator i;
	for (i = m_glyphs.begin(); i != m_glyphs.end(); ++i)
	{
		*(codes++) = (*i).first;
	}
}

//---------------------------------------------------------------------------

bool TexFontImpl::getGlyphInfo(int code, int* glyph)
{
	Glyphs::iterator i = m_glyphs.find(code);
	if (i == m_glyphs.end()) return false;

	glyph[0] = (*i).second.x;
	glyph[1] = (*i).second.y;
	glyph[2] = (*i).second.width;
	glyph[3] = (*i).second.height;
	glyph[4] = (*i).second.advance;
	glyph[5] = (*i).second.left;
	glyph[6] = (*i).second.bottom;
	return true;
}

//---------------------------------------------------------------------------

int TexFontImpl::getHeight() const
{
	return m_height;
}

//---------------------------------------------------------------------------

Texture* TexFontImpl::getTexture()
{
	return m_texture;
}
