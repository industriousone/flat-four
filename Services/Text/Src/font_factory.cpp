/**
 * @file font_factory.cpp
 *
 * @author  Jason Perkins (jason@379.com)
 * @date    Copyright (C) 2001 by 379, Inc.
 * @version $Id: font_factory.cpp,v 1.2 2002/04/22 01:51:56 jason379 Exp $
 *
 * @todo Hack FreeType to use F4 memory manager. File I/O too?
 *
 * @todo Once Char has been converted to 'unsigned char' type, remove casts
 *       to UInt8 in createTextureFont().
 *
 */
#include <freetype/freetype.h>

#include <list>

#include <Core/core.h>
#include <Text/font_factory.h>
#include <Text/texture_font.h>
#include <Memory/memory.h>
using namespace f4;

DECLARE_SERVICE(TextureFont)
struct Glyph;


struct FontFactoryImpl : public FontFactory
{
	FontFactoryImpl();
	virtual ~FontFactoryImpl();

	virtual bool initialize();
	virtual bool setTypeface(const Char* filename);
	virtual bool setPixelSize(int pixels);
	virtual void setGlyphs(const Char* glyphs);
	virtual bool setMaxTextureSize(int size);
	virtual void antialias(bool yesNo);

	virtual TextureFont* createTextureFont();

private:
	FT_Library  m_ft;
	FT_Face     m_face;
	int         m_pixels;
	const Char* m_glyphs;
	int         m_maxSize;
	bool        m_antialias;

	bool _placeGlyph(UInt8* mask, int* usedWidth, int* usedHeight, Glyph& g);

	DECL_SERVICE;
};

IMPL_SERVICE(FontFactoryImpl, FontFactory)



//---------------------------------------------------------------------------
// Local stuff
//---------------------------------------------------------------------------

struct Glyph
{
	int code;
	int x, y;
	int width, height;
	int advance;
	int left, bottom;

	bool operator < (Glyph& g)
	{
		return	(width * height > g.width * g.height);
	}
};

typedef std::list<Glyph> Glyphs;



//---------------------------------------------------------------------------

FontFactoryImpl::FontFactoryImpl()
: m_ft(0),
  m_face(0),
  m_pixels(32),
  m_glyphs(0),
  m_maxSize(256),
  m_antialias(true)
{
}

//---------------------------------------------------------------------------

FontFactoryImpl::~FontFactoryImpl()
{
	if (m_ft)
		FT_Done_FreeType(m_ft);
}

//---------------------------------------------------------------------------

bool FontFactoryImpl::initialize()
{
	return (FT_Init_FreeType(&m_ft) == 0);
}

//---------------------------------------------------------------------------

bool FontFactoryImpl::setTypeface(const Char* filename)
{
	if (!m_ft) return false;
	return (FT_New_Face(m_ft, filename, 0, &m_face) == 0);
}

//---------------------------------------------------------------------------

bool FontFactoryImpl::setPixelSize(int pixels)
{
	if (pixels < 1) return false;
	m_pixels = pixels;
	return true;
}

//---------------------------------------------------------------------------

void FontFactoryImpl::setGlyphs(const Char* glyphs)
{
	m_glyphs = glyphs;
}

//---------------------------------------------------------------------------

bool FontFactoryImpl::setMaxTextureSize(int size)
{
	// Enforce the "power of two greater than 4" rule
	int check = 8;
	for (int i = 0; i < 25; ++i)
	{
		if (check == size) break;
		check <<= 1;
	}

	if (check != size) return false;

	m_maxSize = size;
	return true;
}

//---------------------------------------------------------------------------

void FontFactoryImpl::antialias(bool yesNo)
{
	m_antialias = yesNo;
}

//---------------------------------------------------------------------------

