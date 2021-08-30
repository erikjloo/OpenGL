# the compiler: gcc for C program, define as g++ for C++
SRC_FILES := $(wildcard src/*.cpp)
IMGUI_FILES := $(wildcard src/vendor/imgui/*.cpp)
CPP = g++
CPPFLAGS = -std=gnu++17 -Wall -g -O0
# compiler flags:
#  -g     - this flag adds debugging information to the executable file
#  -Wall  - this flag is used to turn on most compiler warnings
# export MESA_GL_VERSION_OVERRIDE=3.3

main: src/main.cpp
	${CPP} ${CPPFLAGS} $(SRC_FILES) $(IMGUI_FILES) -o app -lglfw -lGL -lGLEW -ldl