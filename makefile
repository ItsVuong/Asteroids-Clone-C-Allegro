TARGET=a.out
CC=g++
DEBUG=-g
OPT=-O0
WARN=-Wall
ALLEGRO=-lallegro -lallegro_main -lallegro_audio -lallegro_dialog -lallegro_ttf -lallegro_image -lallegro_color -lallegro_memfile -lallegro_acodec -lallegro_primitives -lallegro_font
CCFLAGS=$(DEBUG) $(OPT) $(WARN) $(ALLEGRO)
LD=g++
OBJS= game.o ship.o bullet.o comet.o point.o collision.o
all: $(OBJS)
	$(LD) -o $(TARGET) $(OBJS) $(CCFLAGS)
	@rm *.o
	@./$(TARGET)

game.o: game.cpp
	$(CC) -c $(CCFLAGS) game.cpp -o game.o
ship.o: ship.cpp
	$(CC) -c $(CCFLAGS) ship.cpp -o ship.o
bullet.o: bullet.cpp
	$(CC) -c $(CCFLAGS) bullet.cpp -o bullet.o
comet.o: comet.cpp
	$(CC) -c $(CCFLAGS) comet.cpp -o comet.o
point.o: point.cpp
	$(CC) -c $(CCFLAGS) point.cpp -o point.o
collision.o: collision.cpp
	$(CC) -c $(CCFLAGS) collision.cpp -o collision.o
