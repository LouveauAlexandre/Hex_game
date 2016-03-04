CC=gcc
OPT=-Wall -g -lm
LDFLAGS=`sdl-config --cflags --libs` -lSDL_image -lSDL_ttf
EXEC=test_Hex

all: $(EXEC)

affichage.o : affichage.c
	$(CC) -c affichage.c -o affichage.o $(LDFLAGS)

main.o : main.c
	$(CC) -c main.c -o main.o $(OPT) 
	
$(EXEC):  affichage.o main.o
	$(CC) affichage.o main.o -o $(EXEC) $(LDFLAGS)
	
clean:
	rm -rf *.o *.exe *.out $(EXEC)

