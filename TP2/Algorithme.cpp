/*===========================================================*/
/* 	INF147 - TP2 */
/*===========================================================*/
/*
 Module : Algorithme.cpp
 Par    : Maxime Nebili + Guillaum Cadieux
 Date   : 10 Juillet 2025
 Info	: Module qui consiste à faire des simulations de pandemi.
*/
/*=========================================================*/


/*=========================================================*/
// Librairies usuelles à inclure:
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>
#include<assert.h>
#include"alea_pop.h"
#include"personne.h"
#include"utilitaire_affichage.h"
#include"winBGIm.h"
#include"liste_personnes.h"

/*===================== CONSTANTES =======================*/
#define _CRT_SECURE_NO_WARNINGS		// Désactive certains warnings de Visual-Studio
#define INT_BIT (sizeof(int) * 8)	// CONSTANTE du nb de bits dans un integer.
#define EPSILON (0.0000000001)		// La différence réelle minimale

// Dimensions de l'écran d'affichage (en pixels) selon le FACTEUR du nombre de personnes
#define HAUTEUR (int)( 550 * sqrt(0.8 * FACTEUR))
#define LARGEUR (int)(1200 * sqrt(0.8 * FACTEUR))

#define MODE_TEST 0			// Activation pour les tests unitaire
#define MODE_AFFICHAGE 1	// Simuler avec l’affichage des gens


/*===================== MACRO =======================*/
// Macro pour que le system ne plante pas a l'insertion de texte.
// Insérez l’instruction « FFLUSH(); » juste après vos appels à « scanf() ».
#define FFLUSH() while (getchar() != '\n') {}


//=========================================================
//======= ZONE POUR SOUS-FONCTION TEST UNITAITRE ==========
//=========================================================







//=====================================================
//		       CODE PRINCIPALE			     
//=====================================================


