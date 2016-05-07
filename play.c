#include <stdlib.h>
#include <math.h>
#include <SDL/SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "display_menu.h"
#include "display_interface.h"
#include "game_option.h"
#include "play.h"

Stack Stk;

/*------------------------------------------------------------------*/

bool empty_stack(Stack S){
    return(S->sentinel->next == S->sentinel);
}

/*------------------------------------------------------------------*/

void init_stack(Stack *S){
    *S = (Stack)malloc(sizeof(struct Stack_S));
    (*S)->sentinel = (Node)malloc(sizeof(struct Node_S));
    (*S)->sentinel->prev = (*S)->sentinel;
    (*S)->sentinel-> next = (*S)->sentinel;
    (*S)->current = (*S)->sentinel;
}
    
/*------------------------------------------------------------------*/

void stack_hexa(Stack *S, Position hexa){
    Node Sp = (Node)malloc(sizeof(struct Node_S));
    Sp->prev = (*S)->current;
    Sp->next = (*S)->sentinel;
    (*S)->current->next = Sp ;
    (*S)->sentinel->prev = Sp;
    Sp->hexa = hexa;
    (*S)->current = Sp;
}

/*------------------------------------------------------------------*/

void unstack_hexa(Stack *S){
    Node Sp = (*S)->current;
    (*S)->current = (*S)->current->prev;
    (*S)->current->next = (*S)->sentinel;
    (*S)->sentinel->prev = (*S)->current;
    free(Sp);
}

/*------------------------------------------------------------------*/

void free_stack(Stack *S){
    while((*S)->current != (*S)->sentinel)
        unstack_hexa(S);
    free((*S)->sentinel);
    free(*S);
}

/*------------------------------------------------------------------*/

void map_stack(Stack S){
    S->current = S->sentinel;
    printf("map_stack\n");
    while((S->current = S->current->prev) != S->sentinel){
        printf("-----\n");
        printf("%d %d\n", S->current->hexa->i, S->current->hexa->j);
    }
    printf("map_stack_end\n");
    printf("\n");
    S->current = S->current->prev;
}

/*------------------------------------------------------------------*/

bool search_hexa(Surface *Pion, int X, int Y, Position grille[NB_LIGNE][NB_COLONNE], char color_player){
    
    float min_length = 20;
    int x = 0, y = 0, i, j;
    float length;
    
    for(i=0; i<NB_LIGNE; ++i){
        for(j=0; j<NB_COLONNE; ++j){
            length = sqrt(pow(X - grille[i][j]->x, 2) + pow(Y - grille[i][j]->y, 2));
                            
            if(length < min_length){
                x = i;
                y = j;
                min_length = length;
            }
        }
    }
    
    if(min_length < 20){
        get_pos(*Pion)->x = grille[x][y]->x - 10;
        get_pos(*Pion)->y = grille[x][y]->y - 10;
        if(grille[x][y]->color == VIDE){
            grille[x][y]->color = color_player;
            stack_hexa(&Stk, grille[x][y]);
//             map_stack(Stk);
            return true;
        }
    }
    
    return false;
}

/*------------------------------------------------------------------*/

bool player_turn(Window w, Surface *pion1, Surface *pion2, int clicX, int clicY, Position grille[NB_LIGNE][NB_COLONNE], bool player){
    
    bool valide;
    
    if(!player){
        if((valide = search_hexa(pion1, clicX, clicY, grille, BLEU))){
            SDL_BlitSurface(get_surface(*pion1), NULL, get_screen(w), get_pos(*pion1));
        }

    }
    else{
        if((valide = search_hexa(pion2, clicX, clicY, grille, ROUGE))){
            SDL_BlitSurface(get_surface(*pion2), NULL, get_screen(w), get_pos(*pion2));
        }
   }
   
   return valide;
}

/*------------------------------------------------------------------*/

