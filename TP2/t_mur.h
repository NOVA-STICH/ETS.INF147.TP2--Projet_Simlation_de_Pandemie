#pragma once
/******************************************************************************/
/* t_mur.H																	  */
/* NOMS: Maxime Nebili + Guillaum Cadieux									  */
/* Date   : 12 Juillet 2025													  */
/* Librairie de gestion du mur.												  */
/******************************************************************************/
#include <math.h>
#include"utilitaire_affichage.h"
// Directive qui permet de d�sactiver certains warnings de Visual-Studio
#define _CRT_SECURE_NO_WARNINGS

#define NB_JOURS_MAX 60 //nombre de jours avant l'ouverture du mur
#define DIST_MUR (3 * RAYON) //si on est � moins de 3*5 pixels = contact avec le mur

typedef struct {
	int pos_mur;	// position en X du mur
	int longueur;	// la longueur en Y
	int nb_jours;	// nb. de jours depuis l'instauration du mur de confinement
} t_mur;

//=======================================================================================
// init_mur(..) : Cette fonction re�oit la position du mur ainsi que sa longueur.
// Elle d�clare et initialise une variable de type � t_mur � avec les deux valeurs
// re�ues et retourne ce nouveau mur ainsi cr�e.
t_mur init_mur(int pos_mur, int longueur);

//=======================================================================================
// contact_mur(..) : Re�oit un mur par r�f�rence(t_mur*) et la position - X
// d�une personne(un r�el). La fonction v�rifie si cette position est � moins de DIST_MUR
// pixels(distance absolue) et retourne 1 si c�est le cas, 0 sinon.
bool contact_mur(const t_mur *mur, double pos);

//=======================================================================================
// mur_actif(..) : Re�oit un mur par r�f�rence(t_mur*) et incr�mente le nombre de jours
// de confinement de ce mur.Si ce nombre atteint NB_JOURS_MAX la fonction retourne 0
// (plus de mur). Sinon elle retourne 1 (le mur est toujours actif).
bool mur_actif(t_mur *mur);

//=======================================================================================
// dessiner_mur(..) : Re�oit un mur par r�f�rence(t_mur*) et dessine le mur � l'�cran
// (pour le mode AVEC affichages).Utilisez ici la fonction 
// � afficher_mur() � du module � utilitaires_affichage �.
void dessiner_mur(const t_mur *mur);


