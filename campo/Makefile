# Linux Makefile for VART Example "Animation"
# (C) Bruno de Oliveira Schneider - May 2014

# This Makefile requires a local directory "vart" with
# source code for the V-ART framework. Create a symbolic
# link to the real directory and you'll be OK.

APPLICATION= main
CXXFLAGS = -Wall -I. -DVART_OGL -DIL_LIB -std=c++11
LDFLAGS = -L/usr/X11R6/lib
LDLIBS = -lGL -lglut -lGLU -lIL

OBJECTS = point4d.o color.o light.o texture.o material.o boundingbox.o memoryobj.o\
sgpath.o snlocator.o scenenode.o graphicobj.o sphere.o\
cylinder.o mesh.o transform.o bezier.o modifier.o dof.o joint.o\
uniaxialjoint.o biaxialjoint.o polyaxialjoint.o camera.o meshobject.o arrow.o\
picknamelocator.o scene.o file.o mousecontrol.o\
viewerglutogl.o main.o

# first, try to compile from this project
%.o: %.cpp %.h
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o $@ -c $<

# second, try to compile from the core
%.o: vart/source/%.cpp vart/%.h
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o $@ -c $<

# last, try to compile from contribs
%.o: vart/contrib/source/%.cpp vart/contrib/%.h
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o $@ -c $<

$(APPLICATION): $(OBJECTS) main.o
	$(CXX) $(LDFLAGS) $^ $(LDLIBS) -o $@

#gdb:
#	CXXFLAGS = $(CXXFLAGS) -ggdb
#	$(APPLICATION)

clean:
	rm -f *.o *~ $(APPLICATION)
