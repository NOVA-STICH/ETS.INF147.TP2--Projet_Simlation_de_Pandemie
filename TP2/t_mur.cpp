/******************************************************************************/
/* t_mur.H																      */
/* NOMS: Maxime Nebili + Guillaum Cadieux                                     */
/* Date   : 12 Juillet 2025                                                   */
/* CPP de gestion du mur.													  */
/******************************************************************************/

#include"t_mur.h"

//=======================================================================================
// init_mur(..) : Cette fonction reçoit la position du mur ainsi que sa longueur.
// Elle déclare et initialise une variable de type « t_mur » avec les deux valeurs
// reçues et retourne ce nouveau mur ainsi crée.
t_mur init_mur(int pos_mur, int longueur) {
	t_mur mur = {0}; // Met tout les parametre a zero initalement
	mur.longueur = (longueur) ;
	mur.pos_mur = pos_mur;
	return mur;
}

//=======================================================================================
// contact_mur(..) : Reçoit un mur par référence(t_mur*) et la position - X
// d’une personne(un réel). La fonction vérifie si cette position est à moins de DIST_MUR
// pixels(distance absolue) et retourne 1 si c’est le cas, 0 sinon.
bool contact_mur(const t_mur *mur, double pos_personne) {
	return (((mur->pos_mur - pos_personne) < DIST_MUR) ? 1 : 0);
}

//=======================================================================================
// mur_actif(..) : Reçoit un mur par référence(t_mur*) et incrémente le nombre de jours
// de confinement de ce mur.Si ce nombre atteint NB_JOURS_MAX la fonction retourne 0
// (plus de mur). Sinon elle retourne 1 (le mur est toujours actif).
bool mur_actif(t_mur *mur) {
	mur->nb_jours++;
	return ((mur->nb_jours >= NB_JOURS_MAX) ? 0 : 1);
}

//=======================================================================================
// dessiner_mur(..) : Reçoit un mur par référence(t_mur*) et dessine le mur à l'écran
// (pour le mode AVEC affichages).Utilisez ici la fonction 
// « afficher_mur() » du module « utilitaires_affichage ».
void dessiner_mur(const t_mur *mur) {
	afficher_mur(mur->pos_mur,mur->longueur);
}
