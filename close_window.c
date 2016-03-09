#include <stdlib.h>

#include "close_window.h"

void close_window(Window w){
    
    TTF_CloseFont(get_Font(w));
    TTF_Quit();
    SDL_Quit(); // SDL_FreeSurface(screen);
    
}