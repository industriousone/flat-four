#----------------------------------------------------------------------------
# F4 Engine - OpenGL renderer implementation
# $Id: renderGL.mk,v 1.2 2002/05/24 02:15:19 jason379 Exp $
#----------------------------------------------------------------------------

MODULE = RenderGL
TARGET = $(BIN)/libRenderGL.so

PATHS  = $(SERVICES)/Render/Src:$(SERVICES)/Render/Src/GL

FILES  = exports.cpp \
         iterator.cpp \
         render.cpp \
         shader.cpp \
         sprite.cpp \
         texture.cpp \
         vb.cpp \
         visual.cpp


CFLAGS = 
LIBS   = Platform Memory Math Core GL

include makeenv.mk
