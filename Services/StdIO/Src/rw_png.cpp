/**
 * @file rw_png.cpp
 *
 * @author  Jason Perkins (jason@379.com)
 * @date    Copyright (C) 2001 by 379, Inc.
 * @version $Id: rw_png.cpp,v 1.2 2002/04/22 01:51:56 jason379 Exp $
 *
 * @todo Get libpng and zlib to use the F4 memory manager.
 *
 * @todo Support > 8 bits color.
 *
 */
#include "png.h"

#include <Core/core.h>
#include <IO/io.h>
#include <IO/reader_writer.h>
#include <Render/texture.h>
#include <Memory/memory.h>
using namespace f4;


struct rwPNG : public ReaderWriter
{
	virtual Service* read(InputStream* stream);
	virtual bool write(Service* service, OutputStream* stream);

private:
	DECL_SERVICE;
};

IMPL_ANON_SERVICE(rwPNG, ReaderWriter)



//---------------------------------------------------------------------------
// Message text
//---------------------------------------------------------------------------

static const Char* PNG_ERROR       = "PNG: invalid PNG file\n";
static const Char* READ_ERROR      = "PNG: read error";
static const Char* WRITE_ERROR     = "PNG: write error";
static const Char* NO_RENDER       = "PNG: Render not installed\n";
static const Char* BAD_OBJECT_TYPE = "Invalid object type.";


//---------------------------------------------------------------------------
// F4 compatible I/O functions for libPNG
//---------------------------------------------------------------------------

static void user_read_data(png_structp png_ptr,png_bytep data, png_size_t length)
{
	InputStream* stream = (InputStream*)png_get_io_ptr(png_ptr);
	if (stream->read(data, length) != length)
		png_error(png_ptr, READ_ERROR);
}

static void user_write_data(png_structp png_ptr, png_bytep data, png_size_t length)
{
	OutputStream* stream = (OutputStream*)png_get_io_ptr(png_ptr);
	if (stream->write(data, length) != length)
		png_error(png_ptr, WRITE_ERROR);
}

static void user_flush_data(png_structp png_ptr)
{
	OutputStream* stream = (OutputStream*)png_get_io_ptr(png_ptr);
	stream->flush();
}




//---------------------------------------------------------------------------

Service* rwPNG::read(InputStream* stream)
{
	// Set up a read buffer and check the library version
	png_structp png_ptr;
	png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (!png_ptr) 
	{
		Core->print(PNG_ERROR);
		return 0;
	}


	// Allocate/initialize a memory block for the image information
	png_infop info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr) 
	{
		png_destroy_read_struct(&png_ptr, NULL, NULL);
		Core->print(PNG_ERROR);
		return 0;
	}
	
	
	// Set up the error handling
	if (setjmp(png_jmpbuf(png_ptr))) 
	{
		png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
		Core->print(PNG_ERROR);
		return 0;
	}


	// Set up the I/O functions. 
	png_set_read_fn(png_ptr, (void*)stream, user_read_data);


	// Retrieve the image header information
	png_uint_32 width, height;
	int bit_depth, color_type, interlace_type;
	png_read_info(png_ptr, info_ptr);
	png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth,
				 &color_type, &interlace_type, NULL, NULL);


	// Expand paletted colors into true RGB triplets
	if (color_type == PNG_COLOR_TYPE_PALETTE)
		png_set_expand(png_ptr);

	// Expand grayscale images to a full 8 bits
	if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
		png_set_expand(png_ptr);

	// Expand paletted or RGB images with transparency to full alpha
	// channels so the data will be available as RGBA quartets
	if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS))
		png_set_expand(png_ptr);


	// Tell libpng to do the gamma adjustment for me
	png_read_update_info(png_ptr, info_ptr);
	

 	// Allocate memory for the pixel data and read the image
	int    bytes  = png_get_rowbytes(png_ptr, info_ptr);
	UInt8* pixels = (UInt8*)malloc(bytes * height);

	png_bytep* row_data = (png_bytep*)malloc(sizeof(png_bytep) * sizeof(png_byte) * height);
	for (unsigned i = 0; i < height; ++i)
		row_data[i] = &(pixels[bytes * (height - i - 1)]);

	png_read_image(png_ptr, row_data);
	free(row_data);

	
	// Convert the raw data to a Texture
	int numComponents;
	switch (color_type)
	{
	case PNG_COLOR_TYPE_GRAY:       numComponents = 1; break;
	case PNG_COLOR_TYPE_GRAY_ALPHA: numComponents = 2; break;
	case PNG_COLOR_TYPE_RGB_ALPHA:  numComponents = 4; break;
	
	default: numComponents = 3; break;
	}

	Texture* t = (Texture*)Core->createA("Render.Texture");
	if (!t)
	{
		free(pixels);
		Core->print(NO_RENDER);
		return 0;
	}

	t->initialize(width, height, numComponents, 1, pixels);


	// Finish things up
	png_read_end(png_ptr, info_ptr);
	png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
	return t;
}

