/*===========================================================*/
/* 	INF147 - TP2                                             */
/*===========================================================*/
/******************************************************************************/
/* Module:  t_mur.cpp                                                        */
/* NOMS:    Maxime Nebili + Guillaume Cadieux                                 */
/* Date:    13 Juillet 2025                                                   */
/* Info:    Impl�mentation de la gestion du mur.                              */
/******************************************************************************/
#include "t_mur.h"

//=======================================================================================
/* Cette fonction re�oit la position du mur ainsi que sa longueur.
   Elle d�clare et initialise une variable de type � t_mur � avec les deux valeurs
   re�ues et retourne ce nouveau mur ainsi cr��. */
t_mur init_mur(int pos_mur, int longueur) {
    t_mur mur = { 0 }; // Initialise tous les param�tres � z�ro
    mur.longueur = longueur;
    mur.pos_mur = pos_mur;
    return mur;
}

//=======================================================================================
/* Re�oit un mur par r�f�rence et la position X d'une personne.
   V�rifie si cette position est � moins de DIST_MUR pixels (distance absolue)
   et retourne true si c'est le cas, false sinon. */
bool contact_mur(const t_mur* mur, double pos_personne) {
    return (fabs(mur->pos_mur - pos_personne) < DIST_MUR);
}

//=======================================================================================
/* Re�oit un mur par r�f�rence et incr�mente le nombre de jours de confinement.
   Si ce nombre atteint NB_JOURS_MAX, retourne false (mur d�sactiv�).
   Sinon retourne true (mur toujours actif). */
bool mur_actif(t_mur* mur) {
    mur->nb_jours++;
    return (mur->nb_jours < NB_JOURS_MAX);
}

//=======================================================================================
/* Re�oit un mur par r�f�rence et le dessine � l'�cran 
   (pour le mode avec affichage graphique).
   Utilise la fonction afficher_mur() du module utilitaires_affichage. */
void dessiner_mur(const t_mur* mur) {
    afficher_mur(mur->pos_mur, mur->longueur);
}