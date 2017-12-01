#----------------------------------------------------------------------------
# F4 Engine - ODE collider implementation
# $Id: colliderODE.mk,v 1.4 2002/06/02 18:55:02 jason379 Exp $
#----------------------------------------------------------------------------

MODULE = ColliderODE
TARGET = $(BIN)/libColliderODE.so

PATHS  = $(SERVICES)/Collider/Src:$(SERVICES)/Collider/Src/ODE:$(SERVICES)/Common/ODE/src:$(SERVICES)/Collider/Src/OPCODE

FILES  = box.cpp \
         capsule.cpp \
         collider.cpp \
         contact_list.cpp \
         exports.cpp \
         mesh.cpp \
         plane.cpp \
         sphere.cpp \
         \
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
         timer.cpp \
         \
         IceAABB.cpp \
         IceContainer.cpp \
         IceMatrix3x3.cpp \
         IceMatrix4x4.cpp \
         IcePoint.cpp \
         IceTriangle.cpp \
         OPC_AABBTree.cpp \
         OPC_Collider.cpp \
         OPC_Common.cpp \
         OPC_Model.cpp \
         Opcode.cpp \
         OPC_OptimizedTree.cpp \
         OPC_RayCollider.cpp \
         OPC_SphereCollider.cpp \
         OPC_TreeBuilders.cpp \
         OPC_TreeCollider.cpp
         



CFLAGS = -I$(SERVICES)/Collider/Src -I$(SERVICES)/Common/ODE/include
LIBS   = Platform Memory Math Core

include makeenv.mk
