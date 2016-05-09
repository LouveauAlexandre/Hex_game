#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL/SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "display_window.h"
#include "display_interface.h"

struct Surface_S{
    SDL_Surface *surface;
    SDL_Rect pos;
};

Menu Interface[NB_INTERFACE];

/*------------------------------------------------------------------*/

void modif_surface(Surface *s, SDL_Surface *s_elem){
    (*s)->surface = s_elem;
}

/*------------------------------------------------------------------*/

void init_surface(Surface *s){
    *s = (Surface)malloc(sizeof(struct Surface_S));
}

/*------------------------------------------------------------------*/

SDL_Surface* get_surface(Surface s){
    return(s->surface);  
}

/*------------------------------------------------------------------*/

SDL_Rect* get_pos(Surface s){
    return(&s->pos);  
}

/*------------------------------------------------------------------*/

void config_surface(Surface *s, int w, int h, int x, int y, Uint32 color){
    
    (*s)->surface = SDL_CreateRGBSurface(SDL_HWSURFACE, w, h, BPP_VIDEO, 0, 0, 0, 0);
    SDL_FillRect((*s)->surface, NULL, color);
    (*s)->pos.x = x;
    (*s)->pos.y = y;
    
}

/*------------------------------------------------------------------*/

void config_text(Surface *s, int x, int y, TTF_Font *font, char *text, SDL_Color font_color){
    
    (*s)->surface = TTF_RenderText_Blended(font, text, font_color);
    (*s)->pos.x = x;
    (*s)->pos.y = y; 
   
}
    
/*------------------------------------------------------------------*/

bool clic_area(SDL_Rect *area, const int X, const int Y){
    if(X >= area->x && X <= area->x + area->w && Y >=  area->y && Y <= area->y + area->h)
        return true;
    else
        return false;
}

/*------------------------------------------------------------------*/

void main_menu(Window w){

    bool stop = false;
    SDL_Event event;
    SDLKey key_pressed;
    
    int i, clicX, clicY, posX, posY;
    bool afficher_menu_2 = false;
    
    display_background(w);
    display_menu(Interface[TITLE], w);
    
    while(!stop){
        
        display_menu(Interface[MENU_1], w);
        
        SDL_WaitEvent(&event);
        switch(event.type){
            case SDL_QUIT :
                stop = true;
            break;
            
            case SDL_MOUSEMOTION:
                posX = event.motion.x;
                posY = event.motion.y;
                
                for(i=0; i<NB_BOUTON_MENU_1; ++i)
                    modif_menu(&Interface[MENU_1], w, get_color(w, GREEN), i);
                
                if(clic_area(get_pos_menu(Interface[MENU_1], 0), posX, posY))
                    modif_menu(&Interface[MENU_1], w, get_color(w, CYAN), 0);
                
                else if(clic_area(get_pos_menu(Interface[MENU_1], 1), posX, posY))
                    modif_menu(&Interface[MENU_1], w, get_color(w, CYAN), 1);
                
                else if(clic_area(get_pos_menu(Interface[MENU_1], 2), posX, posY))
                    modif_menu(&Interface[MENU_1], w, get_color(w, CYAN), 2);
                
                if(afficher_menu_2){
                        
                    for(i=0; i<NB_BOUTON_MENU_2; ++i)
                        modif_menu(&Interface[MENU_2], w, get_color(w, GREEN), i);
                    
                    if(clic_area(get_pos_menu(Interface[MENU_2], 0), posX, posY))
                        modif_menu(&Interface[MENU_2], w, get_color(w, CYAN), 0);
                        
                    else if(clic_area(get_pos_menu(Interface[MENU_2], 1), posX, posY))
                        modif_menu(&Interface[MENU_2], w, get_color(w, CYAN), 1);
                        
                    else if(clic_area(get_pos_menu(Interface[MENU_2], 2), posX, posY))
                        modif_menu(&Interface[MENU_2], w, get_color(w, RED), 2);
                            
                    display_menu(Interface[MENU_2], w);
                }
                
                SDL_Flip(get_screen(w));
                
            break;
            
            case SDL_MOUSEBUTTONDOWN:
                clicX = event.button.x;
                clicY = event.button.y;
                
                // on récupère les coordonnées du clic
                
                if(clic_area(get_pos_menu(Interface[MENU_1], 0), clicX, clicY)){
                    afficher_menu_2 = !afficher_menu_2;
                    if(!afficher_menu_2){
                        display_background(w);
                        display_menu(Interface[TITLE], w);    
                    }
                    else
                        display_menu(Interface[MENU_2], w);
                }
                
                else if(clic_area(get_pos_menu(Interface[MENU_1], 1), clicX, clicY)){
                    stop = play(w, Interface, true, 0);
                    display_background(w);
                    display_menu(Interface[TITLE], w);
                }
                
                else if(clic_area(get_pos_menu(Interface[MENU_1], 2), clicX, clicY))
                    stop = true;
                
                else if(clic_area(get_pos_menu(Interface[MENU_2], 0), clicX, clicY) && afficher_menu_2){
                    stop = play(w, Interface, false, 0);
                    display_background(w);
                    display_menu(Interface[TITLE], w);
                }
                
                else if(clic_area(get_pos_menu(Interface[MENU_2], 1), clicX, clicY) && afficher_menu_2){
                    stop = play(w, Interface, false, 1);
                    display_background(w);
                    display_menu(Interface[TITLE], w);
                }

            break;
                
            case SDL_KEYDOWN :
                key_pressed = event.key.keysym.sym; // on récupère la touche
                switch (key_pressed){
                    case SDLK_ESCAPE :
                        stop = true;
                    break;
                        
                    default :
                    break;
                }
            break;
        }
        SDL_Flip(get_screen(w)); 
    }
}
    
