/*===========================================================*/
/* 	INF147 - TP2 */
/*===========================================================*/
/*
 Module : personne.cpp
 Par    : Maxime Nebili + Guillaum Cadieux
 Date   : 7 Juillet 2025
 Info	: Module qui consiste � faire les fonctions utilitaires des personnes.
*/
/*=========================================================*/
#include "personne.h"


            /* ---------------------------------*/
            /* ------  ZONE DE FONCTIONS ------ */
            /* ---------------------------------*/

// ==================================================================
/* INIT_PERSONNE
   Fonction qui va initialiser et retourner une nouvelle personne.
   PARAM�TRES : px, py - position initiale de la personne (double).
                prob_quar - la probabilit� que cette personne soit en quarantaine (double)
   RETOUR : une nouvelle personne (t_personne).    */
t_personne init_personne(double px, double py, double prob_quar) {

    double angle_radian = 0 ; // Variable qui vas contenir l'angle aleatoire.
    t_personne nouvelle_personne; // creation de le personne

    nouvelle_personne.quarantaine = ((RANDF < prob_quar) ? 1 : 0); // Quarantaine?
    angle_radian = RANDF_BORNES(0.5, 1.5); // Generation de l'angle aleatoire

    // Pour les variables de vitesses, on fait un comparaison avec un valeur 1/2 et
    // si elle est inferieur, alors on multiplie le resultat des angles avec celle si:
    nouvelle_personne.vitx = (((RANDF < 0.5) ? -1 : 1) * (2 * cos(angle_radian)));
    nouvelle_personne.vity = (((RANDF < 0.5) ? -1 : 1) * (2 * sin(angle_radian)));
    nouvelle_personne.posx = px; // init sa position x
    nouvelle_personne.posy = py; // init sa position y
    nouvelle_personne.etat = EN_SANTE; // init son etat de sante
    nouvelle_personne.age = rand_age_canada(); // init son age
    nouvelle_personne.prob_inf = PROB_CONTAGION; // init la probabiliter de contagion
    nouvelle_personne.hrs_malade = 0; // init les nb d'heures
    nouvelle_personne.nb_inf = 0; // init les nb d'infections
    
    return nouvelle_personne;
}

// ==================================================================
/* DETERMINER_MORT
   Re�oit une personne et d�termine s'il va mourir selon son age et son
   nombre d'infections.  Un r�el al�atoire g�n�r� est compar� avec:
     randf() < (PROB_DECES[..case selon l'age..] / (nb_infections + 1))
   PARAM�TRES : *lui - pointeur sur une personne (t_personne).
   RETOUR : 1 si cette personne est destin� � mourir, 0 sinon (bool�en, 0 ou 1).   */
int determiner_mort(const t_personne* lui) {
    
    // initalise et d�termine la tranche d'age � utiliser:
    int tranche = 
        (lui->age < 30) ? 0 :
        (lui->age < 40) ? 1 :
        (lui->age < 50) ? 2 :
        (lui->age < 60) ? 3 :
        (lui->age < 70) ? 4 :
        (lui->age < 80) ? 5 : 6;

    // G�n�re un r�el al�atoire entre [0.0, 1.0] et si ce r�el est inf�rieur
    // � (probabilit� de d�c�s de la personne / (son nombre d�infections + 1)),
    // on retourne 1 : 
#if (MODE_MACRO == 1)
    return ((RANDF < (PROB_DECES[tranche] / (lui->nb_inf + 1))) ? 1:0);
#else
    return ((randf() < (PROB_DECES[tranche] / (lui->nb_inf + 1))) ? 1 : 0);
#endif
}

// ==================================================================
/* CONTACT_PERSONNES
   Re�oit une personne et une position. D�terminera s'il cette personne
   entre en contact avec la position re�ue.
   PARAM�TRES : *lui - pointeur sur une personne (t_personne).
                px, py - la position � v�rifier (double).
   RETOUR : 1 si cette personne est situ� � la position [py,px], 0 sinon (bool�en, 0 ou 1). */
int contact_personnes(const t_personne* lui, double px, double py) {
    // Utilisation d'une MACRO d�finie dans "personne.h" pour futur simplification
    return ((DISTANCE_2P(lui->posx, lui->posy, px, py) < 2 * RAYON_1m) ? 1 : 0);
}

/****************** FONCTIONS MUTATEURS DES CHAMPS ****************************/
// ==================================================================
/* DEPLACER_PERSONNES
   Re�oit une personne et la d�place selon sa position. Suite au d�placement on
   v�rifie si on doit le faire "rebondir" sur une des bordures.
   On re�oit aussi le "mode_mur" et le mur pour valider une collision avec le mur.
   PARAM�TRES : *lui - pointeur sur la personne � modifier (t_personne).
                largeur, hauteur - les dimension de l'espace de simulation (int).
                mode_mur - �tat du mode avec ou sans mur de confinement (bool�en, 0 ou 1).
                mur - le mur de confinement (t_mur *).   */
