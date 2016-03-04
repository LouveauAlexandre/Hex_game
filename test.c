#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <SDL/SDL.h>
// #include <SDL/SDL_keysym.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#define LARGEUR 800
#define HAUTEUR 600


int main(int argc, char* args[]){ 
    
    SDL_Surface *ecran, *rectangle, *rectangle2, *image, *image_pion_1, *titre;
    SDL_Surface *ligne[HAUTEUR];
    Uint32 couleur_1, couleur_2;
    
//     int largeur_fenetre, hauteur_fenetre;
    
    // surface avec du texte
    if( TTF_Init() == -1 ){
        fprintf(stderr, "Erreur d'initialisation de TTF_Init : %s\n", TTF_GetError());
        exit(EXIT_FAILURE);
    }
    
    TTF_Font *police = TTF_OpenFont("Images/FunSized.ttf",60);
//     TTF_SetFontStyle(police, TTF_STYLE_ITALIC );
    
    if(SDL_Init(SDL_INIT_VIDEO) == -1){
         fprintf(stderr, "Erreur d'initialisation de la SDL : %s\n", SDL_GetError());

        exit(EXIT_FAILURE);
    }
    
    SDL_Color policeNoire = {0,0,0};
    titre = TTF_RenderText_Blended(police, "Jeu de Hex", policeNoire);
    
    SDL_Rect posTitre; // SDL_Rect structure dont x et y position du rectangle sur la surface à coller
    posTitre.x = LARGEUR/2 - titre->w/2;
    posTitre.y = 25;
    
    SDL_WM_SetIcon(IMG_Load("Images/icone_Hex.png"), NULL);
    
    ecran = SDL_SetVideoMode(LARGEUR, HAUTEUR, 32, SDL_HWSURFACE | SDL_DOUBLEBUF); // fenetre pour la surface principale
    
    rectangle2 = SDL_CreateRGBSurface(SDL_HWSURFACE, titre->w + 20, titre->h + 10, 32, 0, 0, 0, 0);
    rectangle = SDL_CreateRGBSurface(SDL_HWSURFACE, titre->w + 10, titre->h, 32, 0, 0, 0, 0); // taille du rectangle
    
    couleur_1 = SDL_MapRGB(ecran->format, 255, 0, 0);
    couleur_2 = SDL_MapRGB(ecran->format, 0, 0, 0);
    
    SDL_FillRect(rectangle, NULL, couleur_1);
    SDL_FillRect(rectangle2, NULL, couleur_2);
    
    // position des surfaces secondaires
    SDL_Rect posRect; // SDL_Rect structure dont x et y position du rectangle sur la surface à coller
    posRect.x = posTitre.x - 5;
    posRect.y = posTitre.y;
    
    SDL_Rect posRect2; // SDL_Rect structure dont x et y position du rectangle sur la surface à coller
    posRect2.x = posTitre.x - 10;
    posRect2.y = posTitre.y - 5;
    
    if(ecran == NULL){
        fprintf(stderr, "Impossible de charger le mode vidéo %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    
    int i; // dégradé noir -> blanc
    for(i=0; i<HAUTEUR; ++i)
        ligne[i] = SDL_CreateRGBSurface(SDL_HWSURFACE, LARGEUR, 1, 32, 0, 0, 0, 0);
    
    SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 0, 0, 0));
    
    image = IMG_Load("Images/hex.png");
    SDL_Rect posImg;
    posImg.x = LARGEUR/2 - image->w/2;
    posImg.y = HAUTEUR/2 - image->h/2;
    
    
    image_pion_1 = IMG_Load("Images/button-red22.png");
    SDL_Rect posImgPion1;
    posImgPion1.x = LARGEUR/2 - image->w/2 + image_pion_1->w +1;
    posImgPion1.y = HAUTEUR/2 - image->h/2 + image_pion_1->h +1;

    
    SDL_WM_SetCaption("Test Hex", NULL); // nom de la fenetre
    
    /*------------------------------------------------------*/
       
    bool continuer = true;
    SDL_Event event;
    SDLKey key_pressed;
    
    SDL_EnableKeyRepeat(100, 50);
    
    while(continuer)
    {
        SDL_WaitEvent(&event);
        switch(event.type)
        {
            case SDL_QUIT :
                continuer = false;
                break;
                
            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_LEFT){
                    posImgPion1.x = event.button.x - image_pion_1->w/2 +1;
                    posImgPion1.y = event.button.y - image_pion_1->h/2 +1;
                    printf("X=%d Y=%d\n",event.button.x,event.button.y); // on récupère les coordonnées du clic
                }
                break;
                
            case SDL_MOUSEMOTION: 
                
                SDL_ShowCursor(SDL_DISABLE); // masquer le curseur de la souris
                
                posImgPion1.x = event.button.x - image_pion_1->w/2 +1;
                posImgPion1.y = event.button.y - image_pion_1->h/2 +1;
                break;
                
            case SDL_KEYDOWN :
                key_pressed = event.key.keysym.sym; // on récupère la touche
                switch (key_pressed)
                {
                    case SDLK_ESCAPE :
                        continuer = false;
                        break;
                        
                    case SDLK_UP : 
                        posImgPion1.y = posImgPion1.y - 30;
                        break;
                        
                    case SDLK_DOWN : 
                        posImgPion1.y = posImgPion1.y + 30;
                        break;
                        
                    case SDLK_RIGHT : 
                        posImgPion1.x = posImgPion1.x + 30;
                        break;
                        
                    case SDLK_LEFT : 
                        posImgPion1.x = posImgPion1.x - 30;
                        break;
                        
                    default :
                        printf("%d %c\n", key_pressed, key_pressed);
                        break;
                }
                
                break;
                
//                 case SDL_VIDEORESIZE :
//                     
//                         largeur_fenetre = event.resize.w;
//                         hauteur_fenetre = event.resize.h;
//                         
//                         posImg.x = largeur_fenetre/2 - image->w/2;
//                         posImg.y = hauteur_fenetre/2 - image->h/2;
//                         
//                     break;
                
                default :
                    SDL_ShowCursor(SDL_ENABLE);
                    break;
        }
        
        for(i=0; i<HAUTEUR; ++i){
            //valeurs entre 0 et 255
            int borne_min = 100;
            int borne_max = 220;
            float coef = (HAUTEUR*borne_min)/(256-borne_min);
            int couleur = ((float)(i+coef))/(HAUTEUR+coef)*(borne_max+1);
            SDL_Rect posLigne;
            posLigne.x = 0;
            posLigne.y = i;
            SDL_FillRect(ligne[i], NULL, SDL_MapRGB(ecran->format, 0, couleur, 0));
            SDL_BlitSurface(ligne[i], NULL, ecran, &posLigne);
        }
        
        SDL_BlitSurface(rectangle2, NULL, ecran, &posRect2);
        SDL_BlitSurface(rectangle, NULL, ecran, &posRect);
        SDL_BlitSurface(titre, NULL, ecran, &posTitre);
        SDL_BlitSurface(image, NULL, ecran, &posImg);
        SDL_BlitSurface(image_pion_1, NULL, ecran, &posImgPion1);
        SDL_Flip(ecran); // maj de l'écran
        
    }
    
    /*------------------------------------------------------*/
    
    SDL_FreeSurface(rectangle2);
    SDL_FreeSurface(rectangle);
    SDL_FreeSurface(image);
    
    for(i=0; i<HAUTEUR; ++i)
        SDL_FreeSurface(ligne[i]);
    
    SDL_FreeSurface(titre);
    
    TTF_CloseFont(police);
    TTF_Quit();
    SDL_Quit(); // SDL_FreeSurface(Ecran);

    return EXIT_SUCCESS; 
}
