#----------------------------------------------------------------------------
# F4 Engine - SDL System implementation
# $Id: systemSDL.mk,v 1.3 2002/05/24 02:15:19 jason379 Exp $
#----------------------------------------------------------------------------

MODULE = SystemSDL
TARGET = $(BIN)/libSystemSDL.so

PATHS  = $(SERVICES)/System/Src:$(SERVICES)/System/Src/SDL

FILES  = axis_event.cpp \
         button_event.cpp \
         click_event.cpp \
         display_event.cpp \
         event_loop.cpp \
         exports.cpp \
         key_event.cpp \
         key_table.cpp \
         motion_event.cpp \
         sdl_bindings.cpp \
         system.cpp \
         time_event.cpp

CFLAGS  =
LIBS    = Platform Memory Core

include makeenv.mk
