/*===========================================================*/
/* 	INF147 - TP2 */
/*===========================================================*/
/*
 Module : Algorithme.cpp
 Par    : Maxime Nebili + Guillaum Cadieux
 Date   : 12 Juillet 2025
 Info	: Module qui consiste à faire des simulations de pandemi.
*/
/*=========================================================*/
// Directive qui permet de désactiver certains warnings de Visual-Studio
#define _CRT_SECURE_NO_WARNINGS

/*=========================================================*/
// Librairies usuelles à inclure:
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>
#include<assert.h>
#include"alea_pop.h"
#include"utilitaire_affichage.h"
#include"winBGIm.h"
#include"liste_personnes.h"

/*===================== CONSTANTES =======================*/
// Dimensions de l'écran d'affichage (en pixels) selon le FACTEUR du nombre de personnes
#define HAUTEUR (int)( 1000 * sqrt(0.8 * FACTEUR))		// default: 550
#define LARGEUR (int)( 2000 * sqrt(0.8 * FACTEUR))		// default: 1200

#define MODE_AFFICHAGE 0	// Simuler avec l’affichage (1) ou avec logfile(0)

#define NB_JOURS_SIMULATION 100 // Le NB de jours a simuler pour le log file
#define NOM_FICHIER	"logfile.txt" // Le nom du ficher cree


/*===================== MACRO =======================*/
// Macro pour que le system ne plante pas a l'insertion de texte.
// Insérez l’instruction « FFLUSH(); » juste après vos appels à « scanf() ».
#define FFLUSH() while (getchar() != '\n') {}



//=====================================================
//		       CODE PRINCIPALE			     
//=====================================================


