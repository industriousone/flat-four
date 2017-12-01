#----------------------------------------------------------------------------
# F4 Engine - Shell Makefile
# $Id: shell.mk,v 1.1.1.1 2002/04/09 12:38:19 jason379 Exp $
#----------------------------------------------------------------------------

MODULE = Shell
TARGET = $(BIN)/f4

PATHS  = $(BASE)/Shell/Src
FILES  = shell.cpp

LIBS   = Platform

include makeenv.mk
