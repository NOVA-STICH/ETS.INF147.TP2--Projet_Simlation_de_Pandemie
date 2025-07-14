#pragma once
/******************************************************************************/
/* Module:  personne.h                                                        */
/* NOMS:    Maxime Nebili + Guillaume Cadieux                                 */
/* Date:    13 Juillet 2025                                                   */
/* Info:    Librairie de gestion des personnes.                               */
/******************************************************************************/
// Directive qui permet de désactiver certains warnings de Visual-Studio
#define _CRT_SECURE_NO_WARNINGS

// Librairies usuelles à inclure
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "alea_pop.h"    //pour les fonctions aléatoires
#include "t_mur.h"       //pour valider contact avec le mur (mode avec mur, SEM. 3)  


/******************************************************************************/
/*                      DÉCLARATIONS DES CONSTANTES                           */
/******************************************************************************/
#define PROB_CONTAGION   0.3    //probabilité de contracter le virus
#define NB_HRS_QUAR     15*24   //nombre d'heures total en quarantaine (ou malade)
#define RAYON_1m          5     //rayon de 1 mètre (en pixels) = rayon d'affichage des cercles

/* constantes relatives aux probabilités de décès selon l'âge */
#define AGE_JEUNE        29     //limite de la première tranche d'âge
#define INC_CAT_AGE      10     //écart entre chaque tranche d'âge
#define VITESSE          30      // Mutiplicateur de vitesse pour aller plus vite   :)

/* prob. de décès selon l'âge [0-29, 30-39, 40-49, 50-59, 60-69, 70-79, >=80] */
const double PROB_DECES[7] = { 0.001, 0.001, 0.004, 0.017, 0.059, 0.182, 0.736 };

/* Source des statistiques :
https://www.inspq.qc.ca/covid-19/donnees?fbclid=IwAR0WOWnq-kyUn_75t-zOXvQmnlNBwyo7MHI57M5k1EILJor4FwmIL51ZDUI
*/

// MACRO pour le calcul de distance entre 2 points: 
#define DISTANCE_2P(x1,y1,x2,y2) (sqrt(((x1)-(x2))*((x1)-(x2)) + ((y1)-(y2))*((y1)-(y2))))

/******************************************************************************/
/*                         DÉCLARATIONS DE TYPES                              */
/******************************************************************************/
typedef enum { MORT, EN_SANTE, MALADE } t_etat;     //les 3 états possibles

/* Type-structure pour une personne */
typedef struct {
    t_etat  etat;            //son état de santé (type enum)
    double  posx, posy;      //sa position
    double  vitx, vity;      //sa vitesse & direction
    int     age;             //son âge (en années)
    int     quarantaine;     //est-il en quarantaine?
    int     hrs_malade;      //nombre d'heures en quarantaine (ou malade)
    double  prob_inf;        //probabilité d'infection, va réduire selon le nb. fois
    int     nb_inf;          //le nombre d'infections
} t_personne;


/******************************************************************************/
/*                         DÉCLARATIONS DE FONCTIONS                          */
/******************************************************************************/

/* INIT_PERSONNE
   Fonction qui va initialiser et retourner une nouvelle personne.
   PARAMÈTRES : px, py - position initiale de la personne (double).
                prob_quar - la probabilité que cette personne soit en quarantaine(double)
   RETOUR : une nouvelle personne (t_personne).    */
t_personne init_personne(double px, double py, double prob_quar);

/****************** FONCTIONS ACCESSEURS AUX CHAMPS ***************************/

/* GET_POS_PERSONNE
   Reçoit une personne et retourne sa position via (*px, *py).
   PARAMÈTRES : *lui - pointeur sur une personne (t_personne).
                *px, *py - paramètres de retour pour récupérer la position (double *).  */
void get_pos_personne(const t_personne* lui, double* px, double* py);

/* GET_ETAT
   Reçoit une personne et retourne son état actuel.
   PARAMÈTRES : *lui - pointeur sur une personne (t_personne).
   RETOUR : l'état de la personne (t_etat).    */
