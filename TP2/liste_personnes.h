#pragma once
/*===========================================================*/
/* 	INF147 - TP2 */
/*===========================================================*/
/*
 Module : liste_personnes.h
 Par    : Maxime Nebili + Guillaum Cadieux
 Date   : 10 Juillet 2025
 Info	: Module qui consiste à controler la liste de personne
*/
/*=========================================================*/


/******************************************************************************/
#include "personne.h"
#include "utilitaire_affichage.h"

/******************************************************************************/
/*                      DÉCLARATIONS DES CONSTANTES                           */
/******************************************************************************/
#define FACTEUR 1 //facteur multiplicatif du nombre de personnes (1 à 5)
#define MAX_PERS (300 * FACTEUR) //nombre initial de personnes (300, 600, …, 1500)

/******************************************************************************/
/*                         DÉCLARATIONS DE TYPES                              */
/******************************************************************************/
typedef struct {
	t_personne* liste; //tableau statique des personnes
	int taille; //la taille du tableau
	int nb_pers; //nombre de personnes non-mortes dans la liste
	int nb_malades; //le nombre de malades
	int nb_sante; //le nombre de personnes en santé
	int nb_morts; //le nombre de morts
} t_liste_personnes;

/******************************************************************************/
/*                         DÉCLARATIONS DE FONCTIONS                          */
/******************************************************************************/

void int_liste_personnes(t_liste_personnes* liste_pers, int nbr_personne, double prob_quar,
	int hauteur, int largeur);

void vider_liste_personnne(t_liste_personnes* liste_pers);

void patient_zero(t_liste_personnes* liste_pers);

void patient_mort(t_liste_personnes* liste_pers, int pos_mort);

int traitemnt(t_liste_personnes* liste_pers, double prob_quar,
	int hauteur, int largeur);
void imprimer_pers(t_liste_personnes* liste_pers);