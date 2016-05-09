#include <stdlib.h>
#include <math.h>
#include <SDL/SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "display_menu.h"
#include "display_interface.h"
#include "game_option.h"
#include "play.h"

struct Bot_IA_S{
	int sens;
	int dir;
	Stack pos;
	int bloc;
	int demi;
	int startx;
	int starty;
};

 Stack Stk;
 Bot_IA bot;
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

void stack_hexa(Stack *S, Position hexa, int dir, int sens){
    Node Sp = (Node)malloc(sizeof(struct Node_S));
    Sp->prev = (*S)->current;
    Sp->next = (*S)->sentinel;
    (*S)->current->next = Sp ;
    (*S)->sentinel->prev = Sp;
    Sp->hexa = hexa;
    Sp->dir = dir;
    Sp->sens = sens;
    (*S)->current = Sp;
}

/*------------------------------------------------------------------*/

void unstack_hexa(Stack *S){
    if(!empty_stack(*S)){
	Node Sp = (*S)->current;
	(*S)->current = (*S)->current->prev;
	(*S)->current->next = (*S)->sentinel;
	(*S)->sentinel->prev = (*S)->current;
	free(Sp);
    }
}

/*------------------------------------------------------------------*/

void free_stack(Stack *S){
    while((*S)->current != (*S)->sentinel)
        unstack_hexa(S);
    free((*S)->sentinel);
    free(*S);
}

/*------------------------------------------------------------------*/

