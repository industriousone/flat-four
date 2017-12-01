/**
 * @file texture.cpp
 *
 * @author  Jason Perkins (jason@379.com)
 * @date    Copyright (C) 2001 by 379, Inc.
 * @version $Id: texture.cpp,v 1.2 2002/04/22 01:51:56 jason379 Exp $
 */
#include "opengl.h"

#include <Core/core.h>
#include <Render/texture.h>
#include <Memory/memory.h>
using namespace f4;


struct TextureImpl : public Texture
{
	TextureImpl();
	virtual ~TextureImpl();

	virtual bool initialize(int width, int height, int num, int size, void* pixels);

	virtual int  getWidth() const;
	virtual int  getHeight() const;
	virtual int  getComponents() const;
	virtual int  getComponentSize() const;
	virtual void* getPixels();

	virtual bool hasAlpha() const;

	virtual void execute();

private:
	int    m_width, m_height;
	int    m_num, m_size;
	void*  m_pixels;
	
	GLuint m_id;

	DECL_SERVICE;
};

IMPL_SERVICE(TextureImpl, Texture)


//---------------------------------------------------------------------------
// Message text
//---------------------------------------------------------------------------

static const Char* BAD_NUM  = "Texture: invalid number of components (%d)\n";
static const Char* BAD_SIZE = "Texture: invalid component size (%d)\n";
static const Char* GL_FAIL  = "Texture: unable to upload bitmap (%d)\n";


//---------------------------------------------------------------------------

TextureImpl::TextureImpl()
: m_width(0), m_height(0),
  m_num(0), m_size(0),
  m_pixels(0),
  m_id(0)
{
}


//---------------------------------------------------------------------------

TextureImpl::~TextureImpl()
{
	if (m_pixels) free(m_pixels);
}

//---------------------------------------------------------------------------

bool TextureImpl::initialize(int width, int height, int num, int size, void* pixels)
{
	// Validate the input arguments
	if (num < 1 || num > 4)
	{
		Core->print(BAD_NUM, num);
		return false;
	}

	if (size != 1 && size != 2 && size != 4)
	{
		Core->print(BAD_SIZE, size);
		return false;
	}

	m_width  = width;
	m_height = height;
	m_num    = num;
	m_size   = size;
	m_pixels = pixels;

	return true;
}

//---------------------------------------------------------------------------

int TextureImpl::getWidth() const
{
	return m_width;
}

//---------------------------------------------------------------------------

int TextureImpl::getHeight() const
{
	return m_height;
}

//---------------------------------------------------------------------------

int TextureImpl::getComponents() const
{
	return m_num;
}

//---------------------------------------------------------------------------

int TextureImpl::getComponentSize() const
{
	return m_size;
}

//---------------------------------------------------------------------------

void* TextureImpl::getPixels()
{
	return m_pixels;
}

//---------------------------------------------------------------------------

bool TextureImpl::hasAlpha() const
{
	return (m_num != 3);
}

//---------------------------------------------------------------------------

void TextureImpl::execute()
{
	// If this is the first use of the texture, upload it to OpenGL
	if (!m_id)
	{
		GLenum size   = 0;
		GLenum data   = 0;
		GLenum format = 0;

		switch (m_size)
		{
		case 1:  size = GL_UNSIGNED_BYTE;  break;
		case 2:  size = GL_UNSIGNED_SHORT; break;
		case 4:  size = GL_FLOAT;          break;
		}

		if (m_size < 4)
		{
			switch (m_num)
			{
			case 1:
				format = GL_ALPHA;
				data   = (m_size==1) ? GL_ALPHA8 : GL_ALPHA16;  
				break;

			case 2:
				format = GL_LUMINANCE_ALPHA;
				data   = (m_size==1) ? GL_LUMINANCE8_ALPHA8 : GL_LUMINANCE16_ALPHA16;
				break;

			case 3:
				format = GL_RGB;
				data   = (m_size==1) ? GL_RGB8 : GL_RGB16;
				break;

			case 4:
				format = GL_RGBA;
				data   = (m_size==1) ? GL_RGBA8 : GL_RGBA16;
				break;
			}
		}
		else
		{
			data = m_size;
		}

		glGenTextures(1, &m_id);
		glBindTexture(GL_TEXTURE_2D, m_id);

		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, data, m_width, m_height, 0, format, size, m_pixels);
		int err = glGetError();
		if (err != 0) Core->print(GL_FAIL, err);
	}
	else
	{
		glBindTexture(GL_TEXTURE_2D, m_id);
	}
}