#if (MODE_AFFICHAGE) 
int main(void) { // CODE avec afficahge (900 personne max):
	
	// Permets de générer des valeurs aléatoires:
	srand(time(NULL));

	// VARIABLES:
	int max_pers_dep = 900, // Le NB maxiamle de personne en deplacement
		jours = 0,		// NB de jours ecoule
		nb_morts = 0,	// NB de mort
		nb_malades = 0,	// NB de malade
		lemax = 0,		// NB maximal de malade
		touche = NULL,	// Variable que contien la touche pesée
		nb_heure_simulation = 0,	// Auto-explicatoire
		mode_mur = 0; // Selection du mode avec(1) ou sans(0) mur

	// Generation de la prop_quarantaine
	double prop_quarantaine = RANDF;

	// === TYPE DEF ===
	t_mur mur = { 0 }; //cree un type mur avec 0 comme parametre
	t_liste_personnes liste_pers = {0}; // Liste de personne

	// Initialise le mur si l'utilisateur le veuille:
	printf("Voulez-vous une simulation avec un mur(0/1)?");
	scanf_s("%d", &mode_mur, sizeof(mode_mur));
	FFLUSH();
	if (mode_mur) mur = init_mur(LARGEUR / 2, HAUTEUR);

// --------------------------------------------------------------------------------------
	// Initialiser le mode graphique et l’écran
	init_graphe(LARGEUR, HAUTEUR);
	init_zone_environnement(LARGEUR, HAUTEUR);

	// Vidage de la liste
	vider_liste_personnne(&liste_pers);
	// Creation d'une liste avec valeur:
	int_liste_personnes(&liste_pers, max_pers_dep, prop_quarantaine,
						HAUTEUR, LARGEUR, mode_mur, &mur);

	// Initalisation du patien zero:
	patient_zero(&liste_pers);

	// Afficher les compteurs de depart:
	afficher_infos(jours, nb_morts, nb_malades, lemax);

	// On nettoye l'affichage:
	effacer_zone_environnement(LARGEUR, HAUTEUR);

	// Dessiner les personnes:
	imprimer_pers(&liste_pers);

	// Afficher initalement le mur:
	if (mode_mur) dessiner_mur(&mur);
	
	// Pause, on attent le saisie d'une touche:
	while (touche_pesee() == 0);
	//while (touche == NULL) if (touche_pesee()) touche = obtenir_touche();

	
	// loop TANT QUE: le NB de malades > 0 ET que la touche pesée n’est pas ESC (= 27).
	do {
		// On incremente le compteur d'heures:
		nb_heure_simulation++;

		// Si mode_mur = 1 ET ca fait 24h, on vérifie si le mur est encore actif:
		if (mode_mur && (nb_heure_simulation % 24 == 0)) mode_mur = mur_actif(&mur);
		
		// Obtenir le nombre de morts dans la liste de personnes:
		nb_morts = get_nb_morts(&liste_pers);
		// Traiter toutes les personnes:
		nb_malades = traitemnt(&liste_pers, prop_quarantaine,
								HAUTEUR, LARGEUR, mode_mur, &mur);
		
		// Ajuster le nombre maximal de malades si c’est le cas
		if (nb_malades > lemax) lemax = nb_malades;
		
		// On nettoye l'affichage:
		effacer_zone_environnement(LARGEUR, HAUTEUR);
		//imprimer les personnes
		imprimer_pers(&liste_pers);
		// si le mur est toujours actif, on l'affiche:
		if (mode_mur) dessiner_mur(&mur);

		// crée un léger délais:
		//delai_ecran(30 / FACTEUR);


		// Si le NB d’heures écoulé est un multiple de 24:
		if ((nb_heure_simulation % 24) == 0) {
			jours++; // Vue que ca fait 24h, donc une journnée c'est écoulé
			afficher_infos(jours, nb_morts, nb_malades, lemax);
		}

		// Si le NB d’heures écoulé est un multiple de 4, alors
		// on affiche une barre du graphique pour la progression:
		if ((nb_heure_simulation % 4) == 0) {
			afficher_graphe((nb_heure_simulation / 4),
							(double)nb_malades / max_pers_dep,
							(double)liste_pers.nb_sante / max_pers_dep);
		}

		// Si il y a saisie d'une touche, on la met en memoire:
		if (touche_pesee()) touche = obtenir_touche();

	} while (nb_malades > 0 && touche != 27);
	
	//nettoyer la zone, imprimer les personnes et affichage des infos final:
	effacer_zone_environnement(LARGEUR, HAUTEUR);
	imprimer_pers(&liste_pers);
	// si le mur est toujours actif, on l'affiche:
	if (mode_mur) dessiner_mur(&mur);
	afficher_infos(jours, nb_morts, nb_malades, lemax);

	// Pause, on attent le saisie d'une touche:
	while (touche_pesee() == 0);

	// Fermer le mode graphique et vidage de la liste:
	fermer_mode_graphique();
	vider_liste_personnne(&liste_pers);
	
	// Fin du main:
	printf("\n\n");
	system("pause");
	return EXIT_SUCCESS;
}
/* =================================================================================== */
#else 
int main(void) { // CODE sans affichage (1500 personne max) + logfile:
	
	// Permets de générer des valeurs aléatoires.
	srand(time(NULL));
	
	// VARIABLES:
	int max_pers_dep = 1500,		// Le NB maxiamle de personne en deplacement
		population = 0,				// le NB de personne encore en vie
		nb_sante = 0,				// NB en sante
		jours = 0,					// NB de jours ecoule
		nb_morts = 0,				// NB de mort
		nb_malades = 0,				// NB de malade
		lemax = 0,					// NB maximal de malade
		max_inf = 0,				// La veleur maximal d'infection sur une personne
		jamais_infectes = 0,		// NB de personne jamais infecter
		nb_heure_simulation = 0,	// Auto-explicatoire
		mode_mur = 0;				// Selection du mode avec(1) ou sans(0) mur

	// Initalisation de la moyenne et generation de la prop_quarantaine:
	double moyenne = 0,
		   prop_quarantaine = RANDF;

	// === TYPE DEF ===
	t_mur mur = { 0 }; //cree un type mur avec 0 comme parametre
	t_liste_personnes liste_pers = { 0 }; // Liste de personne
	FILE* fichier;	// Pointeur vers le fichier a traiter

	// Initialise le mur si l'utilisateur le veuille:
	printf("Voulez-vous une simulation avec un mur(0/1)?");
	scanf("%d", &mode_mur);
	FFLUSH();
	if (mode_mur) mur = init_mur(LARGEUR / 2, HAUTEUR);

	// On ouvre/cree le fichier en mode ecriture texte (Write Text) et on verifie:
	printf("Creation du ficher\n"); // --- DEBUG ---
	if ((fichier = fopen(NOM_FICHIER, "wt")) == NULL) {
		printf("Impossible de creer le fichier %s.\n", NOM_FICHIER);
		return EXIT_FAILURE;
	}
	printf("NOM du ficher: %s \n", NOM_FICHIER);// --- DEBUG ---

	// On vide la liste:
	vider_liste_personnne(&liste_pers);
	// On initialise des valeurs dans la liste:
	int_liste_personnes(&liste_pers, max_pers_dep, prop_quarantaine,
		HAUTEUR, LARGEUR, mode_mur, &mur);
	// On infecte une personne dans la liste:
	patient_zero(&liste_pers);

	// Ecriture dans le ficher -> Les Titre 
	printf("Ecriture dans le ficher -> Les Titre\n");// --- DEBUG ---
	fprintf(fichier, "JOUR POPU MORTS SANTE MALADES MAX\n");
	
	// ---------------------------------- LOOP ------------------------------------------
	// TANT QUE: le NB de malades > 0 ET on est en dessou du NB de jours a iterer:
	do {
		// On incremente le compteur d'heures:
		nb_heure_simulation++;
		// Traiter toutes les personnes:
		nb_malades = traitemnt(&liste_pers, prop_quarantaine,
								HAUTEUR, LARGEUR, mode_mur, &mur);
		// Obtention des valeurs de la liste
		nb_morts = get_nb_morts(&liste_pers);
		nb_sante = get_nb_sante(&liste_pers);
		population = get_nb_personnes(&liste_pers);

		// Ajuster le nombre maximal de malades si c’est le cas
		if (nb_malades > lemax) lemax = nb_malades;

		// Si le NB d’heures écoulé est un multiple de 24:
		if ((nb_heure_simulation % 24) == 0) {
			jours++; // Vue que ca fait 24h, donc une journnée c'est écoulé
			// Ecriture dans le ficher -> Les Donnees:
			printf("Ecriture dans le ficher -> Les Donnees\n");// --- DEBUG ---
			fprintf(fichier, "%4d %4d %5d %5d %7d %3d\n",
				jours, population, nb_morts, nb_sante, nb_malades, lemax);
		}
	} while (nb_malades > 0 && jours < NB_JOURS_SIMULATION);
	// ------------------------------ END LOOP ------------------------------------------
	
	// Derniere ligne du logfile:
	// Calcule de la moyenne et impresion des donnees:
	printf("Ecriture dans le ficher -> derniere ligne\n");// --- DEBUG ---
	moyenne = nb_moyen_inf(&liste_pers, &max_inf, &jamais_infectes);
	fprintf(fichier, "\nNB. MOYEN d'INFECTIONS: %.3f (Max. %d, Jamais_infecter = %d)\n",
			moyenne, max_inf, jamais_infectes);
	
	// On ferme le fichier:
	fclose(fichier);
	// Vidage de la liste
	vider_liste_personnne(&liste_pers);

	// Fin du main:
	printf("\n\n");
	system("pause");
	return EXIT_SUCCESS;
}
#endif