void deplacer_personne(t_personne* lui, int largeur, int hauteur, 
    	                   int mode_mur, const t_mur* mur ) {    //  (SEM. 3)
    
    // D�placement initial : 
    lui->posx += lui->vitx; // Ajout de la vitesse � sa position en x
    lui->posy += lui->vity; // Ajout de la vitesse � sa position en y

    // Aprochement de la bordure minimal/maximal X
    // On inverse la direction et on ajoute un deplacement X:
    if ((lui->posx < RAYON_1m) || ((largeur - lui->posx) < RAYON_1m)) {
        lui->vitx *= -1;
        lui->posx += lui->vitx;
    }
    // Aprochement de la bordure minimal/maximal Y
    // On inverse la direction et on ajoute un deplacement Y:
    if ((lui->posy < RAYON_1m) || ((hauteur - lui->posy) < RAYON_1m)) {
        lui->vity *= -1;
        lui->posy += lui->vity;
    }
    // Si le mur est actif et qu'il y a un contact avec le mur,
    // on inverse la direction et on ajoute un deplacement X:
    if((mode_mur == 1) && (contact_mur(mur, lui->posx))){
        lui->vitx *= -1;
        lui->posx += lui->vitx;
    }
}

// ==================================================================
/* INVERSER_DIR_PERS
   Re�oit deux personnes et les d�placent dans deux directions oppos�es suite
   � un contact.
   PARAM�TRES : *lui, *autre - pointeurs sur les 2 personnes (t_personne *).  */
void inverser_dir_pers(t_personne* lui, t_personne* autre) {

    double angle_radian = 0; // Variable qui vas contenir l'angle aleatoire.

#if(MODE_MACRO == 1)
    angle_radian = RANDF_BORNES(0.5, 1.5); // Generation de l'angle aleatoire
#else
    angle_radian = randf_bornes(0.5, 1.5); // Generation de l'angle aleatoire
#endif

    // Pour les variables de vitesses, on fait un comparaison avec un valeur 1/2 et
    // si elle est inferieur, alors on multiplie le resultat des angles avec celle si:
    lui->vitx = (((randf() < 0.5) ? -1 : 1) * (2 * cos(angle_radian)));
    lui->vity = (((randf() < 0.5) ? -1 : 1) * (2 * sin(angle_radian)));
    
    // Pour la deuxieme personne, on copie la vitesse de la personne 1 et on l'inverse:
    autre->vitx = (-1 * lui->vitx);
    autre->vity = (-1 * lui->vitx);
}

// ==================================================================
/* MODIFIER_ETAT_PER
   Re�oit une personne, un nouvel �tat et un mode de quarantaine.
   Modifiera l'�tat de la personne ainsi que son �tat de quarantaine.
   Si le nouvel �tat re�u est "EN_SANTE", on doit diviser sa probabilit�
   d'infection par trois (3) et incr�menter son nombre d�infections.
   On va �galement remettre son nombre d'heures en quarantaine � 0.
   PARAM�TRES : *lui - pointeur sur la personne � modifier (t_personne).
                etat - nouvel �tat � attribuer � la personne (t_etat).
                mode_quar - le mode de quarantaine de la personne (bool�en, 0 ou 1). */
void modifier_etat_pers(t_personne* lui, t_etat etat, int mode_quar) {
    lui->etat = etat;        // transfert de l'etat de la personne
    if (etat == EN_SANTE) {  // si sont etat est "EN_SANTE" alors:
        lui->prob_inf /= 3;  // diviser sa probabilit� d�infection par trois
        lui->nb_inf++;       // incrementer le nombre d'infection
        lui->hrs_malade = 0; // remise a zero du nb d'heure malade/quarantaine
    }
}

// ======================================================================================
/* AJOUTER_HR_QUAR
   Re�oit une personne et va incr�menter (+1) son nombre d'heures de quarantaine.
   PARAM�TRES : *lui - pointeur sur la personne � modifier (t_personne).      */
void ajouter_hr_quar(t_personne* lui) { lui->hrs_malade++; }


// ========================================
// ==== FONCTION SUPLEMENTAIRE "GET" ======
// ========================================


/* GET_POS_PERSONNE
   Re�oit une personne et retourne sa position via (*px, *py).
   PARAM�TRES : *lui - pointeur sur une personne (t_personne).
                *px, *py - param�tres de retour pour r�cup�rer la position (double *).  */
void get_pos_personne(const t_personne* lui, double* px, double* py){
    *px = lui->posx;
    *py = lui->posy;
}

/* GET_ETAT
   Re�oit une personne et retourne son �tat actuel.
   PARAM�TRES : *lui - pointeur sur une personne (t_personne).
   RETOUR : l'�tat de la personne (t_etat).    */
t_etat get_etat(const t_personne* lui) {
    return lui->etat;
}

/* GET_PROB_INF
   Re�oit une personne et retourne sa probabilit� de contagion.
   PARAM�TRES : *lui - pointeur sur une personne (t_personne).
   RETOUR : la probabilit� d.infection de la personne (double).   */
double get_prob_inf(const t_personne* lui) {
    return lui->prob_inf;
}

/* GET_QUARANTAINE
   Re�oit une personne et retourne son �tat de quarantaine.
   PARAM�TRES : *lui - pointeur sur une personne (t_personne).
   RETOUR : l'�tat de quarantaine de la personne (bool�en, 0 ou 1).   */
int get_quarantaine(const t_personne* lui) {
    return lui->quarantaine;
}

/* GET_HRS_MALADE
   Re�oit une personne et retourne le nombre d'heures qu'il a �t� malade.
   PARAM�TRES : *lui - pointeur sur une personne (t_personne).
   RETOUR : nombre d'heures de maladie de la personne (int).    */
int get_hrs_malade(const t_personne* lui) {
    return lui->hrs_malade;
}







