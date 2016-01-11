TARGET = game

CC     = cc
CPP    = c++
CPPFLAGS = -pthread -O2 -Wall --std=c++11 -Wextra

EXTRA_DEFINES  = -DLINUX -DGLM_FORCE_RADIANS -DPROG_NAME=\"$(TARGET)\"
LFLAGS       = $(LIB_LINUX) -lXrandr -lGL -lXi -pthread -lm -lX11 -lXxf86vm -lGLEW -lglfw -lassimp

OBJECTS = $(patsubst %.cpp, %.o, $(wildcard *.cpp))

$(TARGET): $(OBJECTS)
	$(CPP) $(OBJECTS) $(LFLAGS) -o $(TARGET)

%.o: %.cpp
	$(CPP) $(CPPFLAGS) $(EXTRA_DEFINES) -c $^ -o $@

clean:
	rm -f *.o
	rm -f $(TARGET)
