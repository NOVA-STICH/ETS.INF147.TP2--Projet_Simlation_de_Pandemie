#pragma once
/******************************************************************************/
/*
 Module : liste_personnes.h
 Par    : Maxime Nebili + Guillaum Cadieux
 Date   : 12 Juillet 2025
 Info	: Module qui consiste � controler la liste de personne
*/
/******************************************************************************/
#include "personne.h"
#include "utilitaire_affichage.h"

/******************************************************************************/
/*                      D�CLARATIONS DES CONSTANTES                           */
/******************************************************************************/

#define FACTEUR 1 //facteur multiplicatif du nombre de personnes (1 � 5)
#define MAX_PERS (300 * FACTEUR) //nombre initial de personnes (300, 600, �, 1500)

/******************************************************************************/
/*                         D�CLARATIONS DE TYPES                              */
/******************************************************************************/
typedef struct {
	t_personne* liste; //tableau dynamique des personnes
	int taille; //la taille du tableau
	int nb_pers; //nombre de personnes non-mortes dans la liste
	int nb_malades; //le nombre de malades
	int nb_sante; //le nombre de personnes en sant�
	int nb_morts; //le nombre de morts
} t_liste_personnes;

/******************************************************************************/
/*                         D�CLARATIONS DE FONCTIONS                          */
/******************************************************************************/
/*INT_LISTE_PERSONNES
Re�oit une liste de personne, le nombre de personne, la probabilit� de quarantine et 
les dimmensions de l'espaces.
PARAM�TRES :  * liste_pers - pointeur sur une liste de personne (t_liste_personne).
				nbr_personne - nombre de personne dans la liste (int).
				prob_quar - problabilit� de quarantaine (double).
				hauteur, largeur - dimmensions de l'espace (int).*/
void int_liste_personnes(t_liste_personnes* liste_pers, int nbr_personne,
double prob_quar, int hauteur, int largeur, int mode_mur, const t_mur* mur);

/* VIDER_LISTE_PERSONNE
   Re�oit une liste de personne et la vide et lib�re le tableau dynamique.
   PARAM�TRES : * liste_pers - pointeur sur une liste de personne (t_liste_personne).*/
void vider_liste_personnne(t_liste_personnes* liste_pers);

/* PATIENT_ZERO
   Re�oit une liste de personne et choisis al�atoirement une personne qui devient malade
   de la liste et change son �tat.
   PARAM�TRES : * liste_pers - pointeur sur une liste de personne (t_liste_personne).*/
void patient_zero(t_liste_personnes* liste_pers);

/* PATIENT_MORT
   Re�oit une liste de personne et une position d'une personne qui va mourir. On tue la
   personne et on la place vers aux bas de la liste. 
   PARAM�TRES : * liste_pers - pointeur sur une liste de personne (t_liste_personne).
				pos_mort - la position du mort (int).*/
void patient_mort(t_liste_personnes* liste_pers, int pos_mort);

/* TRAITEMENT
   Re�oit une liste de personne, une probabiliter de quarantaine et les dimmensions de 
   l'espace. On d�place toute les personnes vivantes non en quarantaine et on rajoute 
   les heures de maladies au malades, s'il atteigne la limite on v�rifie s'il meurt
   s'il survivre elle retourne en sant�
   PARAM�TRES : * liste_pers - pointeur sur une liste de personne (t_liste_personne).
				prob_quar - problabilit� de quarantaine (double).
				hauteur, largeur - dimmensions de l'espace (int).*/
int traitemnt(t_liste_personnes* liste_pers, double prob_quar,
	int hauteur, int largeur, int mode_mur, const t_mur* mur);

/* IMPRIMER_PERS
   Re�oit une liste de personne et dessine toutes les personnes � l'�cran
   PARAM�TRES : * liste_pers - pointeur sur une liste de personne (t_liste_personne).*/
void imprimer_pers(t_liste_personnes* liste_pers);

/* VERIF_CONTAC_PERS
   Re�oit une liste de personne, une personne � traiter, la probabiliter de quarantaine 
   et les dimmensions de l'espace. On v�rifie si les personnes de la liste sont en 
   contacts avec le traiter, si oui on s'occupe de savoir si la maladie vas se propager
   ou non et ensuite on les bouges.
   PARAM�TRES : * liste_pers - pointeur sur une liste de personne (t_liste_personne).
				*traiter - pointeur sur une personne (t_personne).
				prob_quar - problabilit� de quarantaine (double).
				hauteur, largeur - dimmensions de l'espace (int).*/
void verif_contac_pers(t_liste_personnes* liste_pers, t_personne* traiter,
double prob_quar, int hauteur, int largeur, int mode_mur, const t_mur* mur);

// calcule et retourne la moyenne du nombre d�infections de TOUTES les personnes re�ues
// et aussi r�cup�re le nombre maximal d�infections ainsi que le nombre total de
// personnes qui n�ont jamais �t� infect�es:
double nb_moyen_inf(const t_liste_personnes* liste_pers, int* max, int* nb_zero_fois);

/****************** FONCTIONS ACCESSEURS AUX CHAMPS ***************************/

/* GET_NB_PERSONNES
   Re�oit une liste de personne et retourne le nombre de personnes vivantes.
   PARAM�TRES : * liste_pers - pointeur sur une liste de personne (t_liste_personne).
   RETOUR : nombre de personnes en vie (int).    */
int get_nb_personnes(const t_liste_personnes* liste_pers);

/* GET_NB_MALADES
   Re�oit une liste de personne et retourne le nombre de personnes malades.
   PARAM�TRES : * liste_pers - pointeur sur une liste de personne (t_liste_personne).
   RETOUR : nombre de personnes en malades (int).    */
int get_nb_malades(const t_liste_personnes* liste_pers);

/* GET_NB_SANTE
   Re�oit une liste de personne et retourne le nombre de personnes en sant�.
   PARAM�TRES : * liste_pers - pointeur sur une liste de personne (t_liste_personne).
   RETOUR : nombre de personnes en sant� (int).    */
int get_nb_sante(const t_liste_personnes* liste_pers);

/* GET_NB_MORTS
   Re�oit une liste de personne et retourne le nombre de personnes mortes.
   PARAM�TRES : * liste_pers - pointeur sur une liste de personne (t_liste_personne).
   RETOUR : nombre de personnes mortes (int).    */
int get_nb_morts(const t_liste_personnes* liste_pers);
