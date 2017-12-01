/**
 * @file rw_png.cpp
 *
 * @author  Jason Perkins (jason@379.com)
 * @date    Copyright (C) 2001 by 379, Inc.
 * @version $Id: rw_bmp.cpp,v 1.2 2002/04/22 01:51:56 jason379 Exp $
 */
#include <Core/core.h>
#include <IO/io.h>
#include <IO/reader_writer.h>
#include <Render/texture.h>
#include <Memory/memory.h>
using namespace f4;


struct rwBMP : public ReaderWriter
{
	virtual Service* read(InputStream* stream);
	virtual bool write(Service*, OutputStream*);

private:
	DECL_SERVICE;
};

IMPL_ANON_SERVICE(rwBMP, ReaderWriter)


//---------------------------------------------------------------------------
// Local stuff
//---------------------------------------------------------------------------

typedef UInt8 Palette[256][4];

static const int BMP_BI_RGB  = 0;
static const int BMP_BI_RLE8 = 1;
static const int BMP_BI_RLE4 = 2;

static bool _readData1(InputStream* is, int w, int h, Palette pal, UInt8* pixels);
static bool _readData4(InputStream* is, int w, int h, Palette pal, UInt8* pixels);
static bool _readData4RLE(InputStream* is, int w, int h, Palette pal, UInt8* pixels);
static bool _readData8(InputStream* is, int w, int h, Palette pal, UInt8* pixels);
static bool _readData8RLE(InputStream* is, int w, int h, Palette pal, UInt8* pixels);
static bool _readData24(InputStream* is, int w, int h, Palette pal, UInt8* pixels);
static bool _escape(InputStream* is, int code, int* x, int* y, int w, int h);



//---------------------------------------------------------------------------
// Message text
//---------------------------------------------------------------------------

static const Char* FMT_ERROR       = "BMP: invalid BMP file\n";
static const Char* NO_RENDER       = "BMP: Render not installed\n";



//---------------------------------------------------------------------------

Service* rwBMP::read(InputStream* stream)
{
	stream->setByteOrder(InputStream::LSB);

	// Validate the file format
	if (stream->readInt16() != 0x4d42)
	{
		Core->print(FMT_ERROR);
		return 0;
	}

	// Load the remainder of the file into memory for faster access
	UInt32 filesize = stream->readInt32();
	Ref<InputStream> is = stream->createMemoryFile(filesize - 6);

	// Get some info about the data
	is->readInt32();
	UInt32 offset   = is->readInt32();
	UInt32 infoSize = is->readInt32();
	UInt32 width    = is->readInt32();
	UInt32 height   = is->readInt32();
	UInt16 planes   = is->readInt16();
	UInt16 bits     = is->readInt16();
	UInt32 compress = is->readInt32();

	if ( infoSize < 40 || planes != 1 || 
		(bits != 1 && bits != 4 && bits != 8 && bits != 24) ||
		(compress != BMP_BI_RGB && compress != BMP_BI_RLE8 && compress != BMP_BI_RLE4))
	{
		Core->print(FMT_ERROR);
		return 0;
	}

	is->seek(infoSize - 20, InputStream::CURRENT);

	// Read the palette
	Palette pal;
	int numColors = 1 << bits;
	if (bits < 24) is->read(pal, numColors * 4);

	// Get the pixels
	UInt8* pixels = (UInt8*)malloc(width * height * 3);
	
	is->seek(offset - 6, InputStream::START);
	is->setByteOrder(InputStream::MSB);

	bool ok = true;
	switch (bits)
	{
	case 1:
		ok = _readData1(is, width, height, pal, pixels);
		break;

	case 4:
		if (compress == BMP_BI_RGB) 
			ok = _readData4(is, width, height, pal, pixels);
		else
			ok = _readData4RLE(is, width, height, pal, pixels);
		break;
	
	case 8:
		if (compress == BMP_BI_RGB) 
			ok = _readData8(is, width, height, pal, pixels);
		else
			ok = _readData8RLE(is, width, height, pal, pixels);
		break;
	
	case 24:
		ok = _readData24(is, width, height, pal, pixels);
		break;
	}

	if (!ok)
	{
		free(pixels);
		Core->print(FMT_ERROR);
		return 0;
	}


	// Build and return the texture
	Texture* t = (Texture*)Core->createA("Render.Texture");
	if (!t)
	{
		free(pixels);
		Core->print(NO_RENDER);
		return 0;
	}

	t->initialize(width, height, 3, 1, pixels);
	return t;
}

//---------------------------------------------------------------------------

bool rwBMP::write(Service*, OutputStream*)
{
	return false;
}

//---------------------------------------------------------------------------

