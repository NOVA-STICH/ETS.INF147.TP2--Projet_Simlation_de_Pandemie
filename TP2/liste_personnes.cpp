/*===========================================================*/
/* 	INF147 - TP2 */
/*===========================================================*/
/*
 Module : liste_personnes.cpp
 Par    : Maxime Nebili + Guillaum Cadieux
 Date   : 10 Juillet 2025
 Info	: Module qui consiste à controler la liste de personne
*/
/*=========================================================*/
#include "liste_personnes.h"


            /* ---------------------------------*/
            /* ------  ZONE DE FONCTIONS ------ */
            /* ---------------------------------*/

// ==================================================================

void int_liste_personnes(t_liste_personnes *liste_pers,int nbr_personne, double prob_quar,
    int hauteur, int largeur) {
    int i, j, col = 0 ;
    double px, py;

    liste_pers->liste =(t_personne*) malloc(nbr_personne * sizeof(t_personne));
    if (liste_pers->liste == NULL) {
        printf("Erreur d'allocation de mémoire");
    }

    liste_pers->taille = nbr_personne;
    liste_pers->nb_pers = nbr_personne;
    liste_pers->nb_sante = nbr_personne;

    for (i = 0; i < nbr_personne;i++) {
        do {
            px = randi(largeur);
            py = randi(hauteur);
            for (j = 0; j < i;j++) {
                col = contact_personnes(&(liste_pers->liste[j]), px, py);
            }
        } while (col);
        liste_pers->liste[i] = init_personne(px, py, prob_quar);
    }
}

void vider_liste_personnne(t_liste_personnes* liste_pers) {
    if (liste_pers->liste != NULL) {
        free(liste_pers->liste);
        liste_pers->liste = NULL;
    }
    liste_pers->taille = 0;
    liste_pers->nb_pers = 0;
    liste_pers->nb_malades = 0;
    liste_pers->nb_sante = 0;
    liste_pers->nb_morts = 0;
}

void patient_zero(t_liste_personnes* liste_pers) {
    int x;
    x = randi(liste_pers->taille);
    liste_pers->liste[x].etat = MALADE;
    liste_pers->nb_malades += 1;
    liste_pers->nb_sante -= 1;
}

void patient_mort(t_liste_personnes* liste_pers, int pos_mort) {
    t_personne personne_temp;
    int dernier_vivant;

    dernier_vivant = (liste_pers->taille - (liste_pers->nb_morts - 1));

    liste_pers->liste[pos_mort].etat = MORT;

    personne_temp = liste_pers->liste[pos_mort];

    liste_pers->liste[pos_mort] = liste_pers->liste[dernier_vivant];
    liste_pers->liste[dernier_vivant] = personne_temp;

    liste_pers->nb_malades -= 1;
    liste_pers->nb_morts += 1;
    liste_pers->nb_pers -= 1;
}

int traitemnt(t_liste_personnes* liste_pers, double prob_quar,
    int hauteur, int largeur) {
    int i, j;
    int heure_malade;
    int nb_vivant = liste_pers->taille - liste_pers->nb_morts;
    double px, py;
    int col;

    for (i = 0;i < nb_vivant; i++) {
        if (liste_pers->liste[i].quarantaine != 1) {
            deplacer_personne(&liste_pers->liste[i], largeur, hauteur,
                mode_mur, mur);

            px = liste_pers->liste[i].posx;
            py = liste_pers->liste[i].posy;

            for (j = 0; j < i;j++) {
                col = contact_personnes(&(liste_pers->liste[j]), px, py);
            }
        }
        if (liste_pers->liste[i].etat == MALADE) {
            heure_malade = get_hrs_malade(&liste_pers->liste[i]);
            if ((heure_malade + 10) <= NB_HRS_QUAR) {
                liste_pers->liste[i].hrs_malade += 10;
            }
            else if (determiner_mort(&liste_pers->liste[i])) {
                patient_mort(liste_pers, i);
                i--;
            }
            else {
                liste_pers->liste[i].etat = EN_SANTE;
                liste_pers->nb_malades -= 1;
                liste_pers->nb_sante += 1;
            }
        }
    }
    return liste_pers->nb_malades;
}
void imprimer_pers(t_liste_personnes* liste_pers) {
    int i;
    int nb_vivant = liste_pers->taille - liste_pers->nb_morts;
    double px, py;
    int etat, couleur;

    for (i = 0;i < nb_vivant; i++) {
        etat = get_etat(&liste_pers->liste[i]);
        px = liste_pers->liste[i].posx;
        py = liste_pers->liste[i].posy;

        int etat =
            EN_SANTE ? GREEN :
            MALADE ? RED :
            get_quarantaine(&liste_pers->liste[i]) ? BLUE;
        afficher_cercle(px, py, couleur);
    }
}