#----------------------------------------------------------------------------
# F4 Engine - I/O subsystem
# $Id: io.mk,v 1.1.1.1 2002/04/09 13:40:03 jason379 Exp $
#----------------------------------------------------------------------------

MODULE = IO
TARGET = $(BIN)/libIO.so

PATHS  = $(SERVICES)/IO/Src

FILES  = disk_io.cpp \
         exports.cpp \
         input_stream.cpp \
         io.cpp \
         memory_io.cpp \
         output_stream.cpp \
         reader_writer.cpp


CFLAGS =
LIBS   = Platform Memory Core

include makeenv.mk