void free_ia(Bot_IA *bot){
	free_stack(&(*bot)->pos);
	free(*bot);
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
            stack_hexa(&Stk, grille[x][y], START, START);
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
            free(grille[i][j]->next);
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

void init_IA(Bot_IA *bot){
	*bot = (Bot_IA) malloc(sizeof(struct Bot_IA_S));
	(*bot)->sens = START;
	(*bot)->bloc = 0;
	(*bot)->demi = false;
	init_stack(&((*bot)->pos));
}
/*------------------------------------------------------------------*/

int verif_occup(Node test, int i, int j, int *dir, int *sens){
	if(test->sens == HAUT){
		if(test->dir == START){
			if(test->hexa->i+1 == i && test->hexa->j-1 == j){
				*sens = HAUT;
				*dir = START;
				return 1;
			}
			else if(test->hexa->i+1 == i && test->hexa->j == j){
				*sens = HAUT;
				*dir = START;
				return 2;
			}
		}
		else if(test->dir == GAUCHE){
			if(test->hexa->i == i && test->hexa->j+1 == j){
				*sens = HAUT;
				*dir = GAUCHE;
				return 1;
			}
			else if(test->hexa->i+1 == i && test->hexa->j == j){
				*sens = HAUT;
				*dir = GAUCHE;
				return 2;
			}
		}
		else if(test->dir == DROITE){
			if(test->hexa->i == i && test->hexa->j-1 == j){
				*sens = HAUT;
				*dir = DROITE;
				return 1;
			}
			else if(test->hexa->i+1 == i && test->hexa->j-1 == j){
				*sens = HAUT;
				*dir = DROITE;
				return 2;
			}
		}
	}
	else if(test->sens == BAS){
		if(test->dir == START){
			if(test->hexa->i-1 == i && test->hexa->j == j){
				*sens = BAS;
				*dir = START;
				return 1;
			}
			else if(test->hexa->i-1 == i && test->hexa->j+1 == j){
				*sens = BAS;
				*dir = START;
				return 2;
			}
		}
		else if(test->dir == GAUCHE){
			if(test->hexa->i == i && test->hexa->j+1 == j){
				*sens = BAS;
				*dir = GAUCHE;
				return 1;
			}
			else if(test->hexa->i-1 == i && test->hexa->j+1 == j){
				*sens = BAS;
				*dir = GAUCHE;
				return 2;
			}
		}
		else if(test->dir == DROITE){
			if(test->hexa->i == i && test->hexa->j-1 == j){
				*sens = BAS;
				*dir = DROITE;
				return 1;
			}
			else if(test->hexa->i-1 == i && test->hexa->j == j){
				*sens = BAS;
				*dir = DROITE;
				return 2;
			}
		}
	}
	return START;
}

/*------------------------------------------------------------------*/

void deter_hexa(int *i, int *j, int *dir, int *sens, int hexa){
	if(*sens == HAUT){
		if(*dir == START){
			if(hexa == 1){
				++*j;
			}
			else if(hexa == 2){
				--*j;
			}
		}
		else if(*dir == GAUCHE){
			if(hexa == 1){
				++*i;
				--*j;
			}
			else if(hexa == 2){
				--*i;
				++*j;
			}
		}
		else if(*dir == DROITE){
			if(hexa == 1){
				++*i;
			}
			else if(hexa == 2){
				--*i;
			}
		}
	}
	else if(*sens == BAS){
		if(*dir == START){
			if(hexa == 1){
				++*j;
			}
			else if(hexa == 2){
				--*j;
			}
		}
		else if(*dir == GAUCHE){
			if(hexa == 1){
				--*i;
			}
			else if(hexa == 2){
				++*i;
			}
		}
		else if(*dir == DROITE){
			if(hexa == 1){
				--*i;
				++*j;
			}
			else if(hexa == 2){
				++*i;
				--*j;
			}
		}
	}
}

/*------------------------------------------------------------------*/

bool valide_hexa(int i, int j, Position grille[NB_LIGNE][NB_COLONNE]){
    
	if(i < 0 || i > NB_LIGNE-1 || j < 0 || j > NB_COLONNE-1)
		return false;
    
	if(grille[i][j]->color != VIDE)
		return false;
    
	return true;
}

/*------------------------------------------------------------------*/

bool ami_rempli(int i, int j, Position grille[NB_LIGNE][NB_COLONNE]){
    
	if(i < 0 || i > NB_LIGNE-1 || j < 0 || j > NB_COLONNE-1)
		return false;
    
	if(grille[i][j]->color != ROUGE)
		return false;
    
	return true;
}

/*------------------------------------------------------------------*/

bool deplacer_haut_gauche(int *i, int *j, Position grille[NB_LIGNE][NB_COLONNE], int *dir, int *sens){
	
	int x, y;
	
	x = *i;
	y = *j-1;
	
	if(valide_hexa(x, y, grille)){
		x = *i-1;
		y = *j;
		if(valide_hexa(x, y, grille)){
			x = *i-1;
			y = *j-1;
			if(valide_hexa(x, y, grille)){
				*i = x;
				*j = y;
				*sens = HAUT;
				*dir = GAUCHE;
				return true;
			}
		}
	}
	return false;
}

/*------------------------------------------------------------------*/

bool deplacer_haut_milieu(int *i, int *j, Position grille[NB_LIGNE][NB_COLONNE], int *dir, int *sens){
    
	int x, y;
	
	x = *i-1;
	y = *j;
	
	if(valide_hexa(x, y, grille)){
		x = *i-1;
		y = *j+1;
		if(valide_hexa(x, y, grille)){
			x = *i-2;
			y = *j+1;
			if(valide_hexa(x, y, grille)){
				*i = x;
				*j = y;
				*sens = HAUT;
				*dir = START;
				return true;
			}
		}
	}
	return false;
}

/*------------------------------------------------------------------*/

bool deplacer_haut_droite(int *i, int *j, Position grille[NB_LIGNE][NB_COLONNE], int *dir, int *sens){
    
	int x, y;
	
	x = *i;
	y = *j+1;
	
	if(valide_hexa(x, y, grille)){
		x = *i-1;
		y = *j+1;
		if(valide_hexa(x, y, grille)){
			x = *i-1;
			y = *j+2;
			if(valide_hexa(x, y, grille)){
				*i = x;
				*j = y;
				*sens = HAUT;
				*dir = DROITE;
				return true;
			}
		}
	}
	return false;
}

/*------------------------------------------------------------------*/

bool deplacer_bas_gauche(int *i, int *j, Position grille[NB_LIGNE][NB_COLONNE], int *dir, int *sens){
    
	int x, y;
	
	x = *i;
	y = *j-1;
	
	if(valide_hexa(x, y, grille)){
		x = *i+1;
		y = *j-1;
		if(valide_hexa(x, y, grille)){
			x = *i+1;
			y = *j-2;
			if(valide_hexa(x, y, grille)){
				*i = x;
				*j = y;
				*sens = BAS;
				*dir = GAUCHE;
				return true;
			}
		}
	}
	return false;
}

/*------------------------------------------------------------------*/

bool deplacer_bas_milieu(int *i, int *j, Position grille[NB_LIGNE][NB_COLONNE], int *dir, int *sens){
  
	int x, y;
	x = *i+1;
	y = *j-1;
	
	if(valide_hexa(x, y, grille)){
		x = *i+1;
		y = *j;
		if(valide_hexa(x, y, grille)){
			x = *i+2;
			y = *j-1;
			if(valide_hexa(x, y, grille)){
				*i = x;
				*j = y;
				*sens = BAS;
				*dir = START;
				return true;
			}
		}
	}
	return false;
}

/*------------------------------------------------------------------*/

bool deplacer_bas_droite(int *i, int *j, Position grille[NB_LIGNE][NB_COLONNE], int *dir, int *sens){
    
	int x, y;
	
	x = *i;
	y = *j+1;
	
	if(valide_hexa(x, y, grille)){
		x = *i+1;
		y = *j;
		if(valide_hexa(x, y, grille)){
			x = *i+1;
			y = *j+1;
			if(valide_hexa(x, y, grille)){
				*i = x;
				*j = y;
				*sens = BAS;
				*dir = DROITE;
				return true;
			}
		}
	}
	return false;
}

/*------------------------------------------------------------------*/

bool remplir_haut_gauche(int *i, int *j, Position grille[NB_LIGNE][NB_COLONNE], int *dir, int *sens){

	int x, y;
	
	x = *i;
	y = *j+1;
	
	if(!ami_rempli(x, y, grille) && !ami_rempli(x+1, y-1, grille)){
		if(valide_hexa(x, y, grille)){
			*i = x;
			*j = y;
			return true;
		}
		else if(valide_hexa(x+1, y-1, grille)){
			*i = x+1;
			*j = y-1;
			return true;
		}
	}
	return false;
}

/*------------------------------------------------------------------*/

bool remplir_haut_milieu(int *i, int *j, Position grille[NB_LIGNE][NB_COLONNE], int *dir, int *sens){

	int x, y;
	
	x = *i+1;
	y = *j;
	
	if(!ami_rempli(x, y, grille) && !ami_rempli(x, y-1, grille)){
		if(valide_hexa(x, y, grille)){
			*i = x;
			*j = y;
			return true;
		}
		else if(valide_hexa(x, y-1, grille)){
			*i = x;
			*j = y-1;
			return true;
		}
	}
	return false;
}

/*------------------------------------------------------------------*/

bool remplir_haut_droite(int *i, int *j, Position grille[NB_LIGNE][NB_COLONNE], int *dir, int *sens){

	int x, y;
	
	x = *i;
	y = *j-1;
	
	if(!ami_rempli(x, y, grille) && !ami_rempli(x+1, y, grille)){
		if(valide_hexa(x, y, grille)){
			*i = x;
			*j = y;
			return true;
		}
		else if(valide_hexa(x+1, y, grille)){
			*i = x+1;
			*j = y;
			return true;
		}
	}
	return false;
}

/*------------------------------------------------------------------*/

bool remplir_bas_gauche(int *i, int *j, Position grille[NB_LIGNE][NB_COLONNE], int *dir, int *sens){

	int x, y;
	
	x = *i;
	y = *j+1;
	
	if(!ami_rempli(x, y, grille) && !ami_rempli(x-1, y, grille)){
		if(valide_hexa(x, y, grille)){
			*i = x;
			*j = y;
			return true;
		}
		else if(valide_hexa(x-1, y, grille)){
			*i = x-1;
			*j = y;
			return true;
		}
	}
	return false;
}

/*------------------------------------------------------------------*/

bool remplir_bas_milieu(int *i, int *j, Position grille[NB_LIGNE][NB_COLONNE], int *dir, int *sens){

	int x, y;
	
	x = *i-1;
	y = *j;
	
	if(!ami_rempli(x, y, grille) && !ami_rempli(x, y+1, grille)){
		if(valide_hexa(x, y, grille)){
			*i = x;
			*j = y;
			return true;
		}
		else if(valide_hexa(x, y+1, grille)){
			*i = x;
			*j = y+1;
			return true;
		}
	}
	return false;
}

/*------------------------------------------------------------------*/

bool remplir_bas_droite(int *i, int *j, Position grille[NB_LIGNE][NB_COLONNE], int *dir, int *sens){

	int x, y;
	
	x = *i;
	y = *j-1;
	
	if(!ami_rempli(x, y, grille) && !ami_rempli(x-1, y+1, grille)){
		if(valide_hexa(x, y, grille)){
			*i = x;
			*j = y;
			return true;
		}
		else if(valide_hexa(x-1, y+1, grille)){
			*i = x-1;
			*j = y+1;
			return true;
		}
	}
	return false;
}

/*------------------------------------------------------------------*/

bool valide_rempli(Node x, Position grille[NB_LIGNE][NB_COLONNE]){
	if(x->sens == HAUT){
		if(x->dir == START){
			if(ami_rempli(x->hexa->i+2, x->hexa->j-1, grille))
				return true;
			else
				return false;
		}
		else if(x->dir == GAUCHE){
			if(ami_rempli(x->hexa->i+1, x->hexa->j+1, grille))
				return true;
			else
				return false;
		}
		else if(x->dir == DROITE){
			if(ami_rempli(x->hexa->i+1, x->hexa->j-2, grille))
				return true;
			else
				return false;
		}
	}
	else if(x->sens == BAS){
		if(x->dir == START){
			if(ami_rempli(x->hexa->i-2, x->hexa->j+1, grille))
				return true;
			else
				return false;
		}
		if(x->dir == GAUCHE){
			if(ami_rempli(x->hexa->i-1, x->hexa->j+2, grille))
				return true;
			else
				return false;
		}
		if(x->dir == DROITE){
			if(ami_rempli(x->hexa->i-1, x->hexa->j-1, grille))
				return true;
			else
				return false;
		}
	}
	return false;
}

/*------------------------------------------------------------------*/

bool remplissage_hexa(int *i, int *j, Bot_IA bot, Position grille[NB_LIGNE][NB_COLONNE], int *dir, int *sens){
	bool rempli;
	bot->pos->current = bot->pos->sentinel;
	for(int k = 0; k < bot->bloc; ++k)
		bot->pos->current = bot->pos->current->prev;
	while((bot->pos->current = bot->pos->current->prev) != bot->pos->sentinel){
		rempli = false;
		if(bot->pos->current->sens != START){
			if(bot->pos->current->hexa->i-1 == 0){
				if(ami_rempli(bot->pos->current->hexa->i-1, bot->pos->current->hexa->j+1, grille) || ami_rempli(bot->pos->current->hexa->i-1, bot->pos->current->hexa->j, grille))
					rempli = true;
				if(!rempli){
					  if(valide_hexa(bot->pos->current->hexa->i-1, bot->pos->current->hexa->j, grille)){
						  *i = bot->pos->current->hexa->i-1;
						  *j = bot->pos->current->hexa->j;
						  return true;
					  }
					  else if(valide_hexa(bot->pos->current->hexa->i-1, bot->pos->current->hexa->j+1, grille)){
						  *i = bot->pos->current->hexa->i-1;
						  *j = bot->pos->current->hexa->j+1;
						  return true;
					  }
					  else if(valide_hexa(bot->pos->current->hexa->i, bot->pos->current->hexa->j-1, grille)){
						  *i = bot->pos->current->hexa->i;
						  *j = bot->pos->current->hexa->j-1;
						  *dir = START;
						  *sens = HAUT;
						  return true;
					  }
					  else if(valide_hexa(bot->pos->current->hexa->i, bot->pos->current->hexa->j+1, grille)){
						  *i = bot->pos->current->hexa->i;
						  *j = bot->pos->current->hexa->j+1;
						  *dir = START;
						  *sens = HAUT;
						  return true;
					  }
				}
			}
			else if(bot->pos->current->hexa->i+1 == NB_LIGNE-1){
				if(ami_rempli(bot->pos->current->hexa->i+1, bot->pos->current->hexa->j-1, grille) || ami_rempli(bot->pos->current->hexa->i+1, bot->pos->current->hexa->j, grille))
					rempli = true;
				if(!rempli){
					if(valide_hexa(bot->pos->current->hexa->i+1, bot->pos->current->hexa->j, grille)){
						*i = bot->pos->current->hexa->i+1;
						*j = bot->pos->current->hexa->j;
						return true;
					}
					else if(valide_hexa(bot->pos->current->hexa->i+1, bot->pos->current->hexa->j-1, grille)){
						*i = bot->pos->current->hexa->i+1;
						*j = bot->pos->current->hexa->j-1;
						return true;
					}
					else if(valide_hexa(bot->pos->current->hexa->i, bot->pos->current->hexa->j-1, grille)){
						*i = bot->pos->current->hexa->i;
						*j = bot->pos->current->hexa->j-1;
						*dir = START;
						*sens = BAS;
						return true;
					}
					else if(valide_hexa(bot->pos->current->hexa->i, bot->pos->current->hexa->j+1, grille)){
						*i = bot->pos->current->hexa->i;
						*j = bot->pos->current->hexa->j+1;
						*dir = START;
						*sens = BAS;
						return true;
					}
				}
			}
		}
		*i = bot->pos->current->hexa->i;
		*j = bot->pos->current->hexa->j;
		if(valide_rempli(bot->pos->current, grille)){
			if(bot->pos->current->sens == HAUT){
				if(bot->pos->current->dir == START){
					if(remplir_haut_milieu(i, j, grille, dir, sens))
						return true;
				}
				else if(bot->pos->current->dir == GAUCHE){
					if(remplir_haut_gauche(i, j, grille, dir, sens))
						return true;
				}
				else if(bot->pos->current->dir == DROITE){
					if(remplir_haut_droite(i, j, grille, dir, sens))
						return true;
				}
			}
			else if(bot->pos->current->sens == BAS){
				if(bot->pos->current->dir == START){
					if(remplir_bas_milieu(i, j, grille, dir, sens))
						return true;
				}
				if(bot->pos->current->dir == GAUCHE){
					if(remplir_bas_gauche(i, j, grille, dir, sens))
						return true;
				}
				if(bot->pos->current->dir == DROITE){
					if(remplir_bas_droite(i, j, grille, dir, sens))
						return true;
				}
			}
		}
	}
	printf("\n\n");
	return false;
} 
  
/*------------------------------------------------------------------*/

void strategy_ia_1(Window w, Stack *Stk, Position grille[NB_LIGNE][NB_COLONNE], Surface pion2, Bot_IA *bot){
    
	int i, j;
	float k;
	int dir, sens, hexa;
	bool play = false;

	i = (*Stk)->current->hexa->i;
	j = (*Stk)->current->hexa->j;

	if((*bot)->sens == START){
		j = ((i*j)/2)%NB_COLONNE;
		k = ((float)(i*j)/2.f);
		while(k>=11) k-=NB_COLONNE;
		if((float)j != k && j < 10)
			j = j+1;

		if(j == 0) ++j;
		else if(j == 10) --j;
		
		if(i <= NB_LIGNE/2){
			i = 9;
			(*bot)->sens = HAUT;
			if(j <= NB_COLONNE/3)
				(*bot)->dir = DROITE;
			else
				(*bot)->dir = GAUCHE;
		}
		else{
			i = 1;
			(*bot)->sens = BAS;
			if(j >= (NB_COLONNE/3)*2)
				(*bot)->dir = GAUCHE;
			else
				(*bot)->dir = DROITE;
		}
		(*bot)->startx = i;
		(*bot)->starty = j;
		dir = START;
		sens = (*bot)->sens;
	}
	else{
		sens = START;
		dir = START;
		hexa = START;
		Node current = (*bot)->pos->sentinel;
		while(((current = current->prev) != (*bot)->pos->sentinel) && sens == START)
			hexa = verif_occup(current, i, j, &dir, &sens);
		if(sens != START){
			deter_hexa(&i, &j, &dir, &sens, hexa);
			if(grille[i][j]->color != VIDE)
				hexa = START;
			if(hexa != START) ++(*bot)->bloc;
		}
		if(hexa == START){
			for(int k = 0; k < (*bot)->bloc; ++k)
				(*bot)->pos->current = (*bot)->pos->current->prev;
			i = (*bot)->pos->current->hexa->i;
			j = (*bot)->pos->current->hexa->j;
			
			if(((*bot)->sens == HAUT && i <= 1) || ((*bot)->sens == BAS && i >= 9))
				(*bot)->demi = 1;
			
			if((*bot)->demi == 0){
				while(!play && (*bot)->pos->current != (*bot)->pos->sentinel){
					if((*bot)->sens == HAUT)
						play = deplacer_haut_milieu(&i, &j, grille, &dir, &sens);
					else
						play = deplacer_bas_milieu(&i, &j, grille, &dir, &sens);
					
					if(!play){
						if((*bot)->sens == HAUT){
							if((*bot)->dir == GAUCHE)
								play = deplacer_haut_gauche(&i, &j, grille, &dir, &sens);
							else
								play = deplacer_haut_droite(&i, &j, grille, &dir, &sens);
						}
						else{
							if((*bot)->dir == GAUCHE)
								play = deplacer_bas_gauche(&i, &j, grille, &dir, &sens);
							else
								play = deplacer_bas_droite(&i, &j, grille, &dir, &sens);
						}
					}

					if(!play){
						if((*bot)->sens == HAUT){
							if((*bot)->dir == GAUCHE)
								play = deplacer_haut_droite(&i, &j, grille, &dir, &sens);
							else
								play = deplacer_haut_gauche(&i, &j, grille, &dir, &sens);
						}
						else{
							if((*bot)->dir == GAUCHE)
								play = deplacer_bas_droite(&i, &j, grille, &dir, &sens);
							
							else
								play = deplacer_bas_gauche(&i, &j, grille, &dir, &sens);
						}
					}
					
					if(!play){
						if((*bot)->sens == HAUT){
							if((*bot)->dir == GAUCHE)
								play = deplacer_bas_gauche(&i, &j, grille, &dir, &sens);
							else
								play = deplacer_bas_droite(&i, &j, grille, &dir, &sens);
						}
						else{
							if((*bot)->dir == GAUCHE)
								play = deplacer_haut_gauche(&i, &j, grille, &dir, &sens);
							else
								play = deplacer_haut_droite(&i, &j, grille, &dir, &sens);
						}
					}
					
					if(!play){
						if((*bot)->sens == HAUT){
							if((*bot)->dir == GAUCHE)
								play = deplacer_bas_droite(&i, &j, grille, &dir, &sens);
							else
								play = deplacer_bas_gauche(&i, &j, grille, &dir, &sens);
						}
						else{
							if((*bot)->dir == GAUCHE)
								play = deplacer_haut_droite(&i, &j, grille, &dir, &sens);
							else
								play = deplacer_haut_gauche(&i, &j, grille, &dir, &sens);
						}
					}
					
					if(!play){
						if((*bot)->sens == HAUT)
							play = deplacer_bas_milieu(&i, &j, grille, &dir, &sens);
						else
							play = deplacer_haut_milieu(&i, &j, grille, &dir, &sens);
					}
					if(!play){
						(*bot)->pos->current = (*bot)->pos->current->prev;
						i = (*bot)->pos->current->hexa->i;
						j = (*bot)->pos->current->hexa->j;
						dir = START;
						sens = START;
					}
					else
						(*bot)->pos->current = (*bot)->pos->sentinel->prev;
					
				}
				if(!play){
					i = (*bot)->pos->sentinel->next->hexa->i;
					j = (*bot)->pos->sentinel->next->hexa->j;
					i = NB_LIGNE-1-i;
					j = NB_COLONNE-1-j;
					if((*bot)->sens == HAUT)
						(*bot)->sens = BAS;
					else
						(*bot)->sens = HAUT;
					
					if((*bot)->dir == GAUCHE)
						(*bot)->dir = DROITE;
					else
						(*bot)->dir = GAUCHE;
					
					(*bot)->startx = i;
					(*bot)->starty = j;
					dir = START;
					sens = (*bot)->sens;
					(*bot)->pos->current = (*bot)->pos->sentinel->prev;
					if(!valide_hexa(i, j, grille)){
						i = 0;
						while(j < NB_COLONNE){
							i = 0;
							while(i < NB_LIGNE){
								if(valide_hexa(i, j, grille))
									break;
								++i;
							}
							if(valide_hexa(i, j, grille))
								break;
							++j;
						}
						if( i > (*bot)->pos->current->hexa->i)
							sens = BAS;
						else
							sens = HAUT;
						if(sens == HAUT){
							if(j == (*bot)->pos->current->hexa->j || j == (*bot)->pos->current->hexa->j+1)
								dir = START;
							else if(j < (*bot)->pos->current->hexa->j)
								  dir = GAUCHE;
							else
								  dir = DROITE;
						}
						else{
							if(j == (*bot)->pos->current->hexa->j || j == (*bot)->pos->current->hexa->j-1)
								dir = START;
							else if(j < (*bot)->pos->current->hexa->j-1)
								  dir = GAUCHE;
							else
								  dir = DROITE;
						}
					}
				}
				(*bot)->bloc = 0;
			}
			else{
				play = remplissage_hexa(&i, &j, *bot, grille, &dir, &sens);
				(*bot)->pos->current = (*bot)->pos->sentinel->prev;
				(*bot)->bloc = 0;
				if(!play){
					i = 0;
					while(j < NB_COLONNE){
						i = 0;
						while(i < NB_LIGNE){
							if(valide_hexa(i, j, grille))
								break;
							++i;
						}
						if(valide_hexa(i, j, grille))
							break;
						++j;
					}
 					if( i > (*bot)->pos->current->hexa->i)
						sens = BAS;
					else
						sens = HAUT;
					if(sens == HAUT){
						if(j == (*bot)->pos->current->hexa->j || j == (*bot)->pos->current->hexa->j+1)
							dir = START;
						else if(j < (*bot)->pos->current->hexa->j)
							  dir = GAUCHE;
						else
							  dir = DROITE;
					}
					else{
						if(j == (*bot)->pos->current->hexa->j || j == (*bot)->pos->current->hexa->j-1)
							dir = START;
						else if(j < (*bot)->pos->current->hexa->j-1)
							  dir = GAUCHE;
						else
							  dir = DROITE;
					}
				}
				++(*bot)->demi;
			}
		}
	}
	grille[i][j]->color = ROUGE;
	stack_hexa(&((*bot)->pos), grille[i][j], dir, sens);
	stack_hexa(Stk, grille[i][j], START, START);
	
	get_pos(pion2)->x = grille[i][j]->x-10;
	get_pos(pion2)->y = grille[i][j]->y-10;
	SDL_BlitSurface(get_surface(pion2), NULL, get_screen(w), get_pos(pion2));
}

/*------------------------------------------------------------------*/

bool play(Window w, Menu Interface[NB_INTERFACE], bool load, int ia){
    
    bool in_game = true;
    bool player = false;
    bool win = false;
    int charge;
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
    
      if(ia == 1)
	    init_IA(&bot);
      
    if(load){
        charge = Charger(grille, &Stk, &player);
	if(charge == 0){
	    game_actualisation(w, Stk, pion1, pion2);
	    hist_actualisation(w, Stk, &Interface[HIST]);
	    SDL_Flip(get_screen(w));
	}
	else{
		sprintf(message, "Chargement impossible !");
		modif_texte_menu(&Interface[INFO], w, message, get_Font(w, 2), 0);
		display_menu(Interface[MENU_3], w);
		display_menu(Interface[INFO], w);
		display_menu(Interface[HIST], w);
		SDL_Flip(get_screen(w));
		SDL_Delay(2000);
	}
    
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
                if(ia == 1)
                    free_ia(&bot);
                
                SDL_FreeSurface(get_surface(plateau));
                SDL_FreeSurface(get_surface(pion1));
                SDL_FreeSurface(get_surface(pion2));
                free(plateau);
                free(pion1);
                free(pion2);
                return(true);
            break;
            
            case SDL_MOUSEMOTION:
                posX = event.motion.x;
                posY = event.motion.y;
                
                for(i=0; i<NB_BOUTON_MENU_3; ++i)
                    modif_menu(&Interface[MENU_3], w, get_color(w, GREEN), i);
                
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
                                strategy_ia_1(w, &Stk, grille, pion2, &bot);
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
                        if(ia){
				if( bot->pos->current != bot->pos->sentinel->next && bot->pos->current->hexa->i == bot->startx && bot->pos->current->hexa->j == bot->starty){
					bot->startx = bot->pos->sentinel->next->hexa->i;
					bot->starty = bot->pos->sentinel->next->hexa->j;
					if(bot->sens == HAUT)
						bot->sens = BAS;
					else
						bot->sens = HAUT;
					if(bot->dir == GAUCHE)
						bot->dir = DROITE;
					else
						bot->dir = GAUCHE;
				}
				if(bot->demi > 0) --bot->demi;
				unstack_hexa(&(bot->pos));
				if(empty_stack(bot->pos))
					bot->sens = START;
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
    if(ia == 1)
	free_ia(&bot);
    
    SDL_FreeSurface(get_surface(plateau));
    SDL_FreeSurface(get_surface(pion1));
    SDL_FreeSurface(get_surface(pion2));
    free(plateau);
    free(pion1);
    free(pion2);
    
    return(false);

}