# Makefile for V-ARV
# (C) Bruno de Oliveira Schneider - SET/09/2004

# This Makefile has been created for Linux new targets for other
# platforms should be created later.

# Search the file for "FM:" for "Fix Me" stuff.

# Changelog
# September, 9/2004 - Bruno de Oliveira Schneider
# - Makefile created

# 1. CONFIGURATION VARIABLES
# 1.1 V-ART version
# FM: In the future, version number should come from a version control system,
# or from a file such as config.h .
VERSION = 1.0

# 1.2 Names of the V-ART files
FILES = action.cpp bezier.cpp biaxialjoint.cpp boundingbox.cpp camera.cpp\
color.cpp curve.cpp cylinder.cpp dof.cpp dofmover.cpp dot.cpp graphicobj.cpp\
joint.cpp jointmover.cpp light.cpp linearinterpolator.cpp material.cpp\
memoryobj.cpp mesh.cpp meshobject.cpp modifier.cpp point4d.cpp pointlight.cpp\
polyaxialjoint.cpp rangesineinterpolator.cpp scene.cpp scenenode.cpp\
sineinterpolator.cpp sphere.cpp spotlight.cpp texture.cpp time.cpp\
transform.cpp uniaxialjoint.cpp xmlaction.cpp xmlscene.cpp

# 1.3 Names of the V-ART object files to be created
OBJECTS = action.o bezier.o biaxialjoint.o boundingbox.o camera.o color.o\
curve.o cylinder.o dof.o dofmover.o dot.o graphicobj.o interpolator.o joint.o\
jointmover.o light.o linearinterpolator.o material.o memoryobj.o mesh.o\
meshobject.o modifier.o point4d.o point.o pointlight.o polyaxialjoint.o\
rangesineinterpolator.o scene.o scenenode.o sineinterpolator.o sphere.o\
spotlight.o texture.o time.o transform.o uniaxialjoint.o vart.o xmlaction.o\
xmlscene.o

# 2. FLAGS
CXXFLAGS = -fPIC -I.

# 3. RULES
.PHONY: clean
.PHONY: docs

# Force make to check .h files when deciding whether to recompile a .o file
%.o: %.cpp %.h
	$(CXX) -c $(CXXFLAGS) $(CPPFLAGS) $< -o $@

# 4. TARGETS
help:
	@echo "Possible targets for this Makefile are:"
	@echo "so:            Create Linux dynamic library"
	@echo "documentation: Create DoxyGen documentation (no time-stamp checking)"
	@echo "clean:         Remove temporary files"

# so target: create a dynamic library
so: $(OBJECTS)
	$(CXX) -shared -o libvart.so.$(VERSION) $(OBJECTS)

docs:
	[ ! -d docs/html ] || del docs/*
	doxygen 2>&1 | tee doxygen.out
	cd contrib && doxygen 2>&1 | tee doxygen.out
	cd ..
	vart-packdocs.sh

clean:
	rm -f *.o

test:
	@echo 'versao (maior): $(VERSION_MAJOR)'
	@echo 'versao (menor): $(VERSION_MINOR)'
	@echo 'versao        : $(VERSION)'
