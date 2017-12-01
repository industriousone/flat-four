#----------------------------------------------------------------------------
# F4 Engine - Core Services Makefile
# $Id: core.mk,v 1.2 2002/04/22 20:23:03 jason379 Exp $
#----------------------------------------------------------------------------

MODULE = Core
TARGET = $(BIN)/libCore.so

PATHS  = $(BASE)/Core/Src
FILES  = core.cpp \
         event.cpp \
         event_listener.cpp \
         exports.cpp \
         service.cpp

CFLAGS = -DCORE_API=F4_EXPORT
LIBS   = Platform Memory

include makeenv.mk
