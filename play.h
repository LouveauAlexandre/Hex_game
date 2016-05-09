#ifndef PLAY
#define PLAY

#include <stdbool.h>

#include "display_window.h"
#include "display_menu.h"
#include "display_interface.h"

#define CHEMIN_PLATEAU "Images/hex.png"
#define CHEMIN_PION1 "Images/button-blue22.png"
#define CHEMIN_PION2 "Images/button-red22.png"

#define NB_COLONNE 11
#define NB_LIGNE 11

#define VIDE 'E'
#define ROUGE 'R'
#define BLEU 'B'

#define START 0
#define HAUT 1
#define BAS 2
#define GAUCHE 3
#define DROITE 4

typedef struct Bot_IA_S *Bot_IA;

struct Position_S{
    int x;
    int y;
    char color;
    struct Position_S **next;
    int nb_next;
    int i;
    int j;
    bool visited;
};

typedef struct Position_S *Position;

struct Node_S{
    Position hexa;
    int dir;
    int sens;
    struct Node_S *prev;
    struct Node_S *next;
};

typedef struct Node_S *Node;

struct Stack_S{
    Node sentinel;
    Node current;
};

typedef struct Stack_S *Stack;

bool empty_stack(Stack S);

void init_stack(Stack *S);

void stack_hexa(Stack *S, Position hexa, int dir, int sens);

void unstack_hexa(Stack *S);

void map_stack(Stack S);

bool play(Window w, Menu Interface[NB_INTERFACE], bool load, int ia);

#endif