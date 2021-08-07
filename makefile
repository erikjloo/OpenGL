CPP = g++
CFLAGS = -std=gnu++17 -g3 -O0
opengl: episode12.cpp
	# -std=c++0x allows to use nullptr
	export MESA_GL_VERSION_OVERRIDE=3.3
	${CPP} ${CFLAGS} -o app episode12.cpp -lglfw -lGL -lGLEW -std=c++0x