bool _readData1(InputStream* is, int w, int h, Palette pal, UInt8* pixels)
{
	UInt32 buffer;
	int    color;

	for (int y = 0; y < h; ++y)
	{
		for (int x = 0; x < w; ++x)
		{
			if (x % 32 == 0)
				buffer = is->readInt32();

			color = buffer >> 31;

			*(pixels++) = pal[color][2];
			*(pixels++) = pal[color][1];
			*(pixels++) = pal[color][0];

			buffer <<= 1;
		}
	}

	return true;
}

//---------------------------------------------------------------------------

bool _readData4(InputStream* is, int w, int h, Palette pal, UInt8* pixels)
{
	UInt32 buffer;
	int    color;

	for (int y = 0; y < h; ++y)
	{
		for (int x = 0; x < w; ++x)
		{
			if (x % 8 == 0)
				buffer = is->readInt32();

			color = buffer >> 28;

			*(pixels++) = pal[color][2];
			*(pixels++) = pal[color][1];
			*(pixels++) = pal[color][0];

			buffer <<= 4;
		}
	}

	return true;
}

//---------------------------------------------------------------------------

bool _readData4RLE(InputStream* is, int w, int h, Palette pal, UInt8* pixels)
{
	memset(pixels, 0, w * h * 3);
	int x = 0;
	int y = 0;

	UInt8* pix = pixels;

	while (y < h)
	{
		UInt8 code1 = is->readInt8();
		UInt8 code2 = is->readInt8();

		if (code1 == 0)
		{
			if (code2 < 3)
			{
				_escape(is, code2, &x, &y, w, h);
				pix = pixels + (y * w + x) * 3;
			}
			else
			{
				UInt16 buffer;
				for (int i = 0; i < code2; ++i)
				{
					if (i % 4 == 0)
						buffer = is->readInt16();

					int color = buffer >> 12;
					
					*(pix++) = pal[color][2];
					*(pix++) = pal[color][1];
					*(pix++) = pal[color][0];

					buffer <<= 4;
				}
				x += code2;
			}
		}
		else
		{
			for (int i = 0; i < code1; ++i)
			{
				*(pix++) = pal[code2][2];
				*(pix++) = pal[code2][1];
				*(pix++) = pal[code2][0];
			}
			x += code1;
		}
	}

	return true;
}

//---------------------------------------------------------------------------

bool _readData8(InputStream* is, int w, int h, Palette pal, UInt8* pixels)
{
	UInt32 buffer;
	int    color;

	for (int y = 0; y < h; ++y)
	{
		for (int x = 0; x < w; ++x)
		{
			if (x % 4 == 0)
				buffer = is->readInt32();

			color = buffer >> 24;

			*(pixels++) = pal[color][2];
			*(pixels++) = pal[color][1];
			*(pixels++) = pal[color][0];

			buffer <<= 8;
		}
	}

	return true;
}

//---------------------------------------------------------------------------

bool _readData8RLE(InputStream* is, int w, int h, Palette pal, UInt8* pixels)
{
	memset(pixels, 0, w * h * 3);
	int x = 0;
	int y = 0;

	UInt8* pix = pixels;

	while (y < h)
	{
		UInt8 code1 = is->readInt8();
		UInt8 code2 = is->readInt8();

		if (code1 == 0)
		{
			if (code2 < 3)
			{
				_escape(is, code2, &x, &y, w, h);
				pix = pixels + (y * w + x) * 3;
			}
			else
			{
				UInt16 buffer;
				for (int i = 0; i < code2; ++i)
				{
					if (i % 2 == 0)
						buffer = is->readInt16();

					int color = buffer >> 8;
					
					*(pix++) = pal[color][2];
					*(pix++) = pal[color][1];
					*(pix++) = pal[color][0];

					buffer <<= 8;
				}
				x += code2;
			}
		}
		else
		{
			for (int i = 0; i < code1; ++i)
			{
				*(pix++) = pal[code2][2];
				*(pix++) = pal[code2][1];
				*(pix++) = pal[code2][0];
			}
			x += code1;
		}
	}

	return true;
}

//---------------------------------------------------------------------------

bool _readData24(InputStream* is, int w, int h, Palette pal, UInt8* pixels)
{
	int excess = w * 3 % 4;
	int padding = (excess > 0) ? 4 - excess : 0;

	for (int y = 0; y < h; ++y)
	{
		for (int x = 0; x < w; ++x)
		{
			UInt8 b = is->readInt8();
			UInt8 g = is->readInt8();
			UInt8 r = is->readInt8();
			*(pixels++) = r;
			*(pixels++) = g;
			*(pixels++) = b;
		}

		is->seek(padding, InputStream::CURRENT);
	}

	return true;
}

//---------------------------------------------------------------------------

bool _escape(InputStream* is, int code, int* x, int* y, int w, int h)
{
	switch (code)
	{
	case 0:          // EOL
		*x = 0;
		++(*y);
		break;

	case 1:          // EOF
		*x = w;
		*y = h;
		break;

	case 2:          // move to
		*x += is->readInt8();
		*y += is->readInt8();
		break;

	default:
		return false;
	}

	return true;
}
