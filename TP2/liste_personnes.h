#pragma once
/******************************************************************************/
/*
 Module : liste_personnes.h
 Par    : Maxime Nebili + Guillaume Cadieux
 Date   : 12 Juillet 2025
 Info	: Module qui consiste � contr�ler la liste de personnes
*/
/******************************************************************************/
#include "personne.h"
#include "utilitaire_affichage.h"

/******************************************************************************/
/*                      D�CLARATIONS DES CONSTANTES                           */
/******************************************************************************/

#define FACTEUR 1 //facteur multiplicatif du nombre de personnes (1 � 5)

/******************************************************************************/
/*                         D�CLARATIONS DE TYPES                              */
/******************************************************************************/
typedef struct {
	t_personne* liste; //tableau dynamique des personnes
	int taille; //la taille du tableau
	int nb_pers; //nombre de personnes non-mortes dans la liste
	int nb_malades; //le nombre de malades
	int nb_sante; //le nombre de personnes en bonne sant�
	int nb_morts; //le nombre de morts
} t_liste_personnes;

/******************************************************************************/
/*                         D�CLARATIONS DE FONCTIONS                          */
/******************************************************************************/
/*INIT_LISTE_PERSONNES
Re�oit une liste de personnes, le nombre de personnes, la probabilit� de quarantaine et
les dimensions de l'espace.
PARAM�TRES :  * liste_pers - pointeur sur une liste de personnes (t_liste_personnes).
				nbr_personne - nombre de personnes dans la liste (int).
				prob_quar - probabilit� de quarantaine (double).
				hauteur, largeur - dimensions de l'espace (int).*/
void init_liste_personnes(t_liste_personnes* liste_pers, int nbr_personne,
	double prob_quar, int hauteur, int largeur, int mode_mur, const t_mur* mur);

/* VIDER_LISTE_PERSONNE
   Re�oit une liste de personnes et la vide et lib�re le tableau dynamique.
   PARAM�TRES : * liste_pers - pointeur sur une liste de personnes (t_liste_personnes).*/
void vider_liste_personnne(t_liste_personnes* liste_pers);

/* PATIENT_ZERO
   Re�oit une liste de personnes et choisit al�atoirement une personne qui devient malade
   dans la liste et change son �tat.
   PARAM�TRES : * liste_pers - pointeur sur une liste de personnes (t_liste_personnes).*/
void patient_zero(t_liste_personnes* liste_pers);

/* PATIENT_MORT
   Re�oit une liste de personnes et une position d'une personne qui va mourir. On tue la
   personne et on la place vers le bas de la liste.
   PARAM�TRES : * liste_pers - pointeur sur une liste de personnes (t_liste_personnes).
				pos_mort - la position du mort (int).*/
void patient_mort(t_liste_personnes* liste_pers, int pos_mort);

/* TRAITEMENT
   Re�oit une liste de personnes, une probabilit� de quarantaine et les dimensions de
   l'espace. On d�place toutes les personnes vivantes non en quarantaine et on ajoute
   les heures de maladie aux malades, s'ils atteignent la limite on v�rifie s'ils meurent
   ou s'ils survivent et retournent en bonne sant�.
   PARAM�TRES : * liste_pers - pointeur sur une liste de personnes (t_liste_personnes).
				prob_quar - probabilit� de quarantaine (double).
				hauteur, largeur - dimensions de l'espace (int).*/
int traitement(t_liste_personnes* liste_pers, double prob_quar,
	int hauteur, int largeur, int mode_mur, const t_mur* mur);

/* IMPRIMER_PERS
   Re�oit une liste de personnes et dessine toutes les personnes � l'�cran
   PARAM�TRES : * liste_pers - pointeur sur une liste de personnes (t_liste_personnes).*/
void imprimer_pers(t_liste_personnes* liste_pers);

/* VERIF_CONTACT_PERS
   Re�oit une liste de personnes, une personne � traiter, la probabilit� de quarantaine
   et les dimensions de l'espace. On v�rifie si les personnes de la liste sont en
   contact avec celle � traiter, si oui on d�termine si la maladie va se propager
   ou non et ensuite on les d�place.
   PARAM�TRES : * liste_pers - pointeur sur une liste de personnes (t_liste_personnes).
				*traiter - pointeur sur une personne (t_personne).
				prob_quar - probabilit� de quarantaine (double).
				hauteur, largeur - dimensions de l'espace (int).*/
void verif_contac_pers(t_liste_personnes* liste_pers, t_personne* traiter,
	double prob_quar, int hauteur, int largeur, int mode_mur, const t_mur* mur);

// Calcule et retourne la moyenne du nombre d'infections de TOUTES les personnes re�ues
// et aussi r�cup�re le nombre maximal d'infections ainsi que le nombre total de
// personnes qui n'ont jamais �t� infect�es:
double nb_moyen_inf(const t_liste_personnes* liste_pers, int* max, int* nb_zero_fois);

/****************** FONCTIONS ACCESSEURS AUX CHAMPS ***************************/

/* GET_NB_PERSONNES
   Re�oit une liste de personnes et retourne le nombre de personnes vivantes.
   PARAM�TRES : * liste_pers - pointeur sur une liste de personnes (t_liste_personnes).
   RETOUR : nombre de personnes en vie (int).    */
int get_nb_personnes(const t_liste_personnes* liste_pers);

/* GET_NB_MALADES
   Re�oit une liste de personnes et retourne le nombre de personnes malades.
   PARAM�TRES : * liste_pers - pointeur sur une liste de personnes (t_liste_personnes).
   RETOUR : nombre de personnes malades (int).    */
int get_nb_malades(const t_liste_personnes* liste_pers);

/* GET_NB_SANTE
   Re�oit une liste de personnes et retourne le nombre de personnes en bonne sant�.
   PARAM�TRES : * liste_pers - pointeur sur une liste de personnes (t_liste_personnes).
   RETOUR : nombre de personnes en bonne sant� (int).    */
int get_nb_sante(const t_liste_personnes* liste_pers);

/* GET_NB_MORTS
   Re�oit une liste de personnes et retourne le nombre de personnes mortes.
   PARAM�TRES : * liste_pers - pointeur sur une liste de personnes (t_liste_personnes).
   RETOUR : nombre de personnes mortes (int).    */
int get_nb_morts(const t_liste_personnes* liste_pers);
