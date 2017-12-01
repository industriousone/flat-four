#----------------------------------------------------------------------------
# F4 Engine - Script Services Makefile
# $Id: script.mk,v 1.2 2002/04/22 20:23:03 jason379 Exp $
#----------------------------------------------------------------------------

MODULE = Script
TARGET = $(BIN)/libScript.so

PATHS  = $(BASE)/Script/Src/Lua:$(BASE)/Script/Src/Lua/Lua/src:$(BASE)/Script/Src/Lua/Lua/src/lib

FILES  = context.cpp \
         extensions.cpp \
         matrix.cpp \
         quaternion.cpp \
         refs.cpp \
         script.cpp \
         thunks.cpp \
         vector.cpp \
         lapi.c \
         lcode.c \
         ldebug.c \
         ldo.c \
         lfunc.c \
         lgc.c \
         llex.c \
         lmem.c \
         lobject.c \
         lopcodes.c \
         lparser.c \
         lstate.c \
         lstring.c \
         ltable.c \
         ltests.c \
         ltm.c \
         lundump.c \
         lvm.c \
         lzio.c \
         lauxlib.c \
         lbaselib.c \
         lmathlib.c \
         lstrlib.c


CFLAGS = -I$(BASE)/Script/Src/Lua/Lua/include
LIBS   = Platform Memory Math Core

include makeenv.mk
