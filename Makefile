CC = g++
CFLAGS = -std=c++17 -I glad -I glm
LIBS = -lglfw3 -ldl -lX11 -lpthread -lXxf86vm -lXrandr -lXinerama -lXcursor

main: main.cpp
	${CC} ${CFLAGS} -o breakout main.cpp shader.cpp glad/glad.c ${LIBS}
