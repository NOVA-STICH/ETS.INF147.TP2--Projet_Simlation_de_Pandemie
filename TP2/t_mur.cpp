/*===========================================================*/
/* 	INF147 - TP2                                             */
/*===========================================================*/
/******************************************************************************/
/* Module:  t_mur.cpp                                                        */
/* NOMS:    Maxime Nebili + Guillaume Cadieux                                 */
/* Date:    13 Juillet 2025                                                   */
/* Info:    Implémentation de la gestion du mur.                              */
/******************************************************************************/
#include "t_mur.h"

//=======================================================================================
/* Cette fonction reçoit la position du mur ainsi que sa longueur.
   Elle déclare et initialise une variable de type « t_mur » avec les deux valeurs
   reçues et retourne ce nouveau mur ainsi créé. */
t_mur init_mur(int pos_mur, int longueur) {
    t_mur mur = { 0 }; // Initialise tous les paramètres à zéro
    mur.longueur = longueur;
    mur.pos_mur = pos_mur;
    return mur;
}

//=======================================================================================
/* Reçoit un mur par référence et la position X d'une personne.
   Vérifie si cette position est à moins de DIST_MUR pixels (distance absolue)
   et retourne true si c'est le cas, false sinon. */
bool contact_mur(const t_mur* mur, double pos_personne) {
    return (fabs(mur->pos_mur - pos_personne) < DIST_MUR);
}

//=======================================================================================
/* Reçoit un mur par référence et incrémente le nombre de jours de confinement.
   Si ce nombre atteint NB_JOURS_MAX, retourne false (mur désactivé).
   Sinon retourne true (mur toujours actif). */
bool mur_actif(t_mur* mur) {
    mur->nb_jours++;
    return (mur->nb_jours < NB_JOURS_MAX);
}

//=======================================================================================
/* Reçoit un mur par référence et le dessine à l'écran 
   (pour le mode avec affichage graphique).
   Utilise la fonction afficher_mur() du module utilitaires_affichage. */
void dessiner_mur(const t_mur* mur) {
    afficher_mur(mur->pos_mur, mur->longueur);
}