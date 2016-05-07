CC=gcc
OPT=-Wall -g -lm -std=c99
LDFLAGS=`sdl-config --cflags --libs` -lSDL_image -lSDL_ttf
EXEC=Hex

all: $(EXEC)

display_window.o : display_window.c
	$(CC) -c display_window.c -o display_window.o $(LDFLAGS) $(OPT)
	
display_interface.o : display_interface.c
	$(CC) -c display_interface.c -o display_interface.o $(LDFLAGS) $(OPT)
	
background.o : background.c
	$(CC) -c background.c -o background.o $(LDFLAGS) $(OPT)
	
display_menu.o : display_menu.c
	$(CC) -c display_menu.c -o display_menu.o $(LDFLAGS) $(OPT)
	
play.o : play.c
	$(CC) -c play.c -o play.o $(LDFLAGS) $(OPT)
	
game_option.o : game_option.c
	$(CC) -c game_option.c -o game_option.o $(LDFLAGS) $(OPT)
	
main.o : main.c
	$(CC) -c main.c -o main.o $(LDFLAGS) $(OPT)
	
$(EXEC): display_window.o display_interface.o background.o display_menu.o play.o game_option.o main.o 
	$(CC) display_window.o display_interface.o background.o display_menu.o play.o game_option.o main.o  -o $(EXEC) $(LDFLAGS) $(OPT)

clean:
	rm -rf *.o *.exe *.out $(EXEC)

