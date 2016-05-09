#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>

#include "play.h"
#include "game_option.h"

int Sauvegarder(Position grille[NB_LIGNE][NB_COLONNE], Stack S){
	FILE* save;
	if((save = fopen(FICH_SAVE,"w")) == NULL){
		perror("Fichier de sauvegarde");
		return ERR_FICH;
	}
	
	/* Sauvegarde de la grille de jeu */
	fprintf(save,"%s\n%s\n", "\\hex", "\\board");
	for(int i = 0; i < NB_LIGNE; ++i){
		for(int j = 0; j < NB_COLONNE; ++j){
			if(grille[i][j]->color == VIDE)
				fprintf(save,".");
			else if(grille[i][j]->color == ROUGE)
				fprintf(save,"R");
			else if(grille[i][j]->color == BLEU)
				fprintf(save,"B");
			else{
				printf("Erreur lors de la lectuire de la grille");
				return ERR_GRILLE;
			}
			if(j < NB_COLONNE-1)
				fprintf(save," ");
		}
		fprintf(save,"\n");
	}
	
	/* Sauvegarde de la l'historique de jeu */
	fprintf(save,"%s\n%s\n", "\\endboard", "\\game");
	S->current = S->sentinel;
	while((S->current = S->current->next) != S->sentinel)
		if(fprintf(save, "%s %c %i %i\n", "\\play", S->current->hexa->color, S->current->hexa->i, S->current->hexa->j) < 0){
			perror("Erreur Stack : ");
			return ERR_STACK;
		}
	fprintf(save,"%s\n%s", "\\endgame", "\\endhex");
	S->current = S->sentinel->prev;
	fclose(save);
	return EXIT_SUCCESS;
}

int Charger(Position grille[NB_LIGNE][NB_COLONNE], Stack *S, bool *player){
	char verif_save[16];
 	char ligne_grille[11];
	FILE*save;
	int ent;
	char color;
	int pos1, pos2;
	Position hist = (Position) malloc(sizeof(struct Position_S));
	if((save = fopen(FICH_SAVE,"r")) == NULL){
		perror("Fichier de sauvegarde");
		return ERR_FICH;
	}
	
	/* chargement de la grille de jeu */
	fscanf(save,"%s",verif_save);
	if(strcmp(verif_save,"\\hex")){
		printf("Le fichier de sauvegarde n'est pas valide\n");
		return ERR_FICH;
	}
	
	fscanf(save,"%s",verif_save);
	if(strcmp(verif_save,"\\board")){
		printf("Le fichier de sauvegarde n'est pas valide\n");
		return ERR_FICH;
	}
	
	for(int i = 0; i < NB_LIGNE; ++i){
		if((ent = fscanf(save," %c %c %c %c %c %c %c %c %c %c %c", &ligne_grille[0], &ligne_grille[1], &ligne_grille[2], &ligne_grille[3], &ligne_grille[4], &ligne_grille[5], &ligne_grille[6], &ligne_grille[7], &ligne_grille[8], &ligne_grille[9], &ligne_grille[10])) != 11){
			printf("Erreur lors de la lecture de la grille de jeu\n");
			return ERR_GRILLE;
		}
		for(int j = 0; j < NB_COLONNE; ++j){
			if(ligne_grille[j] == '.' || ligne_grille[j] == ROUGE || ligne_grille[j] == BLEU){
				if(ligne_grille[j] == '.' || ligne_grille[j] == 'B' || ligne_grille[j] == 'R')
					if(ligne_grille[j] != '.')
						grille[i][j]->color = ligne_grille[j];
			}
			else{
				printf("Erreur lors de la lecture de la grille de jeu\n");
				return ERR_GRILLE;
			}
		}
	}
	fscanf(save,"%s",verif_save);
	if(strcmp(verif_save,"\\endboard")){
		printf("Le fichier de sauvegarde n'est pas valide\n");
		return ERR_FICH;
	}
	
	/* chargement de l'historique de jeu */
	fscanf(save,"%s",verif_save);
	if(strcmp(verif_save,"\\game")){
		printf("Le fichier de sauvegarde n'est pas valide\n");
		return ERR_FICH;
	}
	
	while((ent = fscanf(save, "%s %c %d %d", verif_save, &color, &pos1, &pos2)) == 4){
		if(strcmp(verif_save,"\\play")){
			printf("Le fichier de sauvegarde n'est pas valide\n");
			 while((*S)->current != (*S)->sentinel)
			unstack_hexa(S);
			return ERR_STACK;
		}
		if(color != 'B' && color != 'R'){
			printf("Le fichier de sauvegarde n'est pas valide\n");
			while((*S)->current != (*S)->sentinel)
			unstack_hexa(S);
			return ERR_STACK;
		}
		(*S)->current = (*S)->sentinel;
		while(((*S)->current = (*S)->current->prev) != (*S)->sentinel)
			if((*S)->current->hexa->i == pos1 && (*S)->current->hexa->j == pos2){
				printf("Le fichier de sauvegarde n'est pas valide\n");
				while((*S)->current != (*S)->sentinel)
				unstack_hexa(S);
				return ERR_STACK;
			}
		(*S)->current = (*S)->sentinel->prev;
		hist = grille[pos1][pos2];
		if(hist->color == color)
			stack_hexa(S,hist, START, START);
		else{
			printf("Le fichier de sauvegarde n'est pas valide\n");
			while((*S)->current != (*S)->sentinel)
			unstack_hexa(S);
			return ERR_STACK;
		}
	}

	fclose(save);
	if((*S)->current->hexa->color == BLEU)
		*player = true;
	else if((*S)->current->hexa->color == ROUGE)
		*player = false;
	
	return EXIT_SUCCESS;
}