void init_grille(Surface plateau, Position grille[NB_LIGNE][NB_COLONNE]){

    int marge_x = get_pos(plateau)->x + 32;
    // coordonnées du centre du premier hexa
    int marge_y = get_pos(plateau)->y + 34;
    
    int taille_x = 30;
    int taille_y = 10;
    
    int i, j, k, p, q;
    
    for(i=0; i<NB_LIGNE; ++i){
        for(j=0; j<NB_COLONNE; ++j){
            grille[i][j] = (Position)malloc(sizeof(struct Position_S));
            grille[i][j]->x = marge_x + j*taille_x;
            grille[i][j]->y = marge_y + i*taille_y;
            grille[i][j]->i = i;
            grille[i][j]->j = j;
            grille[i][j]->color = VIDE;
            grille[i][j]->visited = false;
            
            if((!i && !j) || (i==NB_LIGNE-1 && j==NB_COLONNE-1))
                grille[i][j]->nb_next = 2;
            else if((!i && j==NB_COLONNE-1) || (!j && i==NB_LIGNE-1))
                grille[i][j]->nb_next = 3;
            else if(!i || !j || i==NB_LIGNE-1 || j==NB_COLONNE-1)
                grille[i][j]->nb_next = 4;
            else
                grille[i][j]->nb_next = 6;
        
            grille[i][j]->next = (Position*)malloc(sizeof(Position*)*grille[i][j]->nb_next);
        }
        
        marge_x = marge_x + 15;
        marge_y = marge_y + 15;
    }
    
    for(i=0; i<NB_LIGNE; ++i){
        for(j=0; j<NB_COLONNE; ++j){
            k=0;
            p = i-1;
            while(p<=i+1 && k<grille[i][j]->nb_next){
                if(p>=0 && p<NB_LIGNE){
                    if(p==i-1)
                        q=j;
                    else
                        q=j-1;
                    
                    while(q<=j+1 && k<grille[i][j]->nb_next){
                        if(q>=0 && q<NB_COLONNE){
                            grille[i][j]->next[k] = grille[p][q];
                            ++k;
                        }
                        
                        if(p==i)
                            q=q+2;
                        else
                            ++q;
                    }
                }
                ++p;
            }
        }
    }
    
//     for(i=0; i<NB_LIGNE; ++i){
//         for(j=0; j<NB_COLONNE; ++j){
//             for(k=0; k<grille[i][j]->nb_next; ++k){
//                 printf("%d %d %d %d %d\n", i, j, k, grille[i][j]->next[k]->i, grille[i][j]->next[k]->j);
//             }
//         }
//     }
}

/*------------------------------------------------------------------*/

bool path(Position p, bool const player, char const COLOR_PLAYER){
    
    bool win = false;
    int k;
    
//     printf("%d %d\n", p->i, p->j);
    
    if((p->i == NB_LIGNE-1 && player) || (p->j == NB_COLONNE-1 && !player)){
        p->visited = false;
        return true;
    }
    else
    {
        k = 0;
        while(k < p->nb_next && !win){
            if(p->next[k]->color == COLOR_PLAYER && !(p->next[k]->visited)){
                p->visited = true;
                win = path(p->next[k], player, COLOR_PLAYER);
            }
            ++k;
        }
    }
    
    p->visited = false;
    return win;
    
}

/*------------------------------------------------------------------*/

bool detect_winner(bool player, Position grille[NB_LIGNE][NB_COLONNE]){
    
    int i, j;
    bool win = false;
    
    i = 0;
    j = 0;
    if(!player){
        while(i<NB_LIGNE && !win){
            if(grille[i][j]->color == BLEU)
                win = path(grille[i][j], player, BLEU);
            ++i;
        }
    }
    else
    {
        while(j<NB_COLONNE && !win){
            if(grille[i][j]->color == ROUGE)
                win = path(grille[i][j], player, ROUGE);
            ++j;
        }
    }
    
    return(win);
}

/*------------------------------------------------------------------*/

void free_grille(Position grille[NB_LIGNE][NB_COLONNE]){
    
    int i, j;
    
    for(i=0; i<NB_LIGNE; ++i){
        for(j=0; j<NB_COLONNE; ++j){
            free(grille[i][j]);
        }
    }
}

/*------------------------------------------------------------------*/

void add_hist(Window w, Menu *m, Stack Stk, char* message, bool player){
    
    int k;
    for(k=NB_BOUTON_HIST-1; k>0; --k)
        modif_texte_menu(m, w, get_text_menu(*m, k-1), get_Font(w, 2), k);
        
    sprintf(message, "J%d en %c.%d ", player+1, Stk->current->hexa->j + 'A', Stk->current->hexa->i + 1);
                        
    modif_texte_menu(m, w, message, get_Font(w, 2), 0);
}

/*------------------------------------------------------------------*/

void undo_hist(Window w, Menu *m, Stack Stk, char* message, bool player){
    
    int k;
    int i =0;
    
    Stk->current = Stk->sentinel;
                        
    while((Stk->current = Stk->current->prev) != Stk->sentinel && i <= get_nb_button(*m)){
        ++i;
    }
    
    for(k=0; k<get_nb_button(*m)-1; ++k)
        modif_texte_menu(m, w, get_text_menu(*m, k+1), get_Font(w, 2), k);
    
    if(i <= get_nb_button(*m))
        sprintf(message, "            ");
    else{
        Stk->current = Stk->current->next;
        sprintf(message, "J%d en %c.%d ", player+1, Stk->current->hexa->j + 'A', Stk->current->hexa->i + 1);
    }
                        
    modif_texte_menu(m, w, message, get_Font(w, 2), get_nb_button(*m)-1);
    Stk->current = Stk->sentinel->prev;
}

