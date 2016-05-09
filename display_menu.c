#include <stdlib.h>
#include <stdbool.h>
#include <SDL/SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "display_menu.h"
#include "display_interface.h"
#include "play.h"

struct Menu_S{
    Surface *menu;
    Surface size_reference;
    char **text;
    int nb_bouton;
    int pos_x;
    int pos_y;
};

/*------------------------------------------------------------------*/

void init_menu(Menu *m, int nb_bouton, char **text, int x, int y){
    
    int i;
    
    *m = (Menu)malloc(sizeof(struct Menu_S));
    
    (*m)->nb_bouton = nb_bouton;
    (*m)->menu = malloc(sizeof(Surface)*nb_bouton*3);
    
    (*m)->text = malloc(sizeof(char*)*nb_bouton);
    
    for(i=0; i<nb_bouton; ++i){
        (*m)->text[i] = malloc(sizeof(char)*MAX_CARACTERE);
        strcpy((*m)->text[i], text[i]);
    }
    
    (*m)->pos_x = x;
    (*m)->pos_y = y;
}

/*------------------------------------------------------------------*/

void free_menu(Menu *m){
    
    int i;
    for(i=0; i<(*m)->nb_bouton*3; ++i)
        SDL_FreeSurface(get_surface((*m)->menu[i]));
        
    free((*m)->menu);
    
    for(i=0; i<(*m)->nb_bouton; ++i)
        free((*m)->text[i]);
    
    free((*m)->text);
    
    SDL_FreeSurface(get_surface((*m)->size_reference));
    free((*m)->size_reference);
    free(*m);
}

/*------------------------------------------------------------------*/

void config_menu(Menu *m, Window w, Uint32 color, TTF_Font *font){
    
    SDL_Color font_color = get_font_color(w);
    
    int longueur = 0;
    int longueur_texte;
    int i;
    
    int k = 0;
    for(i=0; i<(*m)->nb_bouton; ++i){
        longueur_texte = strlen((*m)->text[i]);
        if(longueur < longueur_texte){
            longueur = strlen((*m)->text[i]);
            k = i;
        }
    }
    
    init_surface(&((*m)->size_reference));
    config_text(&((*m)->size_reference), 0, 0, font, (*m)->text[k], font_color);
    
    //k étant l'indice de la case correspondant à la plus large
    
    //Surface size_reference permet de d'adapter la taille des cases entre-elles en fonction de la plus large
    
    init_surface(&((*m)->menu[0]));
    config_text(&((*m)->menu[0]), (*m)->pos_x, (*m)->pos_y, font, (*m)->text[0], font_color);
    
    for(i=1; i<(*m)->nb_bouton*3; ++i){
        init_surface(&((*m)->menu[i]));
        if(i%3 == 1){
            config_surface(&((*m)->menu[i]), get_surface((*m)->size_reference)->w + 10, get_surface((*m)->menu[0])->h, get_pos((*m)->menu[i-1])->x - 5, get_pos((*m)->menu[i-1])->y, color);
        }
        else if(i%3 == 2){
            config_surface(&((*m)->menu[i]), get_surface((*m)->menu[i-1])->w + 10, get_surface((*m)->menu[i-1])->h + 10, get_pos((*m)->menu[i-1])->x - 5, get_pos((*m)->menu[i-1])->y - 5, get_color(w, BLACK));
        }
        else{
            config_text(&((*m)->menu[i]), get_pos((*m)->menu[i-3])->x, get_pos((*m)->menu[i-3])->y + get_surface((*m)->menu[i-2])->h + 5, font, (*m)->text[i/3], font_color);
        }
    }
}

/*------------------------------------------------------------------*/

void modif_menu(Menu *m, Window w, Uint32 color, int i){
    
    i = i*3+1;
    
    SDL_FreeSurface(get_surface((*m)->menu[i]));
    
    config_surface(&((*m)->menu[i]), get_surface((*m)->size_reference)->w + 10, get_surface((*m)->menu[0])->h, get_pos((*m)->menu[i-1])->x - 5, get_pos((*m)->menu[i-1])->y, color);
    
}

/*------------------------------------------------------------------*/

void modif_texte_menu(Menu *m, Window w, char *text, TTF_Font *font, int i){
    
    i = i*3;
    
    SDL_FreeSurface(get_surface((*m)->menu[i]));
    free((*m)->menu[i]);
    init_surface(&((*m)->menu[i]));
    strcpy((*m)->text[i/3], text);
    
    if(!i){
        config_text(&((*m)->menu[0]), (*m)->pos_x, (*m)->pos_y, font, text, get_font_color(w));
    }
    else{
        config_text(&((*m)->menu[i]), get_pos((*m)->menu[i-3])->x, get_pos((*m)->menu[i-3])->y + get_surface((*m)->menu[i-2])->h + 5, font, text, get_font_color(w));
    }
}

/*------------------------------------------------------------------*/

void display_menu(Menu m, Window w){
    int i;
    for(i=(m->nb_bouton*3)-1; i>=0; --i){
        SDL_BlitSurface(get_surface(m->menu[i]), NULL, get_screen(w), get_pos(m->menu[i]));
    }
}

/*------------------------------------------------------------------*/

SDL_Rect* get_pos_menu(Menu m, int i){
    return(get_pos(m->menu[((i+1)*3)-2])); 
}

/*------------------------------------------------------------------*/

bool clic_menu(Menu m, const int X, const int Y){
    if(X >= get_pos(m->menu[2])->x && X <= get_pos(m->menu[2])->x + get_surface(m->menu[2])->w && Y >= get_pos(m->menu[2])->y && Y <= get_pos(m->menu[2])->y + get_surface(m->menu[2])->h * m->nb_bouton)
        return true;
    else
        return false;
}

/*------------------------------------------------------------------*/

char* get_text_menu(Menu m, int i){
    return(m->text[i]);
}

/*------------------------------------------------------------------*/

int get_nb_button(Menu m){
    return(m->nb_bouton);
}