t_etat get_etat(const t_personne* lui);

/* GET_PROB_INF
   Reçoit une personne et retourne sa probabilité de contagion.
   PARAMÈTRES : *lui - pointeur sur une personne (t_personne).
   RETOUR : la probabilité d'infection de la personne (double).   */
double get_prob_inf(const t_personne* lui);

/* GET_QUARANTAINE
   Reçoit une personne et retourne son état de quarantaine.
   PARAMÈTRES : *lui - pointeur sur une personne (t_personne).
   RETOUR : l'état de quarantaine de la personne (booléen, 0 ou 1).   */
int get_quarantaine(const t_personne* lui);

/* GET_HRS_MALADE
   Reçoit une personne et retourne le nombre d'heures qu'il a été malade.
   PARAMÈTRES : *lui - pointeur sur une personne (t_personne).
   RETOUR : nombre d'heures de maladie de la personne (int).    */
int get_hrs_malade(const t_personne* lui);

/* DETERMINER_MORT
   Reçoit une personne et détermine s'il va mourir selon son âge et son
   nombre d'infections.  Un réel aléatoire généré est comparé avec:
     randf() < (PROB_DECES[..case selon l'âge..] / (nb_infections + 1))
   PARAMÈTRES : *lui - pointeur sur une personne (t_personne).
   RETOUR : 1 si cette personne est destinée à mourir, 0 sinon (booléen, 0 ou 1).   */
int determiner_mort(const t_personne* lui);

/* CONTACT_PERSONNES
   Reçoit une personne et une position. Détermine si cette personne
   entre en contact avec la position reçue.
   PARAMÈTRES : *lui - pointeur sur une personne (t_personne).
                px, py - la position à vérifier (double).
   RETOUR : 1 si cette personne est située à la position [py,px], 0 sinon (booléen, 0 ou 1). */
int contact_personnes(const t_personne* lui, double px, double py);


/****************** FONCTIONS MUTATEURS DES CHAMPS ****************************/

/* DEPLACER_PERSONNE
   Reçoit une personne et la déplace selon sa position. Suite au déplacement on
   vérifie si on doit la faire "rebondir" sur une des bordures.
   On reçoit aussi le "mode_mur" et le mur pour valider une collision avec le mur.
   PARAMÈTRES : *lui - pointeur sur la personne à modifier (t_personne).
                largeur, hauteur - les dimensions de l'espace de simulation (int).
                mode_mur - état du mode avec ou sans mur de confinement (booléen, 0 ou 1).
                mur - le mur de confinement (t_mur *).   */
void deplacer_personne(t_personne* lui, int largeur, int hauteur,
    int mode_mur, const t_mur* mur);    //  (SEM. 3)

/* INVERSER_DIR_PERS
   Reçoit deux personnes et les déplace dans deux directions opposées suite
   à un contact.
   PARAMÈTRES : *lui, *autre - pointeurs sur les 2 personnes (t_personne *).  */
void inverser_dir_pers(t_personne* lui, t_personne* autre);

/* MODIFIER_ETAT_PERS
   Reçoit une personne, un nouvel état et un mode de quarantaine.
   Modifie l'état de la personne ainsi que son état de quarantaine.
   Si le nouvel état reçu est "EN_SANTE", on doit diviser sa probabilité
   d'infection par trois (3) et incrémenter son nombre d'infections.
   On va également remettre son nombre d'heures en quarantaine à 0.
   PARAMÈTRES : *lui - pointeur sur la personne à modifier (t_personne).
                etat - nouvel état à attribuer à la personne (t_etat).
                mode_quar - le mode de quarantaine de la personne (booléen, 0 ou 1). */
void modifier_etat_pers(t_personne* lui, t_etat etat, int mode_quar);

/* AJOUTER_HR_QUAR
   Reçoit une personne et incrémente (+1) son nombre d'heures de quarantaine.
   PARAMÈTRES : *lui - pointeur sur la personne à modifier (t_personne).      */
void ajouter_hr_quar(t_personne* lui);