/*------------------------------------------------------------------*/

void game_actualisation(Window w, Stack Stk, Surface pion1, Surface pion2){
    
    bool player;
    Stk->current = Stk->sentinel;
                        
    while((Stk->current = Stk->current->next) != Stk->sentinel){
                            
//         printf("-----\n");
//         printf("%d %d\n", Stk->current->hexa->i, Stk->current->hexa->j);
                                
        if(Stk->current->hexa->color == BLEU)
            player = false;
        else if(Stk->current->hexa->color == ROUGE)
            player = true;
        
        if(!player){
            get_pos(pion1)->x = Stk->current->hexa->x-10;
            get_pos(pion1)->y = Stk->current->hexa->y-10;
            SDL_BlitSurface(get_surface(pion1), NULL, get_screen(w), get_pos(pion1));
        }
                                
        else if(player){
            get_pos(pion2)->x = Stk->current->hexa->x-10;
            get_pos(pion2)->y = Stk->current->hexa->y-10;
            SDL_BlitSurface(get_surface(pion2), NULL, get_screen(w), get_pos(pion2));
        }
    }
    Stk->current = Stk->sentinel->prev;
}

/*------------------------------------------------------------------*/

void hist_actualisation(Window w, Stack Stk, Menu *m){
    
    char message[MAX_CARACTERE];
    bool player;

    Stk->current = Stk->sentinel;
    
    while((Stk->current = Stk->current->next) != Stk->sentinel){
                            
//         printf("-----\n");
//         printf("%d %d\n", Stk->current->hexa->i, Stk->current->hexa->j);
                                
        if(Stk->current->hexa->color == BLEU)
            player = false;
        else if(Stk->current->hexa->color == ROUGE)
            player = true;
        
        sprintf(message, "J%d en %c.%d ", player+1, Stk->current->hexa->j + 'A', Stk->current->hexa->i + 1);
        add_hist(w, m, Stk, message, player);
    }
    
    Stk->current = Stk->sentinel->prev;
}

/*------------------------------------------------------------------*/

void save_game(Window w, Position grille[NB_LIGNE][NB_COLONNE], Stack Stk, Menu *m){
    
    char message[MAX_CARACTERE];
    
    Sauvegarder(grille, Stk);
    sprintf(message, "Sauvegarde en cours");
    modif_texte_menu(m, w, message, get_Font(w, 2), 0);
    display_menu(*m, w);
    SDL_Flip(get_screen(w)); 
    sprintf(message, "Sauvegarde en cours.");
    modif_texte_menu(m, w, message, get_Font(w, 2), 0);
    SDL_Delay(200);
    display_menu(*m, w);
    SDL_Flip(get_screen(w)); 
    sprintf(message, "Sauvegarde en cours..");
    modif_texte_menu(m, w, message, get_Font(w, 2), 0);
    SDL_Delay(200);
    display_menu(*m, w);
    SDL_Flip(get_screen(w)); 
    sprintf(message, "Sauvegarde en cours...");
    modif_texte_menu(m, w, message, get_Font(w, 2), 0);
    SDL_Delay(200);
    display_menu(*m, w);
    SDL_Flip(get_screen(w));
    SDL_Delay(200);
    sprintf(message, "Sauvegarde terminee.");
    modif_texte_menu(m, w, message, get_Font(w, 2), 0);
}
    
/*------------------------------------------------------------------*/

bool player_abondon(Window w, Menu *m, bool player){
    
    char message[MAX_CARACTERE];
    sprintf(message, "Le joueur %d abandonne la partie !", player+1);
    modif_texte_menu(m, w, message, get_Font(w, 2), 0);
    return(true);
}
                    
/*------------------------------------------------------------------*/

void undo_game(Window w, Position grille[NB_LIGNE][NB_COLONNE], Stack Stk, Menu *m1, Menu *m2, bool player){
    
    char message[MAX_CARACTERE];
    undo_hist(w, m1, Stk, message, player);
    
    sprintf(message, "J%d retire son pion en %c.%d ", player+1, Stk->current->hexa->j + 'A', Stk->current->hexa->i + 1);
    modif_texte_menu(m2, w, message, get_Font(w, 2), 0);
                        
    grille[Stk->current->hexa->i][Stk->current->hexa->j]->color = VIDE;
    unstack_hexa(&Stk);
}