/*------------------------------------------------------------------*/

void afficher_interface(Window w){
    
    char *text_title[NB_BOUTON_TITLE] = {"JEU DE HEX"};
    char *text_menu_1[NB_BOUTON_MENU_1] = {"JOUER", "CHARGER", "QUITTER"};
    char *text_menu_2[NB_BOUTON_MENU_2] = {"PLAYER vs PLAYER", "PLAYER vs IA 1", "PLAYER vs IA 2"};
    char *text_menu_3[NB_BOUTON_MENU_3] = {"ANNULER", "SAUVEGARDER", "ABANDONNER", "RETOUR"};
    char *text_info[NB_BOUTON_INFO] = {"                                  "};
    
    char *text_hist[NB_BOUTON_HIST];
    
    int i;
    
    for(i=0; i<NB_BOUTON_HIST; ++i){
        text_hist[i] = malloc(sizeof(char)*13);
        strcpy(text_hist[i], "            ");
    }
    
    init_menu(&Interface[TITLE], NB_BOUTON_TITLE, text_title, POS_X_TITLE, POS_Y_TITLE);
    config_menu(&Interface[TITLE], w, get_color(w, GREEN), get_Font(w, 0));
    
    init_menu(&Interface[MENU_1], NB_BOUTON_MENU_1, text_menu_1, POS_X_MENU_1, POS_Y_MENU_1);
    config_menu(&Interface[MENU_1], w, get_color(w, GREEN), get_Font(w, 1));
    
    init_menu(&Interface[MENU_2], NB_BOUTON_MENU_2, text_menu_2, POS_X_MENU_2, POS_Y_MENU_2);
    config_menu(&Interface[MENU_2], w, get_color(w, GREEN), get_Font(w, 1));
    
    init_menu(&Interface[MENU_3], NB_BOUTON_MENU_3, text_menu_3, POS_X_MENU_3, POS_Y_MENU_3);
    config_menu(&Interface[MENU_3], w, get_color(w, GREEN), get_Font(w, 2));
    
    init_menu(&Interface[INFO], NB_BOUTON_INFO, text_info, POS_X_INFO, POS_Y_INFO);
    config_menu(&Interface[INFO], w, get_color(w, GREEN), get_Font(w, 2));
    
    init_menu(&Interface[HIST], NB_BOUTON_HIST, text_hist, POS_X_HIST, POS_Y_HIST);
    config_menu(&Interface[HIST], w, get_color(w, GREEN), get_Font(w, 2));
    
    main_menu(w);
    
    for(i=0; i<NB_INTERFACE; ++i)
        free_menu(&(Interface[i]));
    
    for(i=0; i<NB_BOUTON_HIST; ++i)
        free(text_hist[i]);
    
}