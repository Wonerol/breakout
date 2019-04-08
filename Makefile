CC = g++
CFLAGS = -std=c++17 -I glad -I glm
LIBS = -lglfw3 -ldl -lX11 -lpthread -lXxf86vm -lXrandr -lXinerama -lXcursor

main: main.cpp glad.o paddle.o brick.o ball.o game_object.o scene.o square.o
	${CC} ${CFLAGS} -o breakout main.cpp shader.cpp glad.o paddle.o brick.o ball.o game_object.o scene.o square.o ${LIBS}

scene.o: scene.cpp paddle.o brick.o ball.o
	${CC} ${CFLAGS} -c -o scene.o scene.cpp

paddle.o: paddle.cpp game_object.o glad.o
	${CC} ${CFLAGS} -c -o paddle.o paddle.cpp

brick.o: brick.cpp game_object.o glad.o
	${CC} ${CFLAGS} -c -o brick.o brick.cpp

ball.o: ball.cpp game_object.o glad.o
	${CC} ${CFLAGS} -c -o ball.o ball.cpp

game_object.o: game_object.cpp square.o
	${CC} ${CFLAGS} -c -o game_object.o game_object.cpp

square.o: square.cpp
	${CC} ${CFLAGS} -c -o square.o square.cpp

glad.o: glad/glad.c
	${CC} ${CFLAGS} -c -o glad.o glad/glad.c

clean:
	rm *.o breakout
