#pragma once
/******************************************************************************/
/* Module:  personne.h                                                        */
/* NOMS:    Maxime Nebili + Guillaume Cadieux                                 */
/* Date:    13 Juillet 2025                                                   */
/* Info:    Librairie de gestion des personnes.                               */
/******************************************************************************/
// Directive qui permet de d�sactiver certains warnings de Visual-Studio
#define _CRT_SECURE_NO_WARNINGS

// Librairies usuelles � inclure
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "alea_pop.h"    //pour les fonctions al�atoires
#include "t_mur.h"       //pour valider contact avec le mur (mode avec mur, SEM. 3)  


/******************************************************************************/
/*                      D�CLARATIONS DES CONSTANTES                           */
/******************************************************************************/
#define PROB_CONTAGION   0.3    //probabilit� de contracter le virus
#define NB_HRS_QUAR     15*24   //nombre d'heures total en quarantaine (ou malade)
#define RAYON_1m          5     //rayon de 1 m�tre (en pixels) = rayon d'affichage des cercles

/* constantes relatives aux probabilit�s de d�c�s selon l'�ge */
#define AGE_JEUNE        29     //limite de la premi�re tranche d'�ge
#define INC_CAT_AGE      10     //�cart entre chaque tranche d'�ge
#define VITESSE          30      // Mutiplicateur de vitesse pour aller plus vite   :)

/* prob. de d�c�s selon l'�ge [0-29, 30-39, 40-49, 50-59, 60-69, 70-79, >=80] */
const double PROB_DECES[7] = { 0.001, 0.001, 0.004, 0.017, 0.059, 0.182, 0.736 };

/* Source des statistiques :
https://www.inspq.qc.ca/covid-19/donnees?fbclid=IwAR0WOWnq-kyUn_75t-zOXvQmnlNBwyo7MHI57M5k1EILJor4FwmIL51ZDUI
*/

// MACRO pour le calcul de distance entre 2 points: 
#define DISTANCE_2P(x1,y1,x2,y2) (sqrt(((x1)-(x2))*((x1)-(x2)) + ((y1)-(y2))*((y1)-(y2))))

/******************************************************************************/
/*                         D�CLARATIONS DE TYPES                              */
/******************************************************************************/
typedef enum { MORT, EN_SANTE, MALADE } t_etat;     //les 3 �tats possibles

/* Type-structure pour une personne */
typedef struct {
    t_etat  etat;            //son �tat de sant� (type enum)
    double  posx, posy;      //sa position
    double  vitx, vity;      //sa vitesse & direction
    int     age;             //son �ge (en ann�es)
    int     quarantaine;     //est-il en quarantaine?
    int     hrs_malade;      //nombre d'heures en quarantaine (ou malade)
    double  prob_inf;        //probabilit� d'infection, va r�duire selon le nb. fois
    int     nb_inf;          //le nombre d'infections
} t_personne;


/******************************************************************************/
/*                         D�CLARATIONS DE FONCTIONS                          */
/******************************************************************************/

/* INIT_PERSONNE
   Fonction qui va initialiser et retourner une nouvelle personne.
   PARAM�TRES : px, py - position initiale de la personne (double).
                prob_quar - la probabilit� que cette personne soit en quarantaine(double)
   RETOUR : une nouvelle personne (t_personne).    */
t_personne init_personne(double px, double py, double prob_quar);

/****************** FONCTIONS ACCESSEURS AUX CHAMPS ***************************/

/* GET_POS_PERSONNE
   Re�oit une personne et retourne sa position via (*px, *py).
   PARAM�TRES : *lui - pointeur sur une personne (t_personne).
                *px, *py - param�tres de retour pour r�cup�rer la position (double *).  */
void get_pos_personne(const t_personne* lui, double* px, double* py);

/* GET_ETAT
   Re�oit une personne et retourne son �tat actuel.
   PARAM�TRES : *lui - pointeur sur une personne (t_personne).
   RETOUR : l'�tat de la personne (t_etat).    */
t_etat get_etat(const t_personne* lui);