TextureFont* FontFactoryImpl::createTextureFont()
{
	if (!m_ft) return false;
	if (!m_face) return false;

	FT_Set_Pixel_Sizes(m_face, 0, m_pixels);

	
	// If no glyph list was specified, use the ASCII character set
	Char ascii[225];
	if (!m_glyphs)
	{
		for (int i = 32; i < 256; ++i)
			ascii[i - 32] = i;
		ascii[224] = 0;
		m_glyphs = ascii;
	}

	
	// Build a list of glyphs with metrics
	Glyphs glyphs;
	for (const Char* ch = m_glyphs; *ch; ++ch)
	{
		// See if the glyph exists in this font
		if (FT_Get_Char_Index(m_face, (UInt8)*ch) == 0) 
			continue;

		FT_Load_Char(m_face, (UInt8)(*ch), FT_LOAD_RENDER | (m_antialias ? 0 : FT_LOAD_MONOCHROME));

		Glyph g;
		g.code = (UInt8)*ch;
		g.x = 0;
		g.y = 0;
		g.width   = m_face->glyph->metrics.width / 64;
		g.height  = m_face->glyph->metrics.height / 64;
		g.advance = m_face->glyph->advance.x / 64;
		g.left    = m_face->glyph->metrics.horiBearingX / 64;
		g.bottom  = m_face->glyph->metrics.horiBearingY / 64;

		glyphs.push_back(g);
	}


	// I build the font using two textures. One holds the glyph images, and 
	// the other is a mask which indicates what parts of the first texture has
	// been used. 
	int size = m_maxSize * m_maxSize;

	UInt8* pixels = (UInt8*)malloc(size);
	memset(pixels, 0, size);

	UInt8* mask   = (UInt8*)malloc(size);
	memset(mask, 0, size);


	// Place the glyphs on the texture. I start with the largest and work 
	// down, trying to pack glyphs into as small a space as possible while 
	// maintaining a square texture

	glyphs.sort();

	int usedWidth = 0;
	int usedHeight = 0;

	Glyphs::iterator i;
	for (i = glyphs.begin(); i != glyphs.end(); ++i)
	{
		// Load the glyph image and find a place for it on the texture
		FT_Load_Char(m_face, (*i).code, FT_LOAD_RENDER | (m_antialias ? 0 : FT_LOAD_MONOCHROME));
		if (!_placeGlyph(mask, &usedWidth, &usedHeight, *i))
			break;

		// Copy the bitmap to the pixel texture
		UInt8* src = m_face->glyph->bitmap.buffer;
		UInt8* dst = pixels + ((*i).y * m_maxSize + (*i).x);
	
		int y;
		for (y = 0; y < (*i).height; ++y)
		{
			for (int x = 0; x < (*i).width; ++x)
				dst[x] = src[x];

			src += m_face->glyph->bitmap.pitch;
			dst += m_maxSize;
		}

		// Mask off the used area
		dst = mask + ((*i).y - 1) * m_maxSize + ((*i).x - 1);
		for (y = 0; y < (*i).height + 1; ++y)
		{
			for (int x = 0; x < (*i).width + 1; ++x)
				dst[x] = 0xff;
			dst += m_maxSize;
		}
	}


	free(mask);

	if (i != glyphs.end())
		return 0;


	// Trim the texture down to the smallest power of two possible
	int maxSize = ((usedWidth > usedHeight) ? usedWidth : usedHeight) + 1;
	
	size = 1;
	for (int p = 0; p < 30; ++p)
	{
		if (size > maxSize) break;
		size <<= 1;
	}

	if (size < m_maxSize)
	{
		UInt8* newPixels = (UInt8*)malloc(size * size);

		UInt32* src = (UInt32*)pixels;
		UInt32* dst = (UInt32*)newPixels;

		int words = size / 4;
		for (int y = 0; y < size; ++y)
		{
			for (int x = 0; x < words; ++x)
				*(dst++) = *(src++);
			
			src += (m_maxSize - size) / 4;
		}

		free(pixels);
		pixels = newPixels;
	}
	else
		size = m_maxSize;


	// Create the font object
	Texture* texture = (Texture*)Core->createA("Render.Texture");
	texture->initialize(size, size, 1, 1, pixels);

	TextureFont* txf = CREATEA_LOCAL(TextureFont);
	txf->initialize(m_face->size->metrics.height / 64, texture);
	for (i = glyphs.begin(); i != glyphs.end(); ++i)
	{
		txf->setGlyph((*i).code, (*i).x, (*i).y, (*i).width, (*i).height,
		              (*i).advance, (*i).left, (*i).bottom);
	}

	return txf;
}

//---------------------------------------------------------------------------

bool FontFactoryImpl::_placeGlyph(UInt8* mask, int* usedWidth, int* usedHeight, Glyph& g)
{
	// Leave a one pixel border to the left and top of the glyph image to
	// avoid "bleeding" from adjacent glyphs
	int width = g.width + 1;
	int height = g.height + 1;

	// Try to find a big enough empty space within the current used box
	for (int y = 0; y <= (*usedHeight - height); ++y)
	{
		for (int x = 0; x <= (*usedWidth - width); ++x)
		{
			int cx, cy;
			for (cy = y; cy < y + height; ++cy)
			{
				for (cx = x; cx < x + width; ++cx)
				{
					if (mask[cy * m_maxSize + cx])
					{
						cy = 99999;
						break;
					}
				}
			}

			// Put it here...leave a one pixel border to the top and left
			if (cy < 99999)
			{
				g.x = x + 1;
				g.y = y + 1;
				return true;
			}
		}
	}


	// If no space was found, expand the smallest edge of the used box, 
	// which will keep the texture close to square
	if (*usedHeight < *usedWidth)
	{
		if (*usedHeight == m_maxSize - 1)
			return false;

		*usedHeight += height;
		if (*usedHeight >= m_maxSize)
			*usedHeight = m_maxSize - 1;
	}
	else
	{
		if (*usedWidth == m_maxSize - 1)
			return false;

		*usedWidth += width;
		if (*usedWidth >= m_maxSize)
			*usedWidth = m_maxSize - 1;
	}

	return _placeGlyph(mask, usedWidth, usedHeight, g);
}

