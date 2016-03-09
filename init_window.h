#ifndef INIT_WINDOW
#define INIT_WINDOW

#define LARGEUR 800
#define HAUTEUR 600

#include <SDL/SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#define REPERTOIRE_ICONE "Images/icone_Hex.png"
#define REPERTOIRE_POLICE "Images/FunSized.ttf"
#define TAILLE_POLICE 60
#define TITLE "Jeu de Hex"
#define TITLE_WINDOW "--> | Hex Game | <--"
#define NB_PIXEL 32

#define RFONTCOLOR 0
#define GFONTCOLOR 0
#define BFONTCOLOR 0

typedef struct Window_config *Window;

Window init_window();

void init_sdl_video();

void init_sdl_text();

void check_video(const SDL_Surface *surface);

TTF_Font* get_Font(Window w);

Uint8 get_bpp_video(Window w);

SDL_Color get_font_color(Window w);

SDL_Surface* get_Screen(Window w);

#endif