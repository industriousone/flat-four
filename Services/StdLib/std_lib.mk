#----------------------------------------------------------------------------
# F4 Engine - standard collection of useful things
# $Id: std_lib.mk,v 1.2 2002/04/22 20:23:04 jason379 Exp $
#----------------------------------------------------------------------------

MODULE = StdLib
TARGET = $(BIN)/libStdLib.so

PATHS  = $(SERVICES)/StdLib/Src:$(SERVICES)/StdLib/Src/Shapes

FILES  = action.cpp \
         action_mapper.cpp \
         cube.cpp \
         exports.cpp

CFLAGS =
LIBS   = Platform Math Memory Core

include makeenv.mk
