#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <SDL_image.h>
#include <SDL_ttf.h>
#include "affichage.h"

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
        fprintf(stderr, "Impossible de charger le mode vidéo %s\n", SDL_GetError());
        SDL_Quit();
        exit(EXIT_FAILURE);
    }
}

/*------------------------------------------------------------------*/

void pause(){

    bool stop = false;
    SDL_Event event;
    SDLKey key_pressed;
    
    while(!stop){
        SDL_WaitEvent(&event);
        switch(event.type){
            case SDL_QUIT :
                stop = true;
                break;
                
            case SDL_MOUSEBUTTONDOWN:
                printf("X=%d Y=%d\n",event.button.x,event.button.y); // on récupère les coordonnées du clic
                break;
                
            case SDL_KEYDOWN :
                key_pressed = event.key.keysym.sym; // on récupère la touche
                switch (key_pressed){
                    case SDLK_ESCAPE :
                        stop = true;
                        break;
                        
                    default :
                        printf("%d %c\n", key_pressed, key_pressed);
                        break;
                }
            break;
        }
    }
}
    
/*------------------------------------------------------------------*/

typedef struct{
    SDL_Surface *surface;
    SDL_Rect pos;
}Surface;

void afficher_menu(){
    
    SDL_Surface *screen;
    Uint8  bit_color = 32; // 32 bits de couleur
    Uint32 videoflags = SDL_HWSURFACE | SDL_DOUBLEBUF;
    int i;
    
    init_sdl_video();
    screen = SDL_SetVideoMode(LARGEUR, HAUTEUR, bit_color, videoflags); // fenetre pour la surface principale
    check_video(screen);
    
    init_sdl_text();
    
    // nom de la fenetre
    SDL_WM_SetCaption(TITLE_WINDOW, NULL);
    // icone de la fenetre
    SDL_WM_SetIcon(IMG_Load(REPERTOIRE_ICONE), NULL);
    
    TTF_Font *police = TTF_OpenFont(REPERTOIRE_POLICE, TAILLE_POLICE);
    SDL_Color policeNoire = {0,0,0};
    
    Surface title;
    title.surface = TTF_RenderText_Blended(police, TITLE, policeNoire);
    
    title.pos.x = LARGEUR/2 - title.surface->w/2;
    title.pos.y = 25;
    
    //affichage des cadres
    
    Surface cadre1, cadre2;
    cadre1.surface = SDL_CreateRGBSurface(SDL_HWSURFACE, title.surface->w + 10, title.surface->h, bit_color, 0, 0, 0, 0);
    cadre2.surface = SDL_CreateRGBSurface(SDL_HWSURFACE, title.surface->w + 20, title.surface->h + 10, bit_color, 0, 0, 0, 0);
    
    SDL_FillRect(cadre1.surface, NULL, SDL_MapRGB(screen->format, 255, 0, 0));
    SDL_FillRect(cadre2.surface, NULL, SDL_MapRGB(screen->format, 0, 0, 0));
    
    cadre1.pos.x = title.pos.x - 5;
    cadre1.pos.y = title.pos.y;
    
    cadre2.pos.x = title.pos.x - 10;
    cadre2.pos.y = title.pos.y - 5;
    
    //affichage du plateau
    Surface plateau;
    plateau.surface = IMG_Load(REPERTOIRE_PLATEAU);
    plateau.pos.x = LARGEUR/2 - plateau.surface->w/2;
    plateau.pos.y = HAUTEUR/2 - plateau.surface->h/2;
    
    //afficahe fond dégradé
    Surface line[HAUTEUR];
    
    for(i=0; i<HAUTEUR; ++i){
        line[i].surface = SDL_CreateRGBSurface(SDL_HWSURFACE, LARGEUR, 1, bit_color, 0, 0, 0, 0);
        //valeurs entre 0 et 255
        int borne_min = 100;
        int borne_max = 200;
        float coef = (HAUTEUR*borne_min)/(256-borne_min);
        int couleur = ((float)(i+coef))/(HAUTEUR+coef)*(borne_max+1);
        
        line[i].pos.x = 0;
        line[i].pos.y = i;
        SDL_FillRect(line[i].surface, NULL, SDL_MapRGB(screen->format, couleur, couleur, couleur));
        SDL_BlitSurface(line[i].surface, NULL, screen, &line[i].pos);
    }
    
    SDL_BlitSurface(cadre2.surface, NULL, screen, &cadre2.pos);
    SDL_BlitSurface(cadre1.surface, NULL, screen, &cadre1.pos);
    SDL_BlitSurface(title.surface, NULL, screen, &title.pos);
    SDL_BlitSurface(plateau.surface, NULL, screen, &plateau.pos);
    
    pause();
    
    for(i=0; i<HAUTEUR; ++i)
        SDL_FreeSurface(line[i].surface);
    
    SDL_FreeSurface(cadre1.surface);
    SDL_FreeSurface(cadre2.surface);
    SDL_FreeSurface(plateau.surface);
    SDL_FreeSurface(title.surface);
    TTF_CloseFont(police);
    TTF_Quit();
    SDL_Quit(); // SDL_FreeSurface(screen);
    
}