//---------------------------------------------------------------------------

bool rwPNG::write(Service* service, OutputStream* stream)
{
	// I only know how to write Texture objects
	if (!service->isA("Texture"))
	{
		Core->die(BAD_OBJECT_TYPE);
		return false;
	}

	Texture* tex = static_cast<Texture*>(service);


	// Set up a write buffer and check the library version
	png_structp png_ptr;
	png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (!png_ptr) 
	{
		Core->print(PNG_ERROR);
		return false;
	}


	// Allocate/initialize a memory block for the image information
	png_infop info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr) 
	{
		png_destroy_write_struct(&png_ptr, NULL);
		Core->print(PNG_ERROR);
		return false;
	}
	
	
	// Set up the error handling
	if (setjmp(png_jmpbuf(png_ptr))) 
	{
		png_destroy_write_struct(&png_ptr, &info_ptr);
		Core->print(PNG_ERROR);
		return false;
	}


	// Set up the I/O functions
	png_set_write_fn(png_ptr, (void*)stream, user_write_data, user_flush_data);


	// Set up the image format specification in a way that png will understand
	int pixel_size = tex->getComponents();
	int bit_depth  = tex->getComponentSize() * 8;

	int color_type = 0;
	switch (pixel_size)
	{
	case 1:
		color_type = PNG_COLOR_TYPE_GRAY;
		break;
	case 2:
		color_type = PNG_COLOR_TYPE_GRAY_ALPHA;
		break;
	case 3:
		color_type = PNG_COLOR_TYPE_RGB;
		break;
	case 4:
		color_type = PNG_COLOR_TYPE_RGB_ALPHA;
		break;
	}

			
	// Set the image format information
	int width = tex->getWidth();
	int height = tex->getHeight();
	png_set_IHDR(png_ptr, info_ptr, width, height, bit_depth, color_type, 
		PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);


	// Invert the alpha channel, so that 0 = fully transparent and 255 = fully opaque
//	png_set_invert_alpha(png_ptr);


	// If on a little-endian system, swap the byte ordering
#if defined(F4_SYSTEM_LITTLE_ENDIAN)
	if (bit_depth > 8)
		png_set_swap(png_ptr);
#endif


	// Build a list of rows and write them to disk
	int bytes = png_get_rowbytes(png_ptr, info_ptr);

	UInt8* pixels = (UInt8*)tex->getPixels();
	png_bytep* row_data = (png_bytep*)malloc(sizeof(png_bytep) * sizeof(png_byte) * height);
	for (unsigned i = 0; i < (unsigned)height; ++i)
		row_data[i] = &(pixels[bytes * (height - i - 1)]);

	png_write_info(png_ptr, info_ptr);
	png_write_image(png_ptr, row_data);
	png_write_end(png_ptr, info_ptr);


	// Finish things up
	free(row_data);
	png_destroy_write_struct(&png_ptr, &info_ptr);
	return true;
}