/* GET_PROB_INF
   Re�oit une personne et retourne sa probabilit� de contagion.
   PARAM�TRES : *lui - pointeur sur une personne (t_personne).
   RETOUR : la probabilit� d'infection de la personne (double).   */
double get_prob_inf(const t_personne* lui);

/* GET_QUARANTAINE
   Re�oit une personne et retourne son �tat de quarantaine.
   PARAM�TRES : *lui - pointeur sur une personne (t_personne).
   RETOUR : l'�tat de quarantaine de la personne (bool�en, 0 ou 1).   */
int get_quarantaine(const t_personne* lui);

/* GET_HRS_MALADE
   Re�oit une personne et retourne le nombre d'heures qu'il a �t� malade.
   PARAM�TRES : *lui - pointeur sur une personne (t_personne).
   RETOUR : nombre d'heures de maladie de la personne (int).    */
int get_hrs_malade(const t_personne* lui);

/* DETERMINER_MORT
   Re�oit une personne et d�termine s'il va mourir selon son �ge et son
   nombre d'infections.  Un r�el al�atoire g�n�r� est compar� avec:
     randf() < (PROB_DECES[..case selon l'�ge..] / (nb_infections + 1))
   PARAM�TRES : *lui - pointeur sur une personne (t_personne).
   RETOUR : 1 si cette personne est destin�e � mourir, 0 sinon (bool�en, 0 ou 1).   */
int determiner_mort(const t_personne* lui);

/* CONTACT_PERSONNES
   Re�oit une personne et une position. D�termine si cette personne
   entre en contact avec la position re�ue.
   PARAM�TRES : *lui - pointeur sur une personne (t_personne).
                px, py - la position � v�rifier (double).
   RETOUR : 1 si cette personne est situ�e � la position [py,px], 0 sinon (bool�en, 0 ou 1). */
int contact_personnes(const t_personne* lui, double px, double py);


/****************** FONCTIONS MUTATEURS DES CHAMPS ****************************/

/* DEPLACER_PERSONNE
   Re�oit une personne et la d�place selon sa position. Suite au d�placement on
   v�rifie si on doit la faire "rebondir" sur une des bordures.
   On re�oit aussi le "mode_mur" et le mur pour valider une collision avec le mur.
   PARAM�TRES : *lui - pointeur sur la personne � modifier (t_personne).
                largeur, hauteur - les dimensions de l'espace de simulation (int).
                mode_mur - �tat du mode avec ou sans mur de confinement (bool�en, 0 ou 1).
                mur - le mur de confinement (t_mur *).   */
void deplacer_personne(t_personne* lui, int largeur, int hauteur,
    int mode_mur, const t_mur* mur);    //  (SEM. 3)

/* INVERSER_DIR_PERS
   Re�oit deux personnes et les d�place dans deux directions oppos�es suite
   � un contact.
   PARAM�TRES : *lui, *autre - pointeurs sur les 2 personnes (t_personne *).  */
void inverser_dir_pers(t_personne* lui, t_personne* autre);

/* MODIFIER_ETAT_PERS
   Re�oit une personne, un nouvel �tat et un mode de quarantaine.
   Modifie l'�tat de la personne ainsi que son �tat de quarantaine.
   Si le nouvel �tat re�u est "EN_SANTE", on doit diviser sa probabilit�
   d'infection par trois (3) et incr�menter son nombre d'infections.
   On va �galement remettre son nombre d'heures en quarantaine � 0.
   PARAM�TRES : *lui - pointeur sur la personne � modifier (t_personne).
                etat - nouvel �tat � attribuer � la personne (t_etat).
                mode_quar - le mode de quarantaine de la personne (bool�en, 0 ou 1). */
void modifier_etat_pers(t_personne* lui, t_etat etat, int mode_quar);

/* AJOUTER_HR_QUAR
   Re�oit une personne et incr�mente (+1) son nombre d'heures de quarantaine.
   PARAM�TRES : *lui - pointeur sur la personne � modifier (t_personne).      */
void ajouter_hr_quar(t_personne* lui);