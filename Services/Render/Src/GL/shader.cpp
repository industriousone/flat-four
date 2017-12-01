/**
 * @file shader.cpp
 *
 * @author  Jason Perkins (jason@379.com)
 * @date    Copyright (C) 2001 by 379, Inc.
 * @version $Id: shader.cpp,v 1.2 2002/04/22 01:51:56 jason379 Exp $
 */
#include "opengl.h"

#include <Core/core.h>
#include <Render/shader.h>
#include <Render/texture.h>
#include <Math/math.h>
#include <Memory/memory.h>
using namespace f4;


struct ShaderImpl : public Shader
{
	ShaderImpl();
	virtual ~ShaderImpl() { }

	virtual void enableAlphaBlend();
	virtual void enableDepthTest();

	virtual void disableAlphaBlend();
	virtual void disableDepthTest();

	virtual void setAlphaBlend(BlendOps src, BlendOps dst);

	virtual void setColor(Float red, Float green, Float blue);
	virtual void setAlpha(Float alpha);

	virtual void setTexture(Texture* t);
	virtual Texture* getTexture();
	virtual void setTextureStyle(TextureStyles style);

	virtual void execute();

private:
	UInt32 m_flags;

	GLenum m_srcBlend, m_dstBlend;
	Float m_color[4];

	Ref<Texture> m_texture;
	unsigned     m_textureStyle;

	GLenum _convertBlendOps(int op);

	DECL_SERVICE;
};

IMPL_SERVICE(ShaderImpl, Shader)



//---------------------------------------------------------------------------
// Local stuff
//---------------------------------------------------------------------------

enum 
{
	ALPHA = 0x01,
	DEPTH = 0x02
};




//---------------------------------------------------------------------------
// Message text
//---------------------------------------------------------------------------

static const Char* BAD_BLEND_OPS = "Shader: invalid blend ops\n";



//---------------------------------------------------------------------------

ShaderImpl::ShaderImpl()
: m_flags(DEPTH),
  m_srcBlend(GL_SRC_ALPHA), 
  m_dstBlend(GL_ONE_MINUS_SRC_ALPHA),
  m_textureStyle(0)
{
	m_color[0] = 1.0;
	m_color[1] = 1.0;
	m_color[2] = 1.0;
	m_color[3] = 1.0;
}

//---------------------------------------------------------------------------

void ShaderImpl::enableAlphaBlend()
{
	m_flags |= ALPHA;
}

//---------------------------------------------------------------------------

void ShaderImpl::enableDepthTest()
{
	m_flags |= DEPTH;
}

//---------------------------------------------------------------------------

void ShaderImpl::disableAlphaBlend()
{
	m_flags &= ~ALPHA;
}

//---------------------------------------------------------------------------

void ShaderImpl::disableDepthTest()
{
	m_flags &= ~DEPTH;
}

//---------------------------------------------------------------------------

void ShaderImpl::setAlphaBlend(BlendOps src, BlendOps dst)
{
	int s = _convertBlendOps(src);
	int d = _convertBlendOps(dst);

	if (!s || !d)
	{
		Core->print(BAD_BLEND_OPS);
		return;
	}

	m_srcBlend = s;
	m_dstBlend = d;
}

//---------------------------------------------------------------------------

void ShaderImpl::setColor(Float r, Float g, Float b)
{
	if (r < 0) r = 0;  if (r > 1) r = 1;  m_color[0] = r;
	if (g < 0) g = 0;  if (g > 1) g = 1;  m_color[1] = g;
	if (b < 0) b = 0;  if (b > 1) b = 1;  m_color[2] = b;
}

//---------------------------------------------------------------------------

void ShaderImpl::setAlpha(Float a)
{
	if (a < 0) a = 0;  if (a > 1) a = 1;  m_color[3] = a;
}

//---------------------------------------------------------------------------

void ShaderImpl::setTexture(Texture* t)
{
	m_texture = t;
	if (t && t->hasAlpha())
	{
		enableAlphaBlend();
	}
}

//---------------------------------------------------------------------------

Texture* ShaderImpl::getTexture()
{
	return m_texture;
}

//---------------------------------------------------------------------------

void ShaderImpl::setTextureStyle(TextureStyles style)
{
	m_textureStyle = style;
}

//---------------------------------------------------------------------------

void ShaderImpl::execute()
{
	glColor4fv(m_color);

	// --- alpha test ---
	if (m_flags & ALPHA)
	{
		glEnable(GL_BLEND);
		glEnable(GL_ALPHA_TEST);
		glBlendFunc(m_srcBlend, m_dstBlend);
		glAlphaFunc(GL_GREATER, 0);
	}
	else
	{
		glDisable(GL_BLEND);
		glDisable(GL_ALPHA_TEST);
	}

	// --- z test ---
	if (m_flags & DEPTH)
	{
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
	}
	else
	{
		glDisable(GL_DEPTH_TEST);
	}

	// --- texture ---
	if (m_texture)
	{
		glEnable(GL_TEXTURE_2D);
		m_texture->execute();

		GLenum val = (m_textureStyle & DECAL) ? GL_DECAL : GL_MODULATE;
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, val);

		val = (m_textureStyle & CLAMP) ? GL_CLAMP : GL_REPEAT;
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, val);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, val);
	}
	else
	{
		glDisable(GL_TEXTURE_2D);
	}
}

//---------------------------------------------------------------------------

GLenum ShaderImpl::_convertBlendOps(int op)
{
	switch (op)
	{
	case ZERO:          return GL_ZERO;
	case ONE:           return GL_ONE;
	case DST_COLOR:     return GL_DST_COLOR;
	case INV_DST_COLOR: return GL_ONE_MINUS_DST_COLOR;
	case SRC_COLOR:     return GL_SRC_COLOR;
	case INV_SRC_COLOR: return GL_ONE_MINUS_SRC_COLOR;
	case DST_ALPHA:     return GL_DST_ALPHA;
	case INV_DST_ALPHA: return GL_ONE_MINUS_DST_ALPHA;
	case SRC_ALPHA:     return GL_SRC_ALPHA;
	case INV_SRC_ALPHA: return GL_ONE_MINUS_SRC_ALPHA;
	}

	return 0;
}
