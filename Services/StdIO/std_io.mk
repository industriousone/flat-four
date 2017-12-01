#----------------------------------------------------------------------------
# F4 Engine - standard collection of ReaderWriters
# $Id: std_io.mk,v 1.1.1.1 2002/04/09 13:40:07 jason379 Exp $
#----------------------------------------------------------------------------

MODULE = StdIO
TARGET = $(BIN)/libStdIO.so

PATHS  = $(SERVICES)/StdIO/Src:\
         $(SERVICES)/StdIO/Src/libpng:\
         $(SERVICES)/StdIO/Src/zlib

FILES  = exports.cpp \
         flt_zlib.cpp \
         register.cpp \
         rw_4fnt.cpp \
         rw_bmp.cpp \
         rw_obj.cpp \
         rw_png.cpp \
         rw_tga.cpp \
         png.c pngerror.c pngget.c pngmem.c pngpread.c pngread.c pngrio.c pngrtran.c \
         pngrutil.c pngset.c pngtrans.c pngwio.c pngwrite.c pngwtran.c pngwutil.c \
         adler32.c compress.c crc32.c deflate.c gzio.c infblock.c infcodes.c inffast.c \
         inflate.c inftrees.c infutil.c trees.c uncompr.c zutil.c

CFLAGS = -I$(SERVICES)/StdIO/Src/libpng -I$(SERVICES)/StdIO/Src/zlib
LIBS   = Platform Memory Core

include makeenv.mk
