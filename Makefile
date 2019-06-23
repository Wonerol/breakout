CC = g++
CFLAGS = -std=c++17 -I glad -I glm
LIBS = -lglfw3 -ldl -lX11 -lpthread -lXxf86vm -lXrandr -lXinerama -lXcursor

main: main.cpp glad.o paddle.o brick.o ball.o game_object.o scene.o graphics_system.o graphics_component.o
	${CC} ${CFLAGS} -o bin/breakout main.cpp shader.cpp glad.o paddle.o brick.o ball.o game_object.o scene.o graphics_system.o graphics_component.o ${LIBS}

scene.o: scene.cpp paddle.o brick.o ball.o
	${CC} ${CFLAGS} -c -o scene.o scene.cpp

paddle.o: paddle.cpp game_object.o glad.o
	${CC} ${CFLAGS} -c -o paddle.o paddle.cpp

brick.o: brick.cpp game_object.o glad.o
	${CC} ${CFLAGS} -c -o brick.o brick.cpp

ball.o: ball.cpp game_object.o glad.o
	${CC} ${CFLAGS} -c -o ball.o ball.cpp

game_object.o: game_object.cpp
	${CC} ${CFLAGS} -c -o game_object.o game_object.cpp

graphics_system.o: graphics_system.cpp
	${CC} ${CFLAGS} -c -o graphics_system.o graphics_system.cpp

graphics_component.o: graphics_component.cpp
	${CC} ${CFLAGS} -c -o graphics_component.o graphics_component.cpp

glad.o: glad/glad.c
	${CC} ${CFLAGS} -c -o glad.o glad/glad.c

clean:
	rm *.o breakout
