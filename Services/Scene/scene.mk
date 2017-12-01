#----------------------------------------------------------------------------
# F4 Engine - Scene Manager implementation
# $Id: scene.mk,v 1.5 2002/06/02 18:54:48 jason379 Exp $
#----------------------------------------------------------------------------

MODULE = Scene
TARGET = $(BIN)/libScene.so

PATHS  = $(SERVICES)/Scene/Src

FILES  = body.cpp \
         collision_context.cpp \
         entity.cpp \
         exports.cpp \
         perspective_camera.cpp \
         physics.cpp \
         pose.cpp \
         render_context.cpp \
         simple_space.cpp \
         surface.cpp \
         update_context.cpp

CFLAGS = 
LIBS   = Platform Memory Math Core

include makeenv.mk
