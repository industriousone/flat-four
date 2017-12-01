#----------------------------------------------------------------------------
# F4 Engine - Platform Abstraction Library Makefile
# $Id: platform.mk,v 1.2 2002/04/22 20:23:03 jason379 Exp $
#----------------------------------------------------------------------------

MODULE = Platform
TARGET = $(LIB)/libPlatform.a

PATHS  = $(BASE)/Platform/Src/Common:$(BASE)/Platform/Src/Linux
FILES  = directory.cpp \
         file.cpp \
         library.cpp \
         print_alert.cpp \
         print_debug.cpp \
         string.cpp

LIBS   =

include makeenv.mk
