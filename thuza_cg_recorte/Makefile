CXX = g++
CXXFLAGS = -Wall
GLLIBS = -lGL -lglut -lGLU
OBJS = line.o point.o clip-rectangle.o
ALLOBJS = $(OBJS) openGL.o cli.o
BINS = openGL

# Criar executaveis a partir de objetos usando compilador de C++
%: %.o
	$(CXX) $(LDFLAGS) $@.o $(LOADLIBES) $(LDLIBS) -o $@

all: $(BINS)

openGL: openGL.o $(OBJS)
	$(CXX) $(LDFLAGS) $^ $(LOADLIBES) $(GLLIBS) -o $@

cli: cli.o $(OBJS)
	$(CXX) $(LDFLAGS) $^ $(LOADLIBES) $(LDLIBS) -o $@

clean:
	rm -f *~ $(ALLOBJS) $(BINS)

