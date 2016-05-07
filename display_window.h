#ifndef INIT_WINDOW
#define INIT_WINDOW

#include <SDL/SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#define LARGEUR 1200
#define HAUTEUR 700

#define NB_FONT 3

#define REPERTOIRE_ICONE "Images/icone_Hex.png"
#define REPERTOIRE_POLICE_TITRE "Images/FunSized.ttf"
#define REPERTOIRE_POLICE "Images/04B_30.ttf"

#define TAILLE_POLICE_TITRE 80
#define TAILLE_POLICE 30
#define TAILLE_POLICE_MAIN 50

#define TITLE_WINDOW "--> | Hex Game | <--"

#define BPP_VIDEO 32

#define RFONTCOLOR 0
#define GFONTCOLOR 0
#define BFONTCOLOR 0

#define NB_COLOR 7
#define BLACK 0
#define RED 1
#define BLUE 2
#define GREEN 3
#define CYAN 4
#define YELLOW 5
#define PURPLE 6

typedef struct Window_config *Window;

Window init_window();

void init_sdl_video();

void init_sdl_text();

void check_video(const SDL_Surface *surface);

TTF_Font* get_Font(Window w, int i);

Uint8 get_bpp_video(Window w);

SDL_Color get_font_color(Window w);

Uint32 get_color(Window w, int i);

SDL_Surface* get_screen(Window w);

void close_window(Window *w);

#endif