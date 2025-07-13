/*===========================================================*/
/* 	INF147 - TP2                                             */
/*===========================================================*/
/*
 Module : personne.cpp
 Par    : Maxime Nebili + Guillaume Cadieux
 Date   : 13 Juillet 2025
 Info	: Module impl�mentant les fonctions utilitaires des personnes.
*/
/*=========================================================*/
#include "personne.h"



/*=========================================================*/
/*                IMPL�MENTATION DES FONCTIONS             */
/*=========================================================*/

// ======================================================================================
// Initialise et retourne une nouvelle personne:
t_personne init_personne(double px, double py, double prob_quar) {
    double angle_radian = 0;  // Variable contenant l'angle al�atoire
    t_personne nouvelle_personne; // Cr�ation de la personne

    nouvelle_personne.quarantaine = ((RANDF < prob_quar) ? 1 : 0); // Quarantaine?
    angle_radian = RANDF_BORNES(0.5, 1.5); // G�n�ration de l'angle al�atoire

    // D�termination de la vitesse selon un angle al�atoire
    nouvelle_personne.vitx = (((RANDF < 0.5) ? -1 : 1) * (VITESSE * cos(angle_radian)));
    nouvelle_personne.vity = (((RANDF < 0.5) ? -1 : 1) * (VITESSE * sin(angle_radian)));
    nouvelle_personne.posx = px;                    // Position x
    nouvelle_personne.posy = py;                    // Position y
    nouvelle_personne.etat = EN_SANTE;              // �tat de sant� initial
    nouvelle_personne.age = rand_age_canada();      // �ge al�atoire
    nouvelle_personne.prob_inf = PROB_CONTAGION;    // Probabilit� de contagion
    nouvelle_personne.hrs_malade = 0;               // Heures de maladie/quarantaine
    nouvelle_personne.nb_inf = 0;                   // Nombre d'infections

    return nouvelle_personne;
}

// ======================================================================================
// D�termine si la personne va mourir selon son �ge et nombre d'infections:
int determiner_mort(const t_personne* lui) {
    // D�termination de la tranche d'�ge
    int tranche =
        (lui->age < 30) ? 0 :
        (lui->age < 40) ? 1 :
        (lui->age < 50) ? 2 :
        (lui->age < 60) ? 3 :
        (lui->age < 70) ? 4 :
        (lui->age < 80) ? 5 : 6;

    // Comparaison avec probabilit� ajust�e selon infections
    return ((RANDF < (PROB_DECES[tranche] / (lui->nb_inf + 1))) ? 1 : 0);
}

// ======================================================================================
// V�rifie si la personne est en contact avec une position donn�e.
int contact_personnes(const t_personne* lui, double px, double py) {
    return (DISTANCE_2P(lui->posx, lui->posy, px, py) < 2 * RAYON_1m);
}

// ======================================================================================
// D�place la personne et g�re les collisions avec les bords et le mur:
void deplacer_personne(t_personne* lui, int largeur, int hauteur,
    int mode_mur, const t_mur* mur) {
    // D�placement initial
    lui->posx += lui->vitx;
    lui->posy += lui->vity;

    // Gestion des collisions avec les bords horizontaux
    if ((lui->posx < RAYON_1m) || ((largeur - lui->posx) < RAYON_1m)) {
        lui->vitx *= -1;
        lui->posx += lui->vitx;
    }
    // Gestion des collisions avec les bords verticaux
    if ((lui->posy < RAYON_1m) || ((hauteur - lui->posy) < RAYON_1m)) {
        lui->vity *= -1;
        lui->posy += lui->vity;
    }
    // Gestion de la collision avec le mur si actif
    if ((mode_mur == 1) && contact_mur(mur, lui->posx)) {
        lui->vitx *= -1;
        lui->posx += lui->vitx;
    }
}

// ======================================================================================
// Modifie l'�tat et les propri�t�s d'une personne:
void modifier_etat_pers(t_personne* lui, t_etat etat, int mode_quar) {
    lui->etat = etat;
    lui->quarantaine = mode_quar;
    if (etat == EN_SANTE) {
        lui->prob_inf /= 3;  // R�duction probabilit� infection
        lui->nb_inf++;       // Incr�mentation infections
        lui->hrs_malade = 0; // R�initialisation heures maladie
    }
}

// ======================================================================================
// Incr�mente le compteur d'heures de quarantaine/maladie:
void ajouter_hr_quar(t_personne* lui) {
    lui->hrs_malade++;
}



// ========================================
// ==== FONCTION SUPLEMENTAIRE "GET" ======
// ========================================

// ======================================================================================
// Re�oit une personne et retourne sa position via (*px, *py):
void get_pos_personne(const t_personne* lui, double* px, double* py){
    *px = lui->posx;
    *py = lui->posy;
}

// ======================================================================================
// Re�oit une personne et retourne son �tat actuel:
t_etat get_etat(const t_personne* lui) {
    return lui->etat;
}

// ======================================================================================
// Re�oit une personne et retourne sa probabilit� de contagion:
double get_prob_inf(const t_personne* lui) {
    return lui->prob_inf;
}

// ======================================================================================
// Re�oit une personne et retourne son �tat de quarantaine:
int get_quarantaine(const t_personne* lui) {
    if (lui == NULL) return 0;
    return lui->quarantaine;
}

// ======================================================================================
// Re�oit une personne et retourne le nombre d'heures qu'il a �t� malade:
int get_hrs_malade(const t_personne* lui) {
    return lui->hrs_malade;
}