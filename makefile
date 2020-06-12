opengl: application.cpp
	# g++ -o app main.cpp pkg-config --cflags glfw glew
	g++ -o app episode3.cpp -lglfw -lGL -lGLEW