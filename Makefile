CPP = g++
CPPFLAGS = -O3 -ffast-math -flto -march=native `pkg-config --cflags freeglut`
LIBS = `pkg-config --libs freeglut` -lopengl32
EXT=out

.phony: all clean

OBJS += core.o
OBJS += examples/img.o
OBJS += examples/app.o
OBJS += examples/stb_image_write.o

EXAMPLES_SRC := $(wildcard examples/ex*.cpp)
EXAMPLES := $(EXAMPLES_SRC:examples/ex%.cpp=examples/ex%.$(EXT))

all: $(EXAMPLES)

%.o: %.cpp %.hpp
	$(CPP) $(CPPFLAGS) -c -o $@ $<

%.$(EXT): %.cpp $(OBJS)
	$(CPP) $(CPPFLAGS) -c -o $*.o $*.cpp
	$(CPP) $(CPPFLAGS) $(OBJS) $*.o -o $*.$(EXT) $(LIBS)

examples/ex*.o: core.hpp examples/img.hpp examples/app.hpp
examples/app.o: core.hpp examples/img.hpp
examples/img.o: core.hpp examples/stb_image_write.hpp

clean:
	-find . -type f -name '*.o' -delete
	-find . -type f -name '*.$(EXT)' -delete