/*------------------------------------------------------------------*/

void not_saved(Window w, Menu *m){
    
    char message[MAX_CARACTERE];
    sprintf(message, "Impossible de sauver une victoire !");
    modif_texte_menu(m, w, message, get_Font(w, 2), 0);
}

/*------------------------------------------------------------------*/

void put_pawn(Window w, Stack Stk, Menu *m1, Menu *m2, bool player){
    
    char message[MAX_CARACTERE];
    add_hist(w, m1, Stk, message, player);
    
    sprintf(message, "J%d pose son pion en %c.%d ", player+1, Stk->current->hexa->j + 'A', Stk->current->hexa->i + 1);
    modif_texte_menu(m2, w, message, get_Font(w, 2), 0);
}
                        
/*------------------------------------------------------------------*/

void winner_game(Window w, Menu *m, bool player){
    
    char message[MAX_CARACTERE];
    sprintf(message, "Le joueur %d a gagne !", player+1);
    modif_texte_menu(m, w, message, get_Font(w, 2), 0);
}
                        
/*------------------------------------------------------------------*/

void strategy_ia_1(Window w, Stack Stk, Position grille[NB_LIGNE][NB_COLONNE], Surface pion2, bool player){
    
    int i, j;
    
    i = Stk->current->hexa->i;
    j = Stk->current->hexa->j;
    
    if(Stk->current->prev == Stk->sentinel){
        if(i<=NB_LIGNE/2)
            i = NB_LIGNE - 2;
        else
            i = 1;
        
        if(j<NB_COLONNE/2)
            j = NB_COLONNE/2 + j;
        else
            j = j - NB_COLONNE/2 + 1;
    }
    
    
    grille[i][j]->color = ROUGE;
    stack_hexa(&Stk, grille[i][j]);
    
    get_pos(pion2)->x = grille[i][j]->x-10;
    get_pos(pion2)->y = grille[i][j]->y-10;
    SDL_BlitSurface(get_surface(pion2), NULL, get_screen(w), get_pos(pion2));
}

/*------------------------------------------------------------------*/

