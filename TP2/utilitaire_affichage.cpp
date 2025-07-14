/******************************************************************************/
/*  UTILITAIRE_AFFICHAGE.CPP                                                  */
/*  Module utilitaire qui permet d'afficher les cases d'une grille en mode    */
/*  graphique avec le module graphique "winBGim.h".                           */
/*                                                                            */
/*  Conception : Pierre Bélisle, Eric Thé                                     */
/******************************************************************************/
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utilitaire_affichage.h" 

//Nombre de pixels pour un caractere
#define TAILLE_CAR 24
#define TAILLE_STR 40

//marges pour la zone d'affichage des cercles
#define MARGEX  10
#define MARGEY 123
#define MARGEGR 21
#define MAX_HAUT_GR 100
#define COULEUR_DEFAULT  WHITE

//*********************************
//  INIT GRAPHE
//*********************************
//Crée une fenetre de la dimension voulue
void init_graphe(int taille_x, int taille_y) {  
	//la fenetre principale
	initwindow(taille_x, taille_y);   
}

//******************************
//  INIT ZONE ENVIRONNEMENT
//******************************
//Dessine un cadre blanc autour de la zone d'affichage
void init_zone_environnement(int dimx, int dimy) {
  //La fenetre de la grille
  setviewport(MARGEX, MARGEY, (dimx)+MARGEX+1, (dimy)+MARGEY+1, 1);
  setcolor(COULEUR_DEFAULT);
  rectangle(0, 0, (dimx), (dimy));
  //circle(0,0, 5);  //** test
}

//******************************
//  AFFICHER CERCLE
//******************************
//Affiche un cercle à la position fournie
void afficher_cercle(int col, int lig, int couleur) {
	setcolor(couleur);
 	circle(col, lig, RAYON);
}

//******************************
//  AFFICHER MUR
//******************************
//Affiche un mur à la position fournie de la longueur fournie
void afficher_mur(int pos_x, int longueur) {
	setcolor(COULEUR_DEFAULT);
	setlinestyle(0, 1, 3);
	line(pos_x, 0, pos_x, longueur);
	setlinestyle(0, 1, 1);    //retour à default
}

//******************************
//  AFFICHER INFOS
//******************************
//Affiche le temps (nb. de jours) et l'état de la population.
void afficher_infos(int jours, int nb_morts, int nb_malades, int lemax)  {
	char val[8];

	//Établir la zone d'affichage et faire un grand rectangle noir pour EFFACER
	setviewport(0, 0, getmaxx(), MARGEGR-1, 0); 
	setbkcolor(BLACK);
	setfillstyle(SOLID_FILL, BLACK);
	bar(99, 0, 990, MARGEGR-4);        //rectangle noir pour effacer la zone de messages
	settextstyle(TRIPLEX_FONT,0,2);
	setcolor(LIGHTGRAY);

	//afficher le nombre de jours
	outtextxy(120, 0," Jours : ");
	_itoa(jours, val, 10);
	outtextxy(250, 0, val);

	//afficher le nombre de personnes vivantes
	outtextxy(390, 0," Morts : ");
	_itoa(nb_morts, val, 10);
	outtextxy(530, 0, val);

	//afficher le nombre de personnes malades
	outtextxy(620, 0," Malades : ");
	_itoa(nb_malades, val, 10);
	outtextxy(800, 0, val);

	//afficher le nombre de personnes malades maximal
	outtextxy(870, 0," Max : ");
	_itoa(lemax, val, 10);
	outtextxy(980, 0, val);

	setbkcolor(BLACK);
}                         

//******************************
//  AFFICHER GRAPHE
//******************************
//Affiche le graphe qui indique l'état de la population
// ======== MODIFIER POUR AJOUTER UNE REPRESENTATION DES PERSONNE EN QUARANTAINE ========
void afficher_graphe(int pos, double prop_malades_libres, double prop_sante,
	double prop_quarantaine) {

	int posy = MARGEGR + MAX_HAUT_GR,  // MARGEY + HAUTEUR + MAX_HAUT_GR + 5, 
		posx = MARGEX + pos;

	//setviewport(0, 0, getmaxx(), getmaxy(), 0);
	setviewport(0, 0, getmaxx(), posy + 5, 0);

	// Calculer la hauteur totale disponible
	int hauteur_totale = MAX_HAUT_GR;
	int y_current = posy;

	// S'assurer que la somme des proportions ne dépasse pas 1.0
	double somme_proportions = prop_malades_libres + prop_sante + prop_quarantaine;
	if (somme_proportions > 1.0) {
		// Normaliser les proportions
		prop_malades_libres /= somme_proportions;
		prop_sante /= somme_proportions;
		prop_quarantaine /= somme_proportions;
	}
	
	/*	EXEMPLE DE TYPE D'AFFICHAGE:
	morts (bleu)
	sains (vert)
	malades libres (rouge)
	quarantaine (jaune)
	*/
	
	// Quarantaine (jaune)
	if (prop_quarantaine > 0) {
		setcolor(COUL_QUARANTAINE);
		int hauteur_quarantaine = (int)(prop_quarantaine * MAX_HAUT_GR);
		line(posx, y_current, posx, y_current - hauteur_quarantaine);
		y_current -= hauteur_quarantaine;
	}

	// Malades libres (rouge)
	if (prop_malades_libres > 0) {
		setcolor(COUL_MALADE);
		int hauteur_malades = (int)(prop_malades_libres * MAX_HAUT_GR);
		line(posx, y_current, posx, y_current - hauteur_malades);
		y_current -= hauteur_malades;
	}

	// Sains (vert)
	if (prop_sante > 0) {
		setcolor(COUL_SANTE);
		int hauteur_sante = (int)(prop_sante * MAX_HAUT_GR);
		line(posx, y_current, posx, y_current - hauteur_sante);
		y_current -= hauteur_sante;
	}

	// Morts (bleu)
	if (y_current > MARGEGR) {
		setcolor(COUL_MORT);
		line(posx, y_current, posx, MARGEGR);
	}
}
	// CODE ORIGINEL:
/*
//******************************
//  AFFICHER GRAPHE
//******************************
//Affiche le graphe qui indique l'état de la population
void afficher_graphe(int pos, double prop_malades, double prop_sante,
	double prop_quarantaine){

    int posy = MARGEGR + MAX_HAUT_GR,  // MARGEY + HAUTEUR + MAX_HAUT_GR + 5, 
		posx = MARGEX + pos;

	//setviewport(0, 0, getmaxx(), getmaxy(), 0);
	setviewport(0, 0, getmaxx(), posy + 5, 0);
	setcolor(COUL_MALADE);
	line(posx, posy, posx, posy - (int)(prop_malades * 100));

	posy -= (int)(prop_malades*100); 
	setcolor(COUL_SANTE);
	line(posx, posy, posx, posy - (int)(prop_sante * 100));

	posy -= (int)(prop_sante * 100);
	setcolor(COUL_MORT);
	line(posx, posy, posx, MARGEGR);   
}	
*/

//******************************
//  EFFACER ZONE ENVIRONNEMENT
//******************************
//Permet d'effacer la zone d'affichage au complet
void effacer_zone_environnement(int dimx, int dimy) {
   setviewport(MARGEX+1, MARGEY+1, (dimx)+MARGEX, (dimy)+MARGEY, 1);
   clearviewport();
}

//******************************
//  DELAI ÉCRAN
//******************************
//Permet de faire une pause de "msec" millisecondes à l'écran
void delai_ecran(int msec) {
   delay_graph(msec);
}

//******************************
//  TOUCHE PESEE
//******************************
//Retourne 1 si une touche a été pesée, 0 sinon.
int  touche_pesee() {  
   return kbhit_graph();
}

//******************************
//  OBTENIR TOUCHE
//******************************
//Pour récupérer un caractère saisi dans la console graphique.
int  obtenir_touche() {
   return getch_graph();
}

//******************************
//  FERMER MODE GRAPHIQUE
//******************************
void fermer_mode_graphique() {
	closegraph();
}