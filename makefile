CC=gcc
OPT=-Wall -g
LDFLAGS=`sdl-config --cflags --libs` -lSDL_image -lSDL_ttf
EXEC=test_Hex

all: $(EXEC)

init_window.o : init_window.c
	$(CC) -c init_window.c -o init_window.o $(LDFLAGS) $(OPT)

close_window.o : close_window.c
	$(CC) -c close_window.c -o close_window.o $(LDFLAGS) $(OPT)
	
affichage.o : affichage.c
	$(CC) -c affichage.c -o affichage.o $(LDFLAGS) $(OPT)
	
main.o : main.c
	$(CC) -c main.c -o main.o $(LDFLAGS) $(OPT)
	
$(EXEC): init_window.o close_window.o affichage.o main.o
	$(CC) init_window.o close_window.o affichage.o main.o -o $(EXEC) $(LDFLAGS)

clean:
	rm -rf *.o *.exe *.out $(EXEC)

