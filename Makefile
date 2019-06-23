BUILDDIR = build
BINDIR = bin
SRCDIR = src
IDIR = include

CC = g++
CFLAGS = -std=c++17 -I $(IDIR) -I $(IDIR)/glad
LIBS = -lglfw3 -ldl -lX11 -lpthread -lXxf86vm -lXrandr -lXinerama -lXcursor

main: $(SRCDIR)/main.cpp $(BUILDDIR)/glad.o $(BUILDDIR)/paddle.o $(BUILDDIR)/brick.o $(BUILDDIR)/ball.o $(BUILDDIR)/game_object.o $(BUILDDIR)/scene.o $(BUILDDIR)/graphics_system.o $(BUILDDIR)/graphics_component.o
	${CC} ${CFLAGS} -o $(BINDIR)/breakout $(SRCDIR)/main.cpp $(SRCDIR)/shader.cpp $(BUILDDIR)/*.o ${LIBS}
	cp -r src/shaders $(BINDIR)

$(BUILDDIR)/scene.o: $(SRCDIR)/scene.cpp $(BUILDDIR)/paddle.o $(BUILDDIR)/brick.o $(BUILDDIR)/ball.o
	${CC} ${CFLAGS} -c -o $(BUILDDIR)/scene.o $(SRCDIR)/scene.cpp

$(BUILDDIR)/paddle.o: $(SRCDIR)/paddle.cpp $(BUILDDIR)/game_object.o $(BUILDDIR)/glad.o
	${CC} ${CFLAGS} -c -o $(BUILDDIR)/paddle.o $(SRCDIR)/paddle.cpp

$(BUILDDIR)/brick.o: $(SRCDIR)/brick.cpp $(BUILDDIR)/game_object.o $(BUILDDIR)/glad.o
	${CC} ${CFLAGS} -c -o $(BUILDDIR)/brick.o $(SRCDIR)/brick.cpp

$(BUILDDIR)/ball.o: $(SRCDIR)/ball.cpp $(BUILDDIR)/game_object.o $(BUILDDIR)/glad.o
	${CC} ${CFLAGS} -c -o $(BUILDDIR)/ball.o $(SRCDIR)/ball.cpp

$(BUILDDIR)/game_object.o: $(SRCDIR)/game_object.cpp
	${CC} ${CFLAGS} -c -o $(BUILDDIR)/game_object.o $(SRCDIR)/game_object.cpp

$(BUILDDIR)/graphics_system.o: $(SRCDIR)/graphics_system.cpp
	${CC} ${CFLAGS} -c -o $(BUILDDIR)/graphics_system.o $(SRCDIR)/graphics_system.cpp

$(BUILDDIR)/graphics_component.o: $(SRCDIR)/graphics_component.cpp
	${CC} ${CFLAGS} -c -o $(BUILDDIR)/graphics_component.o $(SRCDIR)/graphics_component.cpp

$(BUILDDIR)/glad.o: $(IDIR)/glad/glad.c
	${CC} ${CFLAGS} -c -o $(BUILDDIR)/glad.o $(IDIR)/glad/glad.c

clean:
	rm $(BUILDDIR)/*.o $(BINDIR)/breakout
