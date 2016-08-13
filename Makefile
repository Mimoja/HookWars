TARGET = game

CC     = cc
CPP    = c++
CPPFLAGS = -pthread -O2 --std=c++11 -Wextra -Wpedantic -g -Ideps/assimp/include/ -Ideps/glfw/include -Ideps/glm

EXTRA_DEFINES  = -DLINUX -DGLM_FORCE_RADIANS -DPROG_NAME=\"$(TARGET)\"
LFLAGS       = $(LIB_LINUX) -lXrandr -lGL -lXi -pthread -lm -lX11 -lXxf86vm

LIBS         =  deps/glfw/src/libglfw3.a deps/assimp/lib/libassimp.so deps/glew/lib/libGLEW.a

OBJECTS = $(patsubst %.cpp, %.o, $(wildcard *.cpp))

$(TARGET): $(LIBS) $(OBJECTS) 
	$(CPP) $(OBJECTS) $(LIBS) $(LFLAGS) -o $(TARGET)

%.o: %.cpp
	$(CPP) $(CPPFLAGS) $(EXTRA_DEFINES) -c $^ -o $@

deps/glfw/src/libglfw3.a:
	( cd deps/glfw && cmake . && make -j8)

deps/assimp/lib/libassimp.so:
	( cd deps/assimp && cmake CMakeLists.txt -DASSIMP_BUILD_TESTS=NO && make -j8)

deps/glew/lib/libGLEW.a:
	( cd deps/glew && make extensions && make -j8)

clean:
	rm -f *.o
	rm -f $(TARGET)
mrproper: clean
	( cd deps/glfw && make clean)
	( cd deps/assimp && make clean -j8)
	( cd deps/glew && make clean -j8)

