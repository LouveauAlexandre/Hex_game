#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <SDL/SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "display_window.h"

struct Window_config{
    SDL_Surface *screen;
    Uint32 videoflags;
    TTF_Font **font;
    SDL_Color font_color;
    Uint32 Color[NB_COLOR];
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

TTF_Font* get_Font(Window w, int i){
    return(w->font[i]);
}

/*------------------------------------------------------------------*/

SDL_Color get_font_color(Window w){
    return(w->font_color);
}

/*------------------------------------------------------------------*/

SDL_Surface* get_screen(Window w){
    return(w->screen);
}

/*------------------------------------------------------------------*/

void init_color(Window w, Uint32 *c){
    
    c[BLACK] = SDL_MapRGB(w->screen->format, 50, 0, 200);
    c[RED] = SDL_MapRGB(w->screen->format, 205, 38, 38);
    c[GREEN] = SDL_MapRGB(w->screen->format, 205, 192, 176);
    c[CYAN] = SDL_MapRGB(w->screen->format, 0, 200, 200);
}

/*------------------------------------------------------------------*/

Uint32 get_color(Window w, int i){
    return w->Color[i];
}

/*------------------------------------------------------------------*/

Window init_window(){
    
    Window w = (Window)malloc(sizeof(struct Window_config));
    
    w->videoflags = SDL_HWSURFACE | SDL_DOUBLEBUF;
    init_sdl_video();
    
    w->screen = SDL_SetVideoMode(LARGEUR, HAUTEUR, BPP_VIDEO, w->videoflags); 
    // fenetre pour la surface principale
    
    check_video(w->screen);
    init_sdl_text();
    
    init_color(w, w->Color);
    //liste des couleurs des surfaces
    
    w->font_color.r = RFONTCOLOR;
    w->font_color.g = GFONTCOLOR;
    w->font_color.b = BFONTCOLOR;
    
    w->font = malloc(sizeof(TTF_Font*) * NB_FONT);
    
    w->font[0] = TTF_OpenFont(REPERTOIRE_POLICE_TITRE, TAILLE_POLICE_TITRE);
    w->font[1] = TTF_OpenFont(REPERTOIRE_POLICE, TAILLE_POLICE_MAIN);
    w->font[2] = TTF_OpenFont(REPERTOIRE_POLICE, TAILLE_POLICE);
    
    // nom de la fenetre
    SDL_WM_SetCaption(TITLE_WINDOW, NULL);
    // icone de la fenetre
    SDL_WM_SetIcon(IMG_Load(REPERTOIRE_ICONE), NULL);
    
    return(w);
}

/*------------------------------------------------------------------*/

void close_window(Window *w){
    
    SDL_FreeSurface((*w)->screen);
    
    int i;
    for(i=0; i<NB_FONT; ++i)
        TTF_CloseFont((*w)->font[i]);
    
    free((*w)->font);
        
    TTF_Quit();
    SDL_Quit(); // équivalent à SDL_FreeSurface(screen);
    free(*w);
    
}