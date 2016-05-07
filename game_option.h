#ifndef GAME_OPTION
#define GAME_OPTION

#include <stdbool.h>
#include "play.h"

#define FICH_SAVE "HexSave.txt"
#define ERR_FICH 1
#define ERR_GRILLE 2
#define ERR_STACK 3

int Sauvegarder(Position grille[NB_LIGNE][NB_COLONNE], Stack S);

int Charger(Position grille[NB_LIGNE][NB_COLONNE], Stack *S, bool *player);

#endif