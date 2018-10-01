# C++ compiler 
CXX = g++

# Location and arguments of wx-config script 
WX_CONFIG ?= wx-config

# Standard flags for C++ 
CXXFLAGS ?= -Wall
VART_CXXFLAGS = $(CXXFLAGS) -I.

# Standard preprocessor flags (common for CC and CXX) 
CPPFLAGS ?= 

# Standard linker flags 
LDFLAGS = 
WX_LDFLAGS ?= $(shell $(WX_CONFIG) --libs core,base) $(LDFLAGS)

# C++ flags to use with wxWidgets code 
WX_CXXFLAGS ?= $(CXXFLAGS) $(shell $(WX_CONFIG) --cxxflags)


# -------------------------------------------------------------------------
# Do not modify the rest of this file!
# -------------------------------------------------------------------------

### Variables: ###

OBJECTS = integerdialog.o drawmodedialog.o bigpixelcanvas.o zbuffer.o main.o

### Conditionally set variables: ###



### Targets: ###

all: main.app

clean: 
	rm -f *.o *~ main.app

main.app: $(OBJECTS)
	$(CXX) $(OBJECTS) $(WX_LDFLAGS) -o $@

zbuffer.o: zbuffer.cpp zbuffer.h
	$(CXX) -c $< $(CPPFLAGS) $(CXXFLAGS) $(CPPDEPS) -o $@

bigpixelcanvas.o: bigpixelcanvas.cpp bigpixelcanvas.h triangulo3d.h aresta.h intervalohorizontal.h
	$(CXX) -c $< $(CPPFLAGS) $(WX_CXXFLAGS) $(CPPDEPS) -o $@

integerdialog.o: integerdialog.cpp integerdialog.h
	$(CXX) -c $< $(CPPFLAGS) $(WX_CXXFLAGS) $(CPPDEPS) -o $@

drawmodedialog.o: drawmodedialog.cpp drawmodedialog.h
	$(CXX) -c $< $(CPPFLAGS) $(WX_CXXFLAGS) $(CPPDEPS) -o $@

main.o: main.cpp
	$(CXX) -c -o $@ $(CPPFLAGS) $(CXXFLAGS) $(WX_CXXFLAGS) $(CPPDEPS) $<

.PHONY: all clean


# Dependencies tracking:
-include ./*.d
