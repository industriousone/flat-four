#----------------------------------------------------------------------------
# Flat Four Engine GNU Makefile
# Hacked together mercilessly by Jason Perkins, 379
#
# Be warned: these scripts do no dependency checking whatsoever, it will
# only build properly after a 'make clean'. I'll get around to that.
#
# $Id: Makefile,v 1.3 2002/05/15 10:37:17 jason379 Exp $
#----------------------------------------------------------------------------

# List of the optional services and extras to be built

USER_TARGETS = Services/Collider/colliderODE \
               Services/IO/io \
               Services/Physics/physicsODE \
               Services/Render/renderGL \
               Services/Scene/scene \
               Services/StdIO/std_io \
               Services/StdLib/std_lib \
               Services/System/systemSDL \
               Services/Text/text
               


#----------------------------------------------------------------------------

TARGETS = Base/Platform/platform \
          Base/Memory/memory \
          Base/Math/math \
          Base/Core/core \
          Base/Script/script \
          Base/Shell/shell \
          $(USER_TARGETS)

ifeq ($(OSTYPE),beos)
   PLATFORM = Beos
else
   PLATFORM = Linux
endif

ifndef BUILD
   BUILD = debug
endif

export BUILD PLATFORM

#----------------------------------------------------------------------------

.PHONY: all clean $(TARGETS)

all: $(TARGETS)

$(TARGETS):
	@$(MAKE) --no-print-directory -f ../$@.mk

clean:
	-@for t in $(TARGETS); \
		do ($(MAKE) --no-print-directory -f ../$$t.mk clean); \
	done

