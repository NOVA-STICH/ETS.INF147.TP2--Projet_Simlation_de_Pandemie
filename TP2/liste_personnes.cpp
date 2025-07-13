/*===========================================================*/
/* 	INF147 - TP2 */
/*===========================================================*/
/*
 Module : liste_personnes.cpp
 Par    : Maxime Nebili + Guillaume Cadieux
 Date   : 13 Juillet 2025
 Info	: Module qui consiste � contr�ler la liste de personnes
*/
/*=========================================================*/
#include "liste_personnes.h"


/* ------  ZONE DE FONCTIONS ------ */
// ======================================================================================
//Cr�e une liste de personnes avec une position al�atoire
void init_liste_personnes(t_liste_personnes* liste_pers, int nbr_personne,
	double prob_quar, int hauteur, int largeur, int mode_mur, const t_mur* mur) {

	//Variables :
	int i, j, col = 0, col_mur = 0;
	double px = 0.0, py = 0.0;

	// Allocation m�moire pour la liste (tableau dynamique) :
	liste_pers->liste = (t_personne*)malloc(nbr_personne * sizeof(t_personne));
	assert(liste_pers->liste);
	
	// Initialisation des valeurs :
	liste_pers->taille = nbr_personne;
	liste_pers->nb_pers = nbr_personne;
	liste_pers->nb_sante = nbr_personne;
	liste_pers->nb_malades = 0;
	liste_pers->nb_morts = 0;

	// Initialisation des personnes
	for (int i = 0; i < nbr_personne; i++) {
		// Tant que nous avons une collision, il va r�essayer :
		do {
			// G�n�ration de position :
			do {
				px = RANDI(largeur);
				col_mur = mode_mur ? contact_mur(mur, px) : 0;
			} while (col_mur);

			py = RANDI(hauteur);

			// V�rification des collisions :
			col = 0;
			for (int j = 0; j < i && !col; j++) {
				col = contact_personnes(&(liste_pers->liste[j]), px, py);
			}
		} while (col);

		// Initialisation de la personne :
		liste_pers->liste[i] = init_personne(px, py, prob_quar);
	}
}

// ======================================================================================
// Vide la liste de personnes :
void vider_liste_personnne(t_liste_personnes* liste_pers) {
	if (liste_pers->liste != NULL) {
		free(liste_pers->liste);       // lib�ration du tableau dynamique
		liste_pers->liste = NULL;
	}
	// mise � z�ro des valeurs :
	liste_pers->taille = 0;
	liste_pers->nb_pers = 0;
	liste_pers->nb_malades = 0;
	liste_pers->nb_sante = 0;
	liste_pers->nb_morts = 0;
}

// ======================================================================================
// Cr�e le premier patient de la maladie
void patient_zero(t_liste_personnes* liste_pers) {
	//choisit un entier al�atoire plus petit que la taille du tableau :
	int x = RANDI(liste_pers->taille);

	//Met l'�tat de la personne x � MALADE :
	modifier_etat_pers(&liste_pers->liste[x], MALADE, 0);

	//Ajuste les compteurs :
	liste_pers->nb_malades++;
	liste_pers->nb_sante--;
}

// ======================================================================================
//Tue la personne � la position donn�e :
void patient_mort(t_liste_personnes* liste_pers, int pos_mort) {
	t_personne personne_temp;
	int dernier_vivant = 0;

	//position de la derni�re personne du tableau en vie :
	dernier_vivant = liste_pers->nb_pers - 1;

	modifier_etat_pers(&liste_pers->liste[pos_mort], MORT, 0);

	//copie temporaire de la personne morte :
	personne_temp = liste_pers->liste[pos_mort];

	//copie le dernier vivant � la place du mort et le mort � la fin du tableau :
	liste_pers->liste[pos_mort] = liste_pers->liste[dernier_vivant];
	liste_pers->liste[dernier_vivant] = personne_temp;

	//ajuste les compteurs :
	liste_pers->nb_malades--;
	liste_pers->nb_morts++;
	liste_pers->nb_pers--;
}

