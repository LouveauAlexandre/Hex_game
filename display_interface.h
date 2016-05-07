#ifndef DISPLAY_INTERFACE
#define DISPLAY_INTERFACE

#include <stdbool.h>

#include "display_window.h"

#define TITLE 0
#define MENU_1 1
#define MENU_2 2
#define MENU_3 3
#define INFO 4
#define HIST 5

#define NB_INTERFACE 6

#define NB_BOUTON_TITLE 1
#define POS_X_TITLE LARGEUR/3 - 50
#define POS_Y_TITLE 5

#define NB_BOUTON_MENU_1 3
#define POS_X_MENU_1 10
#define POS_Y_MENU_1 HAUTEUR/3

#define NB_BOUTON_MENU_2 3
#define POS_X_MENU_2 LARGEUR/3 - 46
#define POS_Y_MENU_2 HAUTEUR/3

#define NB_BOUTON_MENU_3 4
#define POS_X_MENU_3 10
#define POS_Y_MENU_3 HAUTEUR/3

#define NB_BOUTON_INFO 1
#define POS_X_INFO 10
#define POS_Y_INFO HAUTEUR-100

#define NB_BOUTON_HIST 19
#define POS_X_HIST LARGEUR-310
#define POS_Y_HIST 5

#define MAX_CARACTERE 35

typedef struct Surface_S *Surface;

typedef struct Color_S *Color;

void modif_surface(Surface *s, SDL_Surface *s_elem);

void init_surface(Surface *s);

SDL_Surface* get_surface(Surface s);

SDL_Rect* get_pos(Surface s);

void afficher_interface(Window w);

void config_surface(Surface *s, int w, int h, int x, int y, Uint32 color);

void config_text(Surface *s, int x, int y, TTF_Font *font, char *text, SDL_Color font_color);

bool clic_area(SDL_Rect *area, const int X, const int Y);

#include "background.h"
#include "display_menu.h"
#include "play.h"

#endif