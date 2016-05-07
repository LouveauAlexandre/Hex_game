#ifndef DISPLAY_MENU
#define DISPLAY_MENU

#include <stdbool.h>

#include "display_window.h"

typedef struct Menu_S *Menu;

void init_menu(Menu *m, int nb_bouton, char **text, int x, int y);

void free_menu(Menu *m);

void config_menu(Menu *m, Window w, Uint32 color, TTF_Font *font);

void display_menu(Menu m, Window w);

SDL_Rect* get_pos_menu(Menu m, int i);

bool clic_menu(Menu m, const int X, const int Y);

void modif_menu(Menu *m, Window w, Uint32 color, int i);

void modif_texte_menu(Menu *m, Window w, char *text, TTF_Font *font, int i);

char* get_text_menu(Menu m, int i);

int get_nb_button(Menu m);

#endif