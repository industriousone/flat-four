/**
 * @file f4system.cpp
 *
 * A replacement for FreeType's ftsystem.c file. Uses the F4 API for memory
 * and file I/O instead of the ANSI alternatives.
 *
 * @author  Jason Perkins (jason@379.com)
 * @date    Copyright (C) 2001 by 379, Inc.
 * @version $Id: f4system.cpp,v 1.1 2002/04/26 20:26:07 jason379 Exp $
 */
#include <ft2build.h>
#include <freetype/config/ftconfig.h>
#include <freetype/internal/ftdebug.h>
#include <freetype/ftsystem.h>
#include <freetype/fterrors.h>
#include <freetype/fttypes.h>

#include <Core/core.h>
#include <IO/io.h>
#include <Memory/memory.h>
using namespace f4;


//---------------------------------------------------------------------------

  extern "C"
  FT_CALLBACK_DEF( void* )
  ft_alloc( FT_Memory  memory,
            long       size )
  {
    FT_UNUSED( memory );
    return malloc( size );
  }

//---------------------------------------------------------------------------

  extern "C"
  FT_CALLBACK_DEF( void* )
  ft_realloc( FT_Memory  memory,
              long       cur_size,
              long       new_size,
              void*      block )
  {
    FT_UNUSED( memory );
    FT_UNUSED( cur_size );
    return realloc( block, new_size );
  }

//---------------------------------------------------------------------------

  extern "C"
  FT_CALLBACK_DEF( void )
  ft_free( FT_Memory  memory,
           void*      block )
  {
    FT_UNUSED( memory );
    free( block );
  }

//---------------------------------------------------------------------------

// Remove the memory system macros so code below doesn't choke on them
#undef realloc
#undef free

//---------------------------------------------------------------------------

  extern "C"
  FT_EXPORT_DEF( FT_Memory )
  FT_New_Memory( void )
  {
    FT_Memory  memory;
    memory = (FT_Memory)malloc( sizeof ( *memory ) );
    if ( memory )
    {
      memory->user    = 0;
      memory->alloc   = ft_alloc;
      memory->realloc = ft_realloc;
      memory->free    = ft_free;
    }

    return memory;
  }

//---------------------------------------------------------------------------

  extern "C"
  FT_EXPORT_DEF( void )
  FT_Done_Memory( FT_Memory  memory )
  {
    memory->free( memory, memory );
  }

//---------------------------------------------------------------------------

  extern "C"
  FT_CALLBACK_DEF( unsigned long )
  ft_io_stream( FT_Stream       stream,
                unsigned long   offset,
                unsigned char*  buffer,
                unsigned long   count )
  {
	InputStream* is = (InputStream*)stream->descriptor.pointer;
	is->seek(offset, InputStream::START);
	return is->read(buffer, count);
  }

//---------------------------------------------------------------------------

  extern "C"
  FT_CALLBACK_DEF( void )
  ft_close_stream( FT_Stream  stream )
  {
	InputStream* is = (InputStream*)stream->descriptor.pointer;
	is->release();

    stream->descriptor.pointer = NULL;
    stream->size               = 0;
    stream->base               = 0;
  }

//---------------------------------------------------------------------------

  extern "C"
  FT_EXPORT_DEF( FT_Error )
  FT_New_Stream( const char*  filepathname,
                 FT_Stream    astream )
  {
    if ( !astream )
      return FT_Err_Invalid_Stream_Handle;

	Ref<IOService> io = (IOService*)Core->createA("IO.IOService");
	if (!io)
	{
		FT_ERROR(( "FT_NewStream:" ));
		FT_ERROR(( " could not load IOService\n"));
		return FT_Err_Cannot_Open_Resource;
	}

	InputStream* is = io->createInputStream(filepathname);
	if (!is)
	{
		FT_ERROR(( "FT_New_Stream:" ));
		FT_ERROR(( " could not open `%s'\n", filepathname ));
		return FT_Err_Cannot_Open_Resource;
    }

	is->seek(0, InputStream::END);
	astream->size = is->tell();
	is->seek(0, InputStream::START);

    astream->descriptor.pointer = is;
    astream->pathname.pointer   = (char*)filepathname;
    astream->pos                = 0;

    astream->read  = ft_io_stream;
    astream->close = ft_close_stream;

    FT_TRACE1(( "FT_New_Stream:" ));
    FT_TRACE1(( " opened `%s' (%d bytes) successfully\n",
                filepathname, astream->size ));

    return FT_Err_Ok;
  }
