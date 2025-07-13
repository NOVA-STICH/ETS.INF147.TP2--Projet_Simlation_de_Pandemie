#pragma once
/******************************************************************************/
/* Module:  t_mur.h                                                          */
/* NOMS:    Maxime Nebili + Guillaume Cadieux                                 */
/* Date:    13 Juillet 2025                                                   */
/* Info:    Librairie de gestion du mur.                                      */
/******************************************************************************/
// Directive qui permet de désactiver certains warnings de Visual-Studio
#define _CRT_SECURE_NO_WARNINGS

#include <math.h>
#include "utilitaire_affichage.h"

#define NB_JOURS_MAX 60    // nombre de jours avant l'ouverture du mur
#define DIST_MUR (3 * RAYON) // si on est à moins de 3*5 pixels = contact avec le mur

typedef struct {
    int pos_mur;    // position en X du mur
    int longueur;   // la longueur en Y
    int nb_jours;   // nombre de jours depuis l'instauration du mur de confinement
} t_mur;

//=======================================================================================
/* INIT_MUR
   Cette fonction reçoit la position du mur ainsi que sa longueur.
   Elle déclare et initialise une variable de type « t_mur » avec les deux valeurs
   reçues et retourne ce nouveau mur ainsi créé.
   PARAMÈTRES : pos_mur - position en X du mur (int)
                longueur - longueur du mur en Y (int)
   RETOUR : structure t_mur initialisée */
t_mur init_mur(int pos_mur, int longueur);

//=======================================================================================
/* CONTACT_MUR
   Reçoit un mur par référence (t_mur*) et la position X d'une personne (un réel).
   La fonction vérifie si cette position est à moins de DIST_MUR pixels
   (distance absolue) et retourne true si c'est le cas, false sinon.
   PARAMÈTRES : *mur - pointeur sur le mur (t_mur*)
                pos - position X à vérifier (double)
   RETOUR : booléen indiquant si contact avec le mur */
bool contact_mur(const t_mur* mur, double pos);

//=======================================================================================
/* MUR_ACTIF
   Reçoit un mur par référence (t_mur*) et incrémente le nombre de jours
   de confinement de ce mur. Si ce nombre atteint NB_JOURS_MAX la fonction retourne false
   (plus de mur). Sinon elle retourne true (le mur est toujours actif).
   PARAMÈTRES : *mur - pointeur sur le mur (t_mur*)
   RETOUR : booléen indiquant si le mur est toujours actif */
bool mur_actif(t_mur* mur);

//=======================================================================================
/* DESSINER_MUR
   Reçoit un mur par référence (t_mur*) et dessine le mur à l'écran
   (pour le mode AVEC affichage). Utilise la fonction
   « afficher_mur() » du module « utilitaires_affichage ».
   PARAMÈTRES : *mur - pointeur sur le mur (t_mur*) */
void dessiner_mur(const t_mur* mur);