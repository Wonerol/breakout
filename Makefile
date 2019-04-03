CC = g++
CFLAGS = -I glad
LIBS = -lglfw3 -ldl -lX11 -lpthread -lXxf86vm -lXrandr -lXinerama -lXcursor

main: main.cpp
	${CC} ${CFLAGS} -o breakout main.cpp glad/glad.c ${LIBS}
