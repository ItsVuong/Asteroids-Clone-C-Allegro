TARGET=a.out
CC=g++
DEBUG=-g
OPT=-O0
WARN=-Wall
ALLEGRO=-lallegro -lallegro_main -lallegro_audio -lallegro_dialog -lallegro_ttf -lallegro_image -lallegro_color -lallegro_memfile -lallegro_acodec -lallegro_primitives -lallegro_font
CCFLAGS=$(DEBUG) $(OPT) $(WARN) $(ALLEGRO)
LD=g++
OBJS= asteroids.o ship.o bullet.o comet.o point.o
all: $(OBJS) ship.o
	$(LD) -o $(TARGET) $(OBJS) $(CCFLAGS)
	@rm *.o
	@./$(TARGET)

asteroids.o: asteroids.cpp
	$(CC) -c $(CCFLAGS) asteroids.cpp -o asteroids.o
ship.o: ship.cpp
	$(CC) -c $(CCFLAGS) ship.cpp -o ship.o
bullet.o: bullet.cpp
	$(CC) -c $(CCFLAGS) bullet.cpp -o bullet.o
comet.o: comet.cpp
	$(CC) -c $(CCFLAGS) comet.cpp -o comet.o
point.o: point.cpp
	$(CC) -c $(CCFLAGS) point.cpp -o point.o
