#include <stdio.h>
#include <stdlib.h>

#include "init_window.h"
#include "close_window.h"
#include "affichage.h"

int main(int argc, char* args[]){ 
    
    Window w;
    
    w = init_window();
    afficher_menu(w);
    close_window(w);

    return EXIT_SUCCESS; 
}
