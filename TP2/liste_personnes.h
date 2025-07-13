#pragma once
/******************************************************************************/
/*
 Module : liste_personnes.h
 Par    : Maxime Nebili + Guillaume Cadieux
 Date   : 12 Juillet 2025
 Info	: Module qui consiste à contrôler la liste de personnes
*/
/******************************************************************************/
#include "personne.h"
#include "utilitaire_affichage.h"

/******************************************************************************/
/*                      DÉCLARATIONS DES CONSTANTES                           */
/******************************************************************************/

#define FACTEUR 1 //facteur multiplicatif du nombre de personnes (1 à 5)

/******************************************************************************/
/*                         DÉCLARATIONS DE TYPES                              */
/******************************************************************************/
typedef struct {
	t_personne* liste; //tableau dynamique des personnes
	int taille; //la taille du tableau
	int nb_pers; //nombre de personnes non-mortes dans la liste
	int nb_malades; //le nombre de malades
	int nb_sante; //le nombre de personnes en bonne santé
	int nb_morts; //le nombre de morts
} t_liste_personnes;

/******************************************************************************/
/*                         DÉCLARATIONS DE FONCTIONS                          */
/******************************************************************************/
/*INIT_LISTE_PERSONNES
Reçoit une liste de personnes, le nombre de personnes, la probabilité de quarantaine et
les dimensions de l'espace.
PARAMÈTRES :  * liste_pers - pointeur sur une liste de personnes (t_liste_personnes).
				nbr_personne - nombre de personnes dans la liste (int).
				prob_quar - probabilité de quarantaine (double).
				hauteur, largeur - dimensions de l'espace (int).*/
void init_liste_personnes(t_liste_personnes* liste_pers, int nbr_personne,
	double prob_quar, int hauteur, int largeur, int mode_mur, const t_mur* mur);

/* VIDER_LISTE_PERSONNE
   Reçoit une liste de personnes et la vide et libère le tableau dynamique.
   PARAMÈTRES : * liste_pers - pointeur sur une liste de personnes (t_liste_personnes).*/
void vider_liste_personnne(t_liste_personnes* liste_pers);

/* PATIENT_ZERO
   Reçoit une liste de personnes et choisit aléatoirement une personne qui devient malade
   dans la liste et change son état.
   PARAMÈTRES : * liste_pers - pointeur sur une liste de personnes (t_liste_personnes).*/
void patient_zero(t_liste_personnes* liste_pers);

/* PATIENT_MORT
   Reçoit une liste de personnes et une position d'une personne qui va mourir. On tue la
   personne et on la place vers le bas de la liste.
   PARAMÈTRES : * liste_pers - pointeur sur une liste de personnes (t_liste_personnes).
				pos_mort - la position du mort (int).*/
void patient_mort(t_liste_personnes* liste_pers, int pos_mort);

/* TRAITEMENT
   Reçoit une liste de personnes, une probabilité de quarantaine et les dimensions de
   l'espace. On déplace toutes les personnes vivantes non en quarantaine et on ajoute
   les heures de maladie aux malades, s'ils atteignent la limite on vérifie s'ils meurent
   ou s'ils survivent et retournent en bonne santé.
   PARAMÈTRES : * liste_pers - pointeur sur une liste de personnes (t_liste_personnes).
				prob_quar - probabilité de quarantaine (double).
				hauteur, largeur - dimensions de l'espace (int).*/
int traitement(t_liste_personnes* liste_pers, double prob_quar,
	int hauteur, int largeur, int mode_mur, const t_mur* mur);

/* IMPRIMER_PERS
   Reçoit une liste de personnes et dessine toutes les personnes à l'écran
   PARAMÈTRES : * liste_pers - pointeur sur une liste de personnes (t_liste_personnes).*/
void imprimer_pers(t_liste_personnes* liste_pers);

/* VERIF_CONTACT_PERS
   Reçoit une liste de personnes, une personne à traiter, la probabilité de quarantaine
   et les dimensions de l'espace. On vérifie si les personnes de la liste sont en
   contact avec celle à traiter, si oui on détermine si la maladie va se propager
   ou non et ensuite on les déplace.
   PARAMÈTRES : * liste_pers - pointeur sur une liste de personnes (t_liste_personnes).
				*traiter - pointeur sur une personne (t_personne).
				prob_quar - probabilité de quarantaine (double).
				hauteur, largeur - dimensions de l'espace (int).*/
void verif_contac_pers(t_liste_personnes* liste_pers, t_personne* traiter,
	double prob_quar, int hauteur, int largeur, int mode_mur, const t_mur* mur);

// Calcule et retourne la moyenne du nombre d'infections de TOUTES les personnes reçues
// et aussi récupère le nombre maximal d'infections ainsi que le nombre total de
// personnes qui n'ont jamais été infectées:
double nb_moyen_inf(const t_liste_personnes* liste_pers, int* max, int* nb_zero_fois);

/****************** FONCTIONS ACCESSEURS AUX CHAMPS ***************************/

/* GET_NB_PERSONNES
   Reçoit une liste de personnes et retourne le nombre de personnes vivantes.
   PARAMÈTRES : * liste_pers - pointeur sur une liste de personnes (t_liste_personnes).
   RETOUR : nombre de personnes en vie (int).    */
int get_nb_personnes(const t_liste_personnes* liste_pers);

/* GET_NB_MALADES
   Reçoit une liste de personnes et retourne le nombre de personnes malades.
   PARAMÈTRES : * liste_pers - pointeur sur une liste de personnes (t_liste_personnes).
   RETOUR : nombre de personnes malades (int).    */
int get_nb_malades(const t_liste_personnes* liste_pers);

/* GET_NB_SANTE
   Reçoit une liste de personnes et retourne le nombre de personnes en bonne santé.
   PARAMÈTRES : * liste_pers - pointeur sur une liste de personnes (t_liste_personnes).
   RETOUR : nombre de personnes en bonne santé (int).    */
int get_nb_sante(const t_liste_personnes* liste_pers);

/* GET_NB_MORTS
   Reçoit une liste de personnes et retourne le nombre de personnes mortes.
   PARAMÈTRES : * liste_pers - pointeur sur une liste de personnes (t_liste_personnes).
   RETOUR : nombre de personnes mortes (int).    */
int get_nb_morts(const t_liste_personnes* liste_pers);
