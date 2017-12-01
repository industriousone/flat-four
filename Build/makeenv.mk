#----------------------------------------------------------------------------
# F4 Engine Makefile Logic
# Brutally hacked together by Jason Perkins, 379
# $Id: makeenv.mk,v 1.2 2002/06/14 01:14:31 jason379 Exp $
#
# Stay away from -O# and -Wall. I need to supply optimizations and warning
# more specifically for this to work. There's hackery involved. This will
# be fixed in a later release.
#----------------------------------------------------------------------------

X11LIB  = /usr/X11R6/lib
RANLIB  = ranlib

#----------------------------------------------------------------------------

ROOT     = ..
BASE     = $(ROOT)/Base
SERVICES = $(ROOT)/Services
EXTRAS   = $(ROOT)/Extras
LIB      = $(BASE)/Lib
BIN      = $(ROOT)/Bin

CFLAGS  += -fPIC -I$(BASE) -I$(SERVICES)

ifeq ($(BUILD), release)
	# -O# doesn't work, breaks ARG_...() macros in <Core/bindings.h>
	CFLAGS += -mpentium -fomit-frame-pointer -ffast-math
else
	CFLAGS += -DDEBUG -g
endif

ifeq ($(PLATFORM), Beos)
	CFLAGS   += -Wno-multichar -Wno-ctor-dtor-privacy
	SO_FLAGS  = -nostart -Xlinker -soname=$(notdir $(TARGET))
	APP_FLAGS = -Xlinker -soname=_APP_
	APP_LIBS  = -lbe -lstdc++
else
	SO_FLAGS  = -shared -L$(LIB) -L$(BIN) -L$(X11LIB)
	APP_FLAGS =  -L$(LIB) -L$(BIN)
	APP_LIBS  = -ldl
endif

CXXFLAGS = $(CFLAGS)


OBJECT_DIR = _$(MODULE)

VPATH   = $(PATHS)

TMP     = $(patsubst %.cpp,$(OBJECT_DIR)/%.o,$(FILES))
OBJECTS = $(patsubst %.c,$(OBJECT_DIR)/%.o,$(TMP))

LIBRARIES = $(patsubst %,-l%,$(LIBS))


#----------------------------------------------------------------------------
# Build rules
#
# These rules auto-create dependency lists (see "Advanced Auto-Dependency
# Generation" at http://www.paulandlesley.org/gmake/autodep.html) and place
# the *.P and *.o files in $(OBJ_DIR)
#----------------------------------------------------------------------------

$(OBJECT_DIR)/%.o : %.cpp
	-@if [ ! -d $(OBJECT_DIR) ]; then mkdir $(OBJECT_DIR); fi
	@echo $(notdir $<)
	@$(CXX) $(CXXFLAGS) -MD -o $@ -c $<
	@cp $(OBJECT_DIR)/$*.d $(OBJECT_DIR)/$*.P; \
	 sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' \
	     -e '/^$$/ d' -e 's/$$/ :/' < $(OBJECT_DIR)/$*.d >> $(OBJECT_DIR)/$*.P; \
	 rm -f $(OBJECT_DIR)/$*.d

$(OBJECT_DIR)/%.o : %.c
	-@if [ ! -d $(OBJECT_DIR) ]; then mkdir $(OBJECT_DIR); fi
	@echo $(notdir $<)
	@$(CC) $(CFLAGS) -MD -o $@ -c $<
	@cp $(OBJECT_DIR)/$*.d $(OBJECT_DIR)/$*.P; \
	 sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' \
	     -e '/^$$/ d' -e 's/$$/ :/' < $(OBJECT_DIR)/$*.d >> $(OBJECT_DIR)/$*.P; \
	 rm -f $(OBJECT_DIR)/$*.d

#$(OBJECT_DIR)/%.o : %.cpp
#	-@if [ ! -d $(OBJECT_DIR) ]; then mkdir $(OBJECT_DIR); fi
#	@echo $(notdir $<)
#	@$(CXX) $(CXXFLAGS) -o $@ -c $<

#$(OBJECT_DIR)/%.o : %.c
#	-@if [ ! -d $(OBJECT_DIR) ]; then mkdir $(OBJECT_DIR); fi
#	@echo $(notdir $<)
#	@$(CC) $(CFLAGS) -o $@ -c $<



.PHONY:   module executable clean

$(filter %.a, $(TARGET)) : %.a : $(OBJECTS)
	@echo Linking $@
	@$(AR) crs $@ $^
	@$(RANLIB) $@

$(filter %.so, $(TARGET)) : %.so : $(OBJECTS)
	@echo Linking $@
	@$(CXX) $(SO_FLAGS) -o $(TARGET) $^ $(SO_LIBS) $(LIBRARIES)

$(filter-out %.a %.so, $(TARGET)) : % : $(OBJECTS)
	@echo Linking $@
	@$(CXX) $(APP_FLAGS) -o $(TARGET) $^ $(APP_LIBS) $(LIBRARIES)

clean:
	@echo Cleaning $(MODULE)...
	-@rm -rf $(OBJECT_DIR)
	-@rm -f $(TARGET)


# --- Auto-generated Dependencies ---

-include $(OBJECTS:%.o=%.P)
