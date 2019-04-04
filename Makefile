CC = g++
CFLAGS = -std=c++17 -I glad -I glm
LIBS = -lglfw3 -ldl -lX11 -lpthread -lXxf86vm -lXrandr -lXinerama -lXcursor

main: main.cpp glad.o paddle.o
	${CC} ${CFLAGS} -o breakout main.cpp shader.cpp glad.o paddle.o ${LIBS}

glad.o: glad/glad.c
	${CC} ${CFLAGS} -c -o glad.o glad/glad.c

paddle.o: paddle.cpp
	${CC} ${CFLAGS} -c -o paddle.o paddle.cpp
clean:
	rm *.o breakout
