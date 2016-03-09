#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "affichage.h"

typedef struct{
    SDL_Surface *surface;
    SDL_Rect pos;
}Surface;


/*------------------------------------------------------------------*/

void config_surface(Surface *s, int w, int h, int x, int y, Uint8 bpp_video, Uint32 color){
    
    (*s).surface = SDL_CreateRGBSurface(SDL_HWSURFACE, w, h, bpp_video, 0, 0, 0, 0);
    SDL_FillRect((*s).surface, NULL, color);
    (*s).pos.x = x;
    (*s).pos.y = y;
    
}

/*------------------------------------------------------------------*/

void config_text(Surface *s, int x, int y, TTF_Font *font, char *text, SDL_Color font_color){

    (*s).surface = TTF_RenderText_Blended(font, text, font_color);
    (*s).pos.x = x - (*s).surface->w/2;
    (*s).pos.y = y; 
   
}
    
/*------------------------------------------------------------------*/

void background_color(Window w, Surface *background){
    
    int i;
    for(i=0; i<HAUTEUR; ++i){
        //valeurs entre 0 et 255
        int borne_min = 100;
        int borne_max = 200;
        float coef = (HAUTEUR*borne_min)/(256-borne_min);
        int couleur = ((float)(i+coef))/(HAUTEUR+coef)*(borne_max+1);
        config_surface(&background[i], LARGEUR, 1, 0, i, get_bpp_video(w), SDL_MapRGB(get_Screen(w)->format, couleur, couleur, couleur));
    }
        
}

/*------------------------------------------------------------------*/

void afficher_background(Window w, Surface *background){
    
    int i;
    for(i=0; i<HAUTEUR; ++i)
        SDL_BlitSurface(background[i].surface, NULL, get_Screen(w), &background[i].pos);
    
}
    
/*------------------------------------------------------------------*/

void pause(Window w, Surface *background, Surface play, Surface quit){

    bool stop = false;
    SDL_Event event;
    SDLKey key_pressed;
    
                        //affichage du plateau
                    Surface plateau;
                    plateau.surface = IMG_Load(REPERTOIRE_PLATEAU);
                    plateau.pos.x = LARGEUR/2 - plateau.surface->w/2;
                    plateau.pos.y = HAUTEUR/2 - plateau.surface->h/2;
    
    int clicX, clicY;
    
    while(!stop){
        SDL_WaitEvent(&event);
        switch(event.type){
            case SDL_QUIT :
                stop = true;
                break;
                
            case SDL_MOUSEBUTTONDOWN:
                clicX = event.button.x;
                clicY = event.button.y;
                printf("X=%d Y=%d\n",clicX, clicY); // on récupère les coordonnées du clic
                
                if(clicX >= quit.pos.x && clicX <= quit.pos.x + quit.surface->w && clicY >= quit.pos.y && clicY <= quit.pos.y + quit.surface->h)
                    stop = true;
                
                if(clicX >= play.pos.x && clicX <= play.pos.x + play.surface->w && clicY >= play.pos.y && clicY <= play.pos.y + play.surface->h){
                    afficher_background(w, background);
                    SDL_BlitSurface(plateau.surface, NULL, get_Screen(w), &plateau.pos);
                }
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
        SDL_Flip(get_Screen(w)); 
    }
}
    
/*------------------------------------------------------------------*/

void afficher_menu(Window w){
    
    SDL_Surface *screen = get_Screen(w);
    Uint32 red = SDL_MapRGB(screen->format, 255, 0, 0);
    Uint32 black = SDL_MapRGB(screen->format, 0, 0, 0);
    Uint8 bpp_video = get_bpp_video(w);
    TTF_Font *font = get_Font(w);
    SDL_Color font_color = get_font_color(w);
    int i;
    
    Surface title;
    
    config_text(&title, LARGEUR/2, HAUTEUR/20, font, TITLE, font_color);
    
    //affichage des cadres
    
    Surface title_2, title_1;
    
    config_surface(&title_2, title.surface->w + 10, title.surface->h, title.pos.x - 5, title.pos.y, bpp_video, red);
    config_surface(&title_1, title.surface->w + 20, title.surface->h + 10, title.pos.x - 10, title.pos.y - 5, bpp_video, black);
    
    Surface play_2, play_1;
    
    config_surface(&play_2, title_2.surface->w, title_2.surface->h, title_2.pos.x, HAUTEUR/3, bpp_video, red);
    config_surface(&play_1, title_1.surface->w, title_1.surface->h, title_1.pos.x, play_2.pos.y - 5, bpp_video, black);
    
    Surface load_2, load_1;
    
    config_surface(&load_2, play_2.surface->w, play_2.surface->h, play_2.pos.x, play_1.pos.y + play_1.surface->h, bpp_video, red);
    config_surface(&load_1, play_1.surface->w, play_1.surface->h, play_1.pos.x, play_2.pos.y + play_2.surface->h, bpp_video, black);
    
    Surface quit_2, quit_1;
    
    config_surface(&quit_2, load_2.surface->w, load_2.surface->h, load_2.pos.x, load_1.pos.y + load_1.surface->h, bpp_video, red);
    config_surface(&quit_1, load_1.surface->w, load_1.surface->h, load_1.pos.x, load_2.pos.y + load_2.surface->h, bpp_video, black);
    
    Surface play, load, quit;
    
    config_text(&play, LARGEUR/2, play_2.pos.y, font, "JOUER", font_color);
    config_text(&load, LARGEUR/2, load_2.pos.y, font, "CHARGER", font_color);
    config_text(&quit, LARGEUR/2, quit_2.pos.y, font, "QUITTER", font_color);
    
    //affiche fond dégradé
    Surface background[HAUTEUR];
    background_color(w, background);
    afficher_background(w, background);
    
    SDL_BlitSurface(play_1.surface, NULL, screen, &play_1.pos);
    SDL_BlitSurface(play_2.surface, NULL, screen, &play_2.pos);
    
    SDL_BlitSurface(load_1.surface, NULL, screen, &load_1.pos);
    SDL_BlitSurface(load_2.surface, NULL, screen, &load_2.pos);
    
    SDL_BlitSurface(quit_1.surface, NULL, screen, &quit_1.pos);
    SDL_BlitSurface(quit_2.surface, NULL, screen, &quit_2.pos);
    
    SDL_BlitSurface(play.surface, NULL, screen, &play.pos);
    SDL_BlitSurface(load.surface, NULL, screen, &load.pos);
    SDL_BlitSurface(quit.surface, NULL, screen, &quit.pos);
       
    SDL_BlitSurface(title_1.surface, NULL, screen, &title_1.pos);
    SDL_BlitSurface(title_2.surface, NULL, screen, &title_2.pos);
    SDL_BlitSurface(title.surface, NULL, screen, &title.pos);
    
    pause(w, background, play_2, quit_2);
    
    for(i=0; i<HAUTEUR; ++i)
        SDL_FreeSurface(background[i].surface);
    
    SDL_FreeSurface(play_2.surface);
    SDL_FreeSurface(play_1.surface);
    SDL_FreeSurface(load_2.surface);
    SDL_FreeSurface(load_1.surface);
    SDL_FreeSurface(quit_2.surface);
    SDL_FreeSurface(quit_1.surface);
    
    SDL_FreeSurface(play.surface);
    SDL_FreeSurface(load.surface);
    SDL_FreeSurface(quit.surface);
    
    SDL_FreeSurface(title_2.surface);
    SDL_FreeSurface(title_1.surface);
//     SDL_FreeSurface(plateau.surface);
    SDL_FreeSurface(title.surface);
    SDL_FreeSurface(screen);
    
}