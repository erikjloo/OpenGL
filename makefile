# the compiler: gcc for C program, define as g++ for C++
CPP = g++
# compiler flags:
#  -g     - this flag adds debugging information to the executable file
#  -Wall  - this flag is used to turn on most compiler warnings
CFLAGS = -std=gnu++17 -Wall -g -O0

main: src/main.cpp
	export MESA_GL_VERSION_OVERRIDE=3.3
	${CPP} ${CFLAGS} -o app src/main.cpp src/Util.cpp src/Shader.cpp src/VertexBufferLayout.cpp -lglfw -lGL -lGLEW
