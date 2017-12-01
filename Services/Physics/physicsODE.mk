#----------------------------------------------------------------------------
# F4 Engine - ODE physics implementation
# $Id: physicsODE.mk,v 1.2 2002/04/22 20:23:04 jason379 Exp $
#----------------------------------------------------------------------------

MODULE = PhysicsODE
TARGET = $(BIN)/libPhysicsODE.so

PATHS  = $(SERVICES)/Physics/Src:$(SERVICES)/Physics/Src/ODE:$(SERVICES)/Common/ODE/src

FILES  = contact_physics.cpp \
         exports.cpp \
         mass_helper.cpp \
         physics.cpp \
         rigid_body.cpp \
         array.cpp \
         error.cpp \
         fastdot.c \
         fastldlt.c \
         fastlsolve.c \
         fastltsolve.c \
         geom.cpp \
         joint.cpp \
         lcp.cpp \
         mass.cpp \
         mat.cpp \
         matrix.cpp \
         memory.cpp \
         misc.cpp \
         obstack.cpp \
         ode.cpp \
         odemath.cpp \
         rotation.cpp \
         space.cpp \
         step.cpp \
         testing.cpp \
         timer.cpp



CFLAGS = -I$(SERVICES)/Common/ODE/include
LIBS   = Platform Math Memory Core

include makeenv.mk