#if (MODE_TEST) // CODE POUR DÉBUGGAGE
int main(void) {

	// Permets de générer des valeurs aléatoires.
	srand(time(NULL));

	

	// TESTS:
	printf("VERIFICATION DE : test_principe1_valide(); \n");
	test_principe1_valide();





	// Fin du block test
	printf("\n\n");
	system("pause");
	return EXIT_SUCCESS;
}
#elif (MODE_AFFICHAGE) // CODE avec afficahge (900 personne max):
int main(void) {
	// Permets de générer des valeurs aléatoires:
	srand(time(NULL));

	// VARIABLES:
	int max_pers_dep = 900, // Le NB maxiamle de personne en deplacement
		i = 0,			// Variable pour la boucle FOR
		jours = 0,		// NB de jours ecoule
		nb_morts = 0,	// NB de mort
		nb_malades = 0,	// NB de malade
		lemax = 0,		// NB maximal de malade
		coul = NULL,	// Variable qui vas contenir la couleur pour l'affichage
		touche = NULL,	// Variable que contien la touche pesée
		nb_heure_simulation = NULL;	// Auto-explicatoire

	// Generation de la prop_quarantaine
	double prop_quarantaine = RANDF;

	

	// Initialiser le mode graphique et l’écran
	init_graphe(LARGEUR, HAUTEUR);
	init_zone_environnement(LARGEUR, HAUTEUR);

	/*======================================*/
	/*==IL FAUT VIDER LA LISTE DE PERSONNE==*/
	/*======================================*/

	/*======================================*/
	/*==IL FAUT GENERE UNE NOUVELLE LISTE ==*/
	/*======================================*/

	/*======================================*/
	/*==IL FAUT GENERE LE PATIEN ZERO     ==*/
	/*======================================*/


	// Afficher les compteurs de depart:
	afficher_infos(jours, nb_morts, nb_malades, lemax);

	// On nettoye l'affichage:
	effacer_zone_environnement(LARGEUR, HAUTEUR);

	// Selectionner la couleur selon l'état de la personne et ensuite on l'affiche
	for (int i = 0; i < max_pers_dep;i++) {
		coul = (liste_pers->liste[i].etat == MORT) ? COUL_MORT :
			(liste_pers->liste[i].etat == MALADE) ? COUL_MALADE : COUL_SANTE;
		afficher_cercle(liste_pers->liste[i].posy, liste_pers->liste[i].posx, coul);
	}

	// Pause, on attent le saisie d'une touche:
	while (touche_pesee() == 0) touche = obtenir_touche();
	

	// loop TANT QUE:  NB de malades > 0 ET que la touche pesée n’est pas ESC (= 27).
	do {
		nb_heure_simulation++;

		//	• Traiter toutes les personnes ???

		// On nettoye l'affichage:
		effacer_zone_environnement(LARGEUR, HAUTEUR);
		
		// itération pour le nombre de personne dans la simulation:
		for (int i = 0; i < max_pers_dep;i++) {
			// Selectionne la couleur selon l'état de la personne et ensuite on l'affiche
			coul = (liste_pers->liste[i].etat == MORT) ? COUL_MORT :
				   (liste_pers->liste[i].etat == MALADE) ? COUL_MALADE : COUL_SANTE;
			afficher_cercle(liste_pers->liste[i].posy, liste_pers->liste[i].posx, coul);

			// Récupérer le nombre de malades actuel:
			if (liste_pers->liste[i].etat == MALADE) nb_malades++;
		}

		// Ajuster le nombre maximal de malades si c’est le cas
		if (nb_malades > lemax) lemax = nb_malades;

		// crée un léger délais:
		delai_ecran(30 / FACTEUR);

		// Si le NB d’heures écoulé est un multiple de 24, on affiche les infos:
		if ((nb_heure_simulation % 24) == 0) {
			jours++; // Vue que ca fait 24h, donc une journnée c'est écoulé
			afficher_infos(jours, nb_morts, nb_malades, lemax);
		}

		// Si le NB d’heures écoulé est un multiple de 4, alors
		// on affiche une barre du graphique de progrssion:
		if ((nb_heure_simulation % 4) == 0) {
			afficher_graphe((nb_heure_simulation / 4),
							(double)(nb_malades / max_pers_dep),
							(double)(nb_malades / max_pers_dep));
		}

		// Si il y a saisie d'une touche, on la met en memoire:
		if (touche_pesee()) touche = obtenir_touche();

	} while (nb_malades > 0 && touche != 27);


	// On nettoye l'affichage:
	effacer_zone_environnement(LARGEUR, HAUTEUR);

	// itération pour le nombre de personne dans la simulation:
	for (int i = 0; i < max_pers_dep;i++) {
		// Selectionne la couleur selon l'état de la personne et ensuite on l'affiche
		coul = (liste_pers->liste[i].etat == MORT) ? COUL_MORT :
			(liste_pers->liste[i].etat == MALADE) ? COUL_MALADE : COUL_SANTE;
		afficher_cercle(liste_pers->liste[i].posy, liste_pers->liste[i].posx, coul);

		// Récupérer le nombre de malades actuel:
		if (liste_pers->liste[i].etat == MALADE) nb_malades++;
	}

	// Ajuster le nombre maximal de malades si c’est le cas
	if (nb_malades > lemax) lemax = nb_malades;

	// On affiche les info final:
	afficher_infos(jours, nb_morts, nb_malades, lemax);

	// Pause, on attent le saisie d'une touche:
	while (touche_pesee() == 0);

	// Fermer le mode graphique:
	fermer_mode_graphique();
	
	//===================================
	// Videz la liste de personnes:
	//=============================



	// Fin du main:
	printf("\n\n");
	system("pause");
	return EXIT_SUCCESS;
}




#else () // CODE sans affichage (1500 personne max) + logfile:
int main(void) {
	
	// Permets de générer des valeurs aléatoires.
	srand(time(NULL));

	// Initialiser le mode graphique et l’écran
	init_graphe(LARGEUR, HAUTEUR);
	init_zone_environnement(LARGEUR, HAUTEUR);

	/*======================================*/
	/*==IL FAUT VIDER LA LISTE DE PERSONNE==*/
	/*======================================*/










	// Fin du main:
	printf("\n\n");
	system("pause");
	return EXIT_SUCCESS;
}
#endif
