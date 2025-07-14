/******************************************************************************/
/*  UTILITAIRE_AFFICHAGE.H    (module donné aux élèves)                       */
/*  Module utilitaire qui permet d'afficher les cases d'une grille en mode    */
/*  graphique avec le module graphique "winBGim.h".                           */
/*                                                                            */
/*  Conception : Eric Thé                                                     */
/******************************************************************************/

#ifndef utilitaire_affichage_h
#define utilitaire_affichage_h  1

#include "winbgim.h"

//les couleurs d'affichage
#define  COUL_SANTE   GREEN
#define  COUL_MALADE  RED
#define  COUL_MORT    BLUE

// =========== MODIFIER ================
#define  COUL_QUARANTAINE    YELLOW // AJOUTER POUR OBSERVATION, c'est juste cool :0


#define RAYON  5

//Permet de travailler avec une fenêtre graphique de la dimension demandée.
//Reçoit les dimensions de la grille (le nombre de "pixels" désirées).
//NOTE: utilise la fonction "void init_window(int nb_pixels_X, int nb_pixels_Y);"
void init_graphe(int taille_x, int taille_y);

//Permet de dessiner un cadre autour de la zone d'affichage. Reçoit ses dimensions.
void init_zone_environnement(int dimx, int dimy);

//Affiche le temps (nb. de jours) et l'état de la population 
void afficher_infos(int jours, int nb_morts, int nb_malades, int lemax);    

//Affiche le graphe qui indique l'état de la population
// MODIFIER POUR AJOUTER UNE REPRESENTATION DES PERSONNE EN QUARANTAINE:
void afficher_graphe(int pos, double prop_malades, double prop_sante,
	double prop_quarantaine);

//Affiche un cercle à la position fournie de la couleur fournie
void afficher_cercle(int col, int lig, int couleur); 

//Affiche un mur à la position fournie de la longueur fournie
void afficher_mur(int pos_x, int longueur); 

//Permet d'effacer la zone d'affichage au complet
void effacer_zone_environnement(int dimx, int dimy);

//Permet de faire une pause de "msec" millisecondes à l'écran
void delai_ecran(int msec);

//Retourne 1 si une touche a été pesée, 0 sinon.
int  touche_pesee();

//Pour récupérer un caractère saisi dans la console graphique.
int  obtenir_touche();

//Va fermer la fenêtre graphique
void fermer_mode_graphique();

#endif
