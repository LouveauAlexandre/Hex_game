#ifndef AFFICHAGE
#define AFFICHAGE

#define LARGEUR 800
#define HAUTEUR 600

#include <SDL/SDL.h>

#define REPERTOIRE_ICONE "Images/icone_Hex.png"
#define REPERTOIRE_PLATEAU "Images/hex.png"
#define REPERTOIRE_POLICE "Images/FunSized.ttf"
#define TAILLE_POLICE 60
#define TITLE "Jeu de Hex"
#define TITLE_WINDOW "--> | Hex Game | <--"


void init_sdl_video();

void init_sdl_text();

void check_video(const SDL_Surface *surface);

void pause();

void afficher_menu();

#endif