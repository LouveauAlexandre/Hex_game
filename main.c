#include <stdio.h>
#include <stdlib.h>

#include <SDL/SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "display_window.h"
#include "background.h"
#include "display_interface.h"
#include "display_menu.h"
#include "game_option.h"
#include "play.h"

int main(int argc, char* args[]){ 
    
    Window w;
    
    w = init_window();
    init_background(w);
    afficher_interface(w);
    free_background();
    close_window(&w);

    return EXIT_SUCCESS; 
}