// ======================================================================================
/*D�place toute personne vivante et pas en quarantaine, si la personne est malade
*on rajoute des heures de maladie, si elle atteint le nombre d'heures maximum on v�rifie si
elle meurt ou non, si la personne ne meurt pas elle retourne � EN_SANTE*/
int traitement(t_liste_personnes* liste_pers, double prob_quar,
	int hauteur, int largeur, int mode_mur, const t_mur* mur) {

	// Variables :
	int i, nb_vivant = (liste_pers->taille - liste_pers->nb_morts);
	double px = 0.0, py = 0.0;

	//Boucle pour toutes les personnes vivantes :
	for (i = 0;i < nb_vivant; i++) {
		//On regarde si la personne est en quarantaine
		if (!(get_quarantaine(&liste_pers->liste[i]))) {
			// On d�place la personne :
			deplacer_personne(&liste_pers->liste[i], largeur, hauteur, mode_mur, mur);
			// On prend la position de la personne :
			get_pos_personne(&liste_pers->liste[i], &px, &py);
			// Compare la position avec celles de la liste :
			verif_contac_pers(liste_pers, &liste_pers->liste[i], prob_quar,
				hauteur, largeur, mode_mur, mur);

		}
		//V�rifie si son �tat est MALADE
		if (get_etat(&liste_pers->liste[i]) == MALADE) {
			//Augmente son nombre d'heures de quarantaine :
			ajouter_hr_quar(&liste_pers->liste[i]);
			//V�rifie qu'elle n'a pas encore atteint la limite d'heures :    
			if (get_hrs_malade(&liste_pers->liste[i]) >= NB_HRS_QUAR) {
				// V�rification si elle doit mourir :
				if (determiner_mort(&liste_pers->liste[i])) {
					patient_mort(liste_pers, i);
					i--;
				}
				// sinon, on la remet en SANTE :
				else {
					modifier_etat_pers(&liste_pers->liste[i], EN_SANTE, 0);
					liste_pers->nb_malades--;
					liste_pers->nb_sante++;
				}
			}
		}
	}
	// Retourne le nombre de malades dans la liste :
	return liste_pers->nb_malades;
}

// ======================================================================================
//Dessine les personnes sur l'�cran :
void imprimer_pers(t_liste_personnes* liste_pers) {
	// Variables : 
	int i = 0, etat = 0, quar = 0, couleur = 0;
	double px = 0.0, py = 0.0;

	// it�ration pour toute la liste :
	for (i = 0;i < liste_pers->taille; i++) {
		//Prend l'�tat, la valeur de quarantaine et la position de la personne :
		etat = get_etat(&liste_pers->liste[i]);
		quar = get_quarantaine(&liste_pers->liste[i]);
		get_pos_personne(&liste_pers->liste[i], &px, &py);

		// S�lection de la couleur selon l'�tat de sant� :
		if (etat == EN_SANTE) couleur = GREEN;
		else if ((etat == MALADE) && (quar == 0)) couleur = RED;
		else if ((etat == MALADE) && (quar == 1)) couleur = BLUE;
		else if (etat == MORT) couleur = YELLOW;

		// affichage de la personne � l'�cran :
		afficher_cercle(round(px), round(py), couleur);
	}
}