bool play(Window w, Menu Interface[NB_INTERFACE], bool load, int ia){
    
    bool in_game = true;
    bool player = false;
    bool win = false;
    
    Position grille[NB_LIGNE][NB_COLONNE];
    
    SDL_Event event;
    SDLKey key_pressed;
    int i, clicX, clicY, posY, posX;
    
    char message[MAX_CARACTERE];
    
    Surface plateau;
    Surface pion1;
    Surface pion2;
    
    init_surface(&plateau);
    init_surface(&pion1);
    init_surface(&pion2);
    
    modif_surface(&plateau, IMG_Load(CHEMIN_PLATEAU));
    modif_surface(&pion1, IMG_Load(CHEMIN_PION1));
    modif_surface(&pion2, IMG_Load(CHEMIN_PION2));
    
    get_pos(plateau)->x = LARGEUR/2 - get_surface(plateau)->w/2;
    get_pos(plateau)->y = HAUTEUR/2 - get_surface(plateau)->h/2;
    
    display_background(w);
    display_menu(Interface[TITLE], w);
    SDL_BlitSurface(get_surface(plateau), NULL, get_screen(w), get_pos(plateau));
    
    init_grille(plateau, grille);
    init_stack(&Stk);
    
    for(i=0; i<NB_BOUTON_HIST; ++i)
        modif_texte_menu(&Interface[HIST], w, "            ", get_Font(w, 2), i);
    
    if(load){
        Charger(grille, &Stk, &player);
        game_actualisation(w, Stk, pion1, pion2);
        hist_actualisation(w, Stk, &Interface[HIST]);
        SDL_Flip(get_screen(w));
    }
    
    sprintf(message, "Tour de J%d (J1:BLEU / J2:ROUGE)", player+1);
    modif_texte_menu(&Interface[INFO], w, message, get_Font(w, 2), 0);
    
    
    while(in_game){
        
        display_menu(Interface[MENU_3], w);
        display_menu(Interface[INFO], w);
        display_menu(Interface[HIST], w);
        
        SDL_WaitEvent(&event);
        switch(event.type){
            case SDL_QUIT :
                free_grille(grille);
                free_stack(&Stk);
                SDL_FreeSurface(get_surface(plateau));
                SDL_FreeSurface(get_surface(pion1));
                SDL_FreeSurface(get_surface(pion2));
                return(true);
            break;
            
            case SDL_MOUSEMOTION:
                posX = event.motion.x;
                posY = event.motion.y;
                
                if(clic_area(get_pos_menu(Interface[MENU_3], 0), posX, posY) && !empty_stack(Stk) && !win)
                    modif_menu(&Interface[MENU_3], w, get_color(w, CYAN), 0);
                else if(clic_area(get_pos_menu(Interface[MENU_3], 0), posX, posY))
                    modif_menu(&Interface[MENU_3], w, get_color(w, RED), 0);
                else if(clic_area(get_pos_menu(Interface[MENU_3], 1), posX, posY) && !win)
                    modif_menu(&Interface[MENU_3], w, get_color(w, CYAN), 1);
                else if(clic_area(get_pos_menu(Interface[MENU_3], 1), posX, posY))
                    modif_menu(&Interface[MENU_3], w, get_color(w, RED), 1);
                else if(clic_area(get_pos_menu(Interface[MENU_3], 2), posX, posY) && !win)
                    modif_menu(&Interface[MENU_3], w, get_color(w, CYAN), 2);
                else if(clic_area(get_pos_menu(Interface[MENU_3], 2), posX, posY))
                    modif_menu(&Interface[MENU_3], w, get_color(w, RED), 2);
                else if(clic_area(get_pos_menu(Interface[MENU_3], 3), posX, posY))
                    modif_menu(&Interface[MENU_3], w, get_color(w, CYAN), 3);
                else
                    for(i=0; i<NB_BOUTON_MENU_3; ++i)
                        modif_menu(&Interface[MENU_3], w, get_color(w, GREEN), i);
            break;
            
            case SDL_KEYDOWN :
                key_pressed = event.key.keysym.sym; // on récupère la touche
                switch (key_pressed){
                    case SDLK_ESCAPE :
                        in_game = false;
                    break;
                    
                    default :
                    break;
                }
            break;
            
            case SDL_MOUSEBUTTONDOWN:
                clicX = event.button.x;
                clicY = event.button.y;
                
                if(clic_area(get_pos_menu(Interface[MENU_3], 1), clicX, clicY) && !win)
                    save_game(w, grille, Stk, &Interface[INFO]);
                
                else if(clic_area(get_pos_menu(Interface[MENU_3], 1), clicX, clicY) && win)
                    not_saved(w, &Interface[INFO]);
                    
                else if(clic_area(get_pos_menu(Interface[MENU_3], 2), clicX, clicY) && !win)
                   win = player_abondon(w, &Interface[INFO], player);
                
                else if(clic_area(get_pos(plateau), clicX, clicY) && !win){
                    if(player_turn(w, &pion1, &pion2, clicX, clicY, grille, player)){
                        
                        put_pawn(w, Stk, &Interface[HIST], &Interface[INFO], player);
                        
                        if((win = detect_winner(player, grille)))
                            winner_game(w, &Interface[INFO], player);
                            
                        player = !player;
                        
                        if(ia && !win){
                            if(ia == 1){
                                strategy_ia_1(w, Stk, grille, pion2, player);
                                put_pawn(w, Stk, &Interface[HIST], &Interface[INFO], player);
                            }
                            if((win = detect_winner(player, grille)))
                                winner_game(w, &Interface[INFO], player);
                            
                            player = !player;
                        }
                    }
                }
                
                else if(clic_area(get_pos_menu(Interface[MENU_3], 0), clicX, clicY) && !win){
                    
                    if(!empty_stack(Stk)){
                        
                        if(ia)
                            i=2;
                        else{
                            i=1;
                            player = !player;
                        }
                        
                        while(i>0 && !empty_stack(Stk)){
                            undo_game(w, grille, Stk, &Interface[HIST], &Interface[INFO], player);
                            --i;
                        }
                        
                        display_background(w);
                        display_menu(Interface[TITLE], w);
                        SDL_BlitSurface(get_surface(plateau), NULL, get_screen(w), get_pos(plateau));
                        game_actualisation(w, Stk, pion1, pion2);
                    }
                }
                
                else if(clic_area(get_pos_menu(Interface[MENU_3], 3), clicX, clicY))
                    in_game = false;
                    
            break;
            
        }
        SDL_Flip(get_screen(w)); 
    }
    
    free_grille(grille);
    free_stack(&Stk);
    SDL_FreeSurface(get_surface(plateau));
    SDL_FreeSurface(get_surface(pion1));
    SDL_FreeSurface(get_surface(pion2));
    return(false);

}