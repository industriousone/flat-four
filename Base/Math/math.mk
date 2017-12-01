#----------------------------------------------------------------------------
# F4 Engine - Math Library Makefile
# $Id: math.mk,v 1.1.1.1 2002/04/09 12:38:14 jason379 Exp $
#----------------------------------------------------------------------------

MODULE = Math
TARGET = $(BIN)/libMath.so

PATHS  = $(BASE)/Math/Src
FILES  = matrix.cpp quaternion.cpp vector.cpp

LIBS   = 

include makeenv.mk
