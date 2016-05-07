#include <stdlib.h>

#include <SDL/SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "display_interface.h"
#include "background.h"

Surface background[HAUTEUR];

/*------------------------------------------------------------------*/

void init_background(Window w){

    int i;
    for(i=0; i<HAUTEUR; ++i){
        float coef = (HAUTEUR*COLOR_MIN)/(COLOR-COLOR_MIN);
        int couleur = ((float)(i+coef))/(HAUTEUR+coef)*(COLOR_MAX+1);
        init_surface(&background[i]);
        config_surface(&background[i], LARGEUR, 1, 0, i, SDL_MapRGB(get_screen(w)->format, couleur, couleur, couleur));
    }

}

/*------------------------------------------------------------------*/

void display_background(Window w){
 
    int i;
    for(i=0; i<HAUTEUR; ++i)
        SDL_BlitSurface(get_surface(background[i]), NULL, get_screen(w), get_pos(background[i]));    
    
}

/*------------------------------------------------------------------*/


void free_background(){
    
    int i;
    for(i=0; i<HAUTEUR; ++i)
        SDL_FreeSurface(get_surface(background[i]));    
    
}