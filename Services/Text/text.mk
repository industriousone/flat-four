#----------------------------------------------------------------------------
# F4 Engine - text rendering services
# $Id: text.mk,v 1.2 2002/04/26 20:26:06 jason379 Exp $
#----------------------------------------------------------------------------

MODULE = Text
TARGET = $(BIN)/libText.so

PATHS  = $(SERVICES)/Text/Src:\
         $(SERVICES)/Text/Src/FreeType/src/autohint:\
         $(SERVICES)/Text/Src/FreeType/src/base:\
         $(SERVICES)/Text/Src/FreeType/src/sfnt:\
         $(SERVICES)/Text/Src/FreeType/src/smooth:\
         $(SERVICES)/Text/Src/FreeType/src/truetype:\
         $(SERVICES)/Text/Src/FreeType/src/type1

FILES  = font_factory.cpp \
         exports.cpp \
         texture_font.cpp \
         visual.cpp \
         f4system.cpp \
         autohint.c \
         ftbase.c \
         ftglyph.c \
         ftinit.c \
         sfnt.c \
         smooth.c \
         truetype.c \
         type1.c \

CFLAGS = -I$(SERVICES)/Text/Src/FreeType/include -I$(SERVICES)/Text/Src
LIBS   = Platform Memory Core

include makeenv.mk
