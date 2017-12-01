/**
 * @file rw_tga.cpp
 *
 * @author  Jason Perkins (jason@379.com)
 * @date    Copyright (C) 2001 by 379, Inc.
 * @version $Id: rw_tga.cpp,v 1.2 2002/04/22 01:51:56 jason379 Exp $
 */
#include <Core/core.h>
#include <IO/io.h>
#include <IO/reader_writer.h>
#include <Render/texture.h>
#include <Memory/memory.h>
using namespace f4;


struct rwTGA : public ReaderWriter
{
	virtual Service* read(InputStream* stream);
	virtual bool write(Service*, OutputStream*) { return false; }

private:
	DECL_SERVICE;
};

IMPL_ANON_SERVICE(rwTGA, ReaderWriter);



//---------------------------------------------------------------------------
// Message text
//---------------------------------------------------------------------------

static const Char* NO_RENDER    = "TGA: Render not installed\n";
static const Char* BAD_COMP_FMT = "TGA: unsupported compression format\n";
static const Char* BAD_PIX_FMT  = "TGA: unsupported pixel format\n";



//---------------------------------------------------------------------------

enum
{
	TGAHDR_IDLENGTH			= 0x00,		// BYTE
	TGAHDR_COLORMAPTYPE		= 0x01,		// BYTE
	TGAHDR_IMAGETYPE		= 0x02,		// BYTE
	TGAHDR_CMAPSTART		= 0x03,		// WORD
	TGAHDR_CMAPLENGTH		= 0x05,		// WORD
	TGAHDR_CMAPDEPTH		= 0x07,		// BYTE
	TGAHDR_XOFFSET			= 0x08,		// WORD
	TGAHDR_YOFFSET			= 0x0A,		// WORD
	TGAHDR_WIDTH			= 0x0C,		// WORD
	TGAHDR_HEIGHT			= 0x0E,		// WORD
	TGAHDR_PIXELDEPTH		= 0x10,		// BYTE
	TGAHDR_IMAGEDESC		= 0x11,		// BYTE
	TGAHDR_SIZE				= 0x12
};

enum
{
	TGATYPE_NOIMAGE			= 0,
	TGATYPE_COLORMAP		= 1,
	TGATYPE_TRUECOLOR		= 2,
	TGATYPE_MONOCHROME		= 3,
	TGATYPE_COLORMAP_RLE	= 9,
	TGATYPE_TRUECOLOR_RLE	= 10,
	TGATYPE_MONOCHROME_RLE	= 11
};


Service* rwTGA::read(InputStream* stream)
{
	UInt8 header[TGAHDR_SIZE];
	if (stream->read(header, TGAHDR_SIZE) != TGAHDR_SIZE) return 0;
	

	// Check the image type. I'm currently supporting grayscale, RGB, and
	// and RGB with RLE compression
	switch (header[TGAHDR_IMAGETYPE])
	{
		case TGATYPE_TRUECOLOR:
		case TGATYPE_TRUECOLOR_RLE:
		case TGATYPE_MONOCHROME:
			break;
	
		default:
			Core->print(BAD_COMP_FMT);
			return 0;
	}


	// Check the pixel depth and set the pixel format.
	unsigned numComponents;
	switch (header[TGAHDR_PIXELDEPTH])
	{
	case 8:
	case 24:
	case 32:
		numComponents = header[TGAHDR_PIXELDEPTH] / 8;
		break;

	default:
		Core->print(BAD_PIX_FMT);
		return 0;
	}


	// Get the width and the height of the picture.
	int width = header[TGAHDR_WIDTH + 1] * 256 + header[TGAHDR_WIDTH];
	int height = header[TGAHDR_HEIGHT + 1] * 256 + header[TGAHDR_HEIGHT];
	
	// Allocate the pixel buffer.
	UInt32 size   = width * height * numComponents;
	UInt8* pixels = (UInt8*)malloc(size);

	
	// Load the image data.
	bool ok = false;
	switch (header[TGAHDR_IMAGETYPE])
	{
	// Uncompressed image data.
	case TGATYPE_MONOCHROME:
	case TGATYPE_TRUECOLOR:
		ok = (stream->read(pixels, size) == size);
		break;

	// RLE compressed image data.
	case TGATYPE_TRUECOLOR_RLE:
		{
			UInt8* p = pixels;
			UInt8 data;

			while (size > 0)
			{
				if (stream->read(&data, 1) != 1)
					break;

				if (data & 0x80)
				{
					// run-length encoded packet
					UInt8 pixel[4];
					if (stream->read(&pixel, numComponents) != numComponents)
						break;

					size -= ((data & 0x7F) + 1) * numComponents;
					for (int i = (data & 0x7F); i >= 0; i --)
					{
						// excessively clever inline copy
						switch (numComponents)
						{
							case 4: p[3] = pixel[3];
							case 3:	p[2] = pixel[2];
							case 2: p[1] = pixel[1];
							case 1: p[0] = pixel[0];
						}

						p += numComponents;
					}
				}
				else
				{
					// raw encoded packet.
					unsigned count = ((data & 0x7F) + 1) * numComponents;
					size -= count;
					if (stream->read((char*)p, count) != count)
						break;

					p += count;
				}
			}

			ok = (size == 0);
		}
		break;
	}

	if (!ok)
	{
		free(pixels);
		Core->print("TGA: unexpected EOF encountered\n");
		return 0;
	}

	
	// Targa stores pixel data as BGRA, which is unsupported in the base GL
	// spec. Convert it to RGBA to make it more generally useful
	if (numComponents > 2)
	{
		UInt8 tmp;
		for (UInt8* ptr = pixels; ptr < (pixels + size); ptr += numComponents)
		{
			tmp = *ptr;
			*ptr = *(ptr + 2);
			*(ptr + 2) = tmp;
		}
	}


	Texture* t = (Texture*)Core->createA("Render.Texture");
	if (!t)
	{
		free(pixels);
		Core->print(NO_RENDER);
		return 0;
	}

	t->initialize(width, height, numComponents, 1, pixels);
	return t;
}

