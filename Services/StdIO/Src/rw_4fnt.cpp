/**
 * @file rw_4fnt.cpp
 *
 * Read/write the F4's proprietary "4FNT" texture font format.
 *
 * @author  Jason Perkins (jason@379.com)
 * @date    Copyright (C) 2001 by 379, Inc.
 * @version $Id: rw_4fnt.cpp,v 1.2 2002/04/22 01:51:56 jason379 Exp $
 */
#include <Core/core.h>
#include <IO/io.h>
#include <IO/reader_writer.h>
#include <Text/texture_font.h>
#include <Memory/memory.h>
using namespace f4;


struct rw4FNT : public ReaderWriter
{
	virtual Service* read(InputStream* stream);
	virtual bool write(Service* service, OutputStream* stream);

private:
	DECL_SERVICE;
};

IMPL_ANON_SERVICE(rw4FNT, ReaderWriter);


//---------------------------------------------------------------------------
// Message text
//---------------------------------------------------------------------------

static const Char* BAD_OBJECT_TYPE = "Invalid object type.";
static const Char* BAD_FORMAT      = "Invalid .4FNT file";
static const Char* NO_RENDER       = "Render service not available";


//---------------------------------------------------------------------------

Service* rw4FNT::read(InputStream* stream)
{
	// Read the file signature
	stream->setByteOrder(InputStream::LSB);

	char sig[4];
	stream->read(sig, 4);
	if (strncmp(sig, "4FNT", 4) != 0)
	{
		Core->print(BAD_FORMAT);
		return 0;
	}

	// Need a texture font to fill
	TextureFont* txf = (TextureFont*)Core->createA("Text.TextureFont");
	if (!txf) 
	{
		Core->print(NO_RENDER);
		return 0;
	}

	
	// Get the line height
	int height = stream->readInt16();

	// Read the list of glyphs
	int numGlyphs = stream->readInt16();
	for (int i = 0; i < numGlyphs; ++i)
	{
		int code = stream->readInt16();
		int x    = stream->readInt16();
		int y    = stream->readInt16();
		int w    = stream->readInt16();
		int h    = stream->readInt16();
		int a    = stream->readInt16();
		int l    = stream->readInt8();
		int t    = stream->readInt8();
		txf->setGlyph(code, x, y, w, h, a, l, t);
	}
	
	// Read the font texture
	IOService* io = (IOService*)Core->createA("IO.IOService");
	Texture* tex = (Texture*)io->fromStream(stream, "png");
	if (!tex)
	{
		txf->release();
		return 0;
	}

	txf->initialize(height, tex);
	return txf;
}

//---------------------------------------------------------------------------

bool rw4FNT::write(Service* service, OutputStream* stream)
{
	// I only know how to write TextureFont objects
	if (!service->isA("TextureFont"))
	{
		Core->die(BAD_OBJECT_TYPE);
		return false;
	}

	TextureFont* txf = static_cast<TextureFont*>(service);

	
	// Write a file header
	stream->setByteOrder(OutputStream::LSB);
	stream->write((void*)"4FNT", 4);

	// Write the line height
	stream->writeInt16(txf->getHeight());

	// Write the glyph information
	int numGlyphs = txf->countGlyphs();
	stream->writeInt16(numGlyphs);

	int* codes = (int*)malloc(numGlyphs * sizeof(int));
	txf->getGlyphCodes(codes);

	int info[7];
	for (int i = 0; i < numGlyphs; ++i)
	{
		txf->getGlyphInfo(codes[i], info);
		stream->writeInt16(codes[i]);
		stream->writeInt16(info[0]);
		stream->writeInt16(info[1]);
		stream->writeInt16(info[2]);
		stream->writeInt16(info[3]);
		stream->writeInt16(info[4]);
		stream->writeInt8(info[5]);
		stream->writeInt8(info[6]);
	}


	free(codes);


	// Write the texture out in .PNG format
	IOService* io = (IOService*)Core->createA("IO.IOService");
	return io->toStream(txf->getTexture(), stream, "png");
}
