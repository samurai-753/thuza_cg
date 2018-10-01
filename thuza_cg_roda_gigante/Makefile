# Linux Makefile for VART Example "OBJ Viewer"
# (C) Bruno de Oliveira Schneider - May 2014

# This Makefile requires a local directory "vart" with
# source code for the V-ART framework. Create a symbolic
# link to the real directory and you'll be OK.

APPLICATION = main
CXXFLAGS = -Wall -I. -DVART_OGL -DIL_LIB -std=c++11
LDFLAGS = -L/usr/X11R6/lib
LDLIBS = -lGL -lglut -lGLU -lIL

OBJECTS = mesh.o memoryobj.o\
mousecontrol.o meshobject.o bezier.o modifier.o dof.o\
file.o color.o texture.o material.o joint.o box.o\
boundingbox.o sgpath.o snlocator.o scenenode.o camera.o transform.o\
viewerglutogl.o graphicobj.o sphere.o point4d.o\
picknamelocator.o light.o cylinder.o scene.o arrow.o


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

debug: $(OBJECTS)
	$(CXX) -DDEBUG $(CXXFLAGS) main.cpp -c -o main.o
	$(CXX) $(LDFLAGS) -o $(APPLICATION) main.o $^

#gdb:
#	CXXFLAGS = $(CXXFLAGS) -ggdb
#	$(APPLICATION)

clean:
	rm -f *.o *~ $(APPLICATION)