// ======================================================================================
// Fonction qui g�re la propagation de la maladie au contact :
static void verif_contac_pers(t_liste_personnes* liste_pers, t_personne* traiter, double prob_quar,
	int hauteur, int largeur, int mode_mur, const t_mur* mur) {
	// Variables : 
	int i, nb_pers = 0, etat1 = 0, etat2 = 0, copie = 0;
	double px1 = 0.0, py1 = 0.0, px2 = 0.0, py2 = 0.0, prob_inf1, prob_inf2;

	// it�ration pour les personnes dans la liste :
	for (i = 0; i < (get_nb_personnes(liste_pers)); i++) {
		// stocke l'�tat de la personne trait�e :
		etat2 = get_etat(traiter);
		// stocke la probabilit� d'infection de la personne trait�e :
		prob_inf2 = get_prob_inf(traiter);
		// met � jour les positions de la personne trait�e :
		get_pos_personne(traiter, &px2, &py2);

		// stocke l'�tat de l'autre personne :
		etat1 = get_etat(&liste_pers->liste[i]);
		// stocke la probabilit� d'infection de l'autre personne :
		prob_inf1 = get_prob_inf(&liste_pers->liste[i]);
		// met � jour les positions de l'autre personne :
		get_pos_personne(&liste_pers->liste[i], &px1, &py1);

		// V�rifie qu'il ne s'agit pas de la m�me personne :
		copie = ((px1 == px2) &&
			(py1 == py2) &&
			(liste_pers->liste[i].age == traiter->age) &&
			(liste_pers->liste[i].nb_inf == traiter->nb_inf));

		// v�rifie que ce ne soit pas la m�me personne et v�rifie les contacts
		if (!copie && contact_personnes(traiter, px1, py1)) {
			// si la personne trait�e est malade et que l'autre est en sant�
			if ((etat1 == EN_SANTE) && (etat2 == MALADE)) {
				// v�rifie si la probabilit� d'infection est valable :
				if (RANDF < prob_inf1) {
					// modifie l'�tat de la personne et ajuste les compteurs :
					modifier_etat_pers(&liste_pers->liste[i], MALADE,
						((RANDF < prob_quar) ? 1 : 0));
					liste_pers->nb_malades++;
					liste_pers->nb_sante--;
					liste_pers->liste[i].nb_inf++;
				}
			}
			// si l'autre est malade et que la personne trait�e est en sant�
			else if ((etat1 == MALADE) && (etat2 == EN_SANTE)) {
				// v�rifie si la probabilit� d'infection est valable :
				if (RANDF < prob_inf2) {
					// modifie l'�tat de la personne et ajuste les compteurs :
					modifier_etat_pers(traiter, MALADE, ((RANDF < prob_quar) ? 1 : 0));
					liste_pers->nb_malades++;
					liste_pers->nb_sante--;
					traiter->nb_inf++;
				}
			}
			// inversion de la direction et d�placement de la personne :
			inverser_dir_pers(&liste_pers->liste[i], traiter);
			deplacer_personne(traiter, largeur, hauteur,
				mode_mur, mur);
			// si la personne n'est pas en quarantaine, alors on peut la d�placer :
			if (!get_quarantaine(&liste_pers->liste[i])) {
				deplacer_personne(&liste_pers->liste[i], largeur, hauteur,
					mode_mur, mur);
			}
		}
	}
}

// ======================================================================================
// Calcule et retourne la moyenne du nombre d'infections de TOUTES les personnes re�ues
// et aussi r�cup�re le nombre maximal d'infections ainsi que le nombre total de
// personnes qui n'ont jamais �t� infect�es :
double nb_moyen_inf(const t_liste_personnes* liste_pers, int* max, int* nb_zero_fois) {
	int i = 0; // Variable pour la boucle FOR
	double total = 0;
	*max = 0;
	*nb_zero_fois = 0;

	for (i = 0; i < liste_pers->taille; i++) {
		// incr�mente le nombre d'infections total :
		total += liste_pers->liste[i].nb_inf;

		// Modifie la variable MAX si une personne a plus d'infections :
		if (liste_pers->liste[i].nb_inf > *max)	*max = liste_pers->liste[i].nb_inf;
		// Si une personne n'a jamais eu d'infection, on incr�mente le compteur :
		if (liste_pers->liste[i].nb_inf == 0) (*nb_zero_fois)++;
	}
	// Retourne la moyenne :
	return ((double)(total / liste_pers->nb_pers));
}

// ======================================================================================
//Renvoie le nombre de personnes vivantes dans la liste
int get_nb_personnes(const t_liste_personnes* liste_pers) {
	return liste_pers->nb_pers;
}

// ======================================================================================
//Renvoie le nombre de personnes malades
int get_nb_malades(const t_liste_personnes* liste_pers) {
	return liste_pers->nb_malades;
}

// ======================================================================================
//Renvoie le nombre de personnes en sant�
int get_nb_sante(const t_liste_personnes* liste_pers) {
	return liste_pers->nb_sante;
}

// ======================================================================================
//Renvoie le nombre de personnes mortes
int get_nb_morts(const t_liste_personnes* liste_pers) {
	return liste_pers->nb_morts;
}
