#include <stdio.h>
#include <stdlib.h>

#include "init_window.h"

struct Window_config{
    SDL_Surface *screen;
    Uint8  bpp_video;
    Uint32 videoflags;
    TTF_Font *font;
    SDL_Color font_color;
};

/*------------------------------------------------------------------*/

void init_sdl_video(){
    
    if(SDL_Init(SDL_INIT_VIDEO) == -1){
        fprintf(stderr, "Erreur d'initialisation de la SDL : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
}

/*------------------------------------------------------------------*/

void init_sdl_text(){
     if( TTF_Init() == -1 ){
        fprintf(stderr, "Erreur d'initialisation de TTF_Init : %s\n", TTF_GetError());
        SDL_Quit();
        exit(EXIT_FAILURE);
    }
}

/*------------------------------------------------------------------*/

void check_video(const SDL_Surface *surface){
    if(surface == NULL){
        fprintf(stderr, "Impossible de charger le mode vidéo : %s\n", SDL_GetError());
        SDL_Quit();
        exit(EXIT_FAILURE);
    }
}

/*------------------------------------------------------------------*/

TTF_Font* get_Font(Window w){
    return(w->font);
}

/*------------------------------------------------------------------*/

Uint8 get_bpp_video(Window w){
    return(w->bpp_video);
}

/*------------------------------------------------------------------*/

SDL_Color get_font_color(Window w){
    return(w->font_color);
}

/*------------------------------------------------------------------*/

SDL_Surface* get_Screen(Window w){
    return(w->screen);
}

/*------------------------------------------------------------------*/

Window init_window(){
    
    Window w = (Window)malloc(sizeof(struct Window_config));
    
    w->bpp_video = NB_PIXEL;
    w->videoflags = SDL_HWSURFACE | SDL_DOUBLEBUF;
    
    init_sdl_video();
    
    w->screen = SDL_SetVideoMode(LARGEUR, HAUTEUR, w->bpp_video, w->videoflags); // fenetre pour la surface principale
    
    check_video(w->screen);
    
    init_sdl_text();
    
    w->font = TTF_OpenFont(REPERTOIRE_POLICE, TAILLE_POLICE);
    w->font_color.r = RFONTCOLOR;
    w->font_color.g = GFONTCOLOR;
    w->font_color.b = BFONTCOLOR;
    
    // nom de la fenetre
    SDL_WM_SetCaption(TITLE_WINDOW, NULL);
    // icone de la fenetre
    SDL_WM_SetIcon(IMG_Load(REPERTOIRE_ICONE), NULL);
    
    return(w);
}