/*===========================================================*/
/* 	INF147 - TP2 */
/*===========================================================*/
/*
 Module : Algorithme.cpp
 Par    : Maxime Nebili + Guillaume Cadieux
 Date   : 13 Juillet 2025
 Info	: Module qui consiste � faire des simulations de pand�mie.
*/
/*=========================================================*/
// Directive qui permet de d�sactiver certains warnings de Visual-Studio
#define _CRT_SECURE_NO_WARNINGS

/*=========================================================*/
// Librairies usuelles � inclure:
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
// Dimensions de l'�cran d'affichage (en pixels) selon le FACTEUR du nombre de personnes
#define HAUTEUR (int)( 550 * sqrt(0.8 * FACTEUR))		// default: 550
#define LARGEUR (int)( 1200 * sqrt(0.8 * FACTEUR))		// default: 1200

#define NB_JOURS_SIMULATION 200 // Le NB de jours � simuler pour le log file
#define DELAY 1 // Le delay d'affiche � l'�cran
#define NOM_FICHIER	"logfile.txt" // Le nom du fichier cr��

// Macro pour que le syst�me ne plante pas � l'insertion de texte:
// Ins�rez l'instruction � FFLUSH(); � juste apr�s vos appels � � scanf() �.
#define FFLUSH() while (getchar() != '\n') {}


//=====================================================
//		       CODE PRINCIPAL			     
//=====================================================
int main(void) { 
	
	// Permet de g�n�rer des valeurs al�atoires:
	srand(time(NULL));

	// VARIABLES:
	int population = 0,				// le NB de personnes encore en vie
		nb_sante = 0,				// NB en bonne sant�
		jours = 0,					// NB de jours �coul�s
		nb_morts = 0,				// NB de morts
		nb_malades = 0,				// NB de malades
		lemax = 0,					// NB maximal de malades
		max_inf = 0,				// La valeur maximale d'infection sur une personne
		jamais_infectes = 0,		// NB de personnes jamais infect�es
		nb_heure_simulation = 0,	// Auto-explicatoire
		mode_mur = 0,				// S�lection du mode avec(1) ou sans(0) mur
		mode_affichage = 0,			// Variable qui d�terminera si on aura l'affichage
		nb_en_quarantiane = 0,		// NB de personne en quarantaine
		touche = NULL;				// Variable qui contient la touche press�e

	double	moyenne = 0,				// Init de la variable pour la moyenne
			prop_quarantaine = 0;	// G�n�ration de la prop_quarantaine

	// =========== TYPE DEF =============
	t_liste_personnes liste_pers = { 0 }; // Liste de personnes
	t_mur mur = { 0 }; //cr�e un type mur avec 0 comme param�tre
	FILE* fichier; // Pointeur vers le fichier � traiter


	// Demande la probabiliter de quarantaine a l'utilisateur:
	printf("Quelle sera la probabiliter de quarantaine ? [0.0 a 1.0]?");
	scanf("%lf", &prop_quarantaine);
	FFLUSH();

	// Demande a l'utilisateur pour un affichage:
	printf("Voulez-vous une simulation avec un affichage [oui(1) / non(0)]?");
	scanf("%d", &mode_affichage);
	FFLUSH();
	
	// Initialise le mur si l'utilisateur le souhaite:
	printf("Voulez-vous une simulation avec un mur [oui(1) / non(0)]?");
	scanf("%d", &mode_mur);
	FFLUSH();
	if (mode_mur) mur = init_mur(LARGEUR / 2, HAUTEUR);


	// + affichage des parametres pour info:
	printf("DELAY D'AFFICHAGE: %dms   ", DELAY);
	printf("NB DE PERSONNE: %d   ", MAX_PERS);
	printf("VITESSE DE DEPLACEMENT: %d   ", VITESSE);
	printf("\n");


	//===================================================================================
	// Si on veut avec affichage:
	if (mode_affichage) {

		// Initialiser le mode graphique et l'�cran
		init_graphe(LARGEUR, HAUTEUR);
		init_zone_environnement(LARGEUR, HAUTEUR);

		// Vidage de la liste
		vider_liste_personnne(&liste_pers);
		// Cr�ation d'une liste avec valeurs:
		init_liste_personnes(&liste_pers, MAX_PERS, prop_quarantaine,
			HAUTEUR, LARGEUR, mode_mur, &mur);

		// Initialisation du patient z�ro:
		patient_zero(&liste_pers);

		// Afficher les compteurs de d�part:
		afficher_infos(jours, nb_morts, nb_malades, lemax);

		// On nettoie l'affichage:
		effacer_zone_environnement(LARGEUR, HAUTEUR);

		// Dessiner les personnes:
		imprimer_pers(&liste_pers);

		// Afficher initialement le mur:
		if (mode_mur) dessiner_mur(&mur);

		// Pause, on attend la saisie d'une touche:
		while (touche_pesee() == 0);

		// ---------------------------------- LOOP --------------------------------------
		// TANT QUE: le NB de malades > 0 ET que la touche press�e n'est pas ESC (= 27):
		do {
			// On incr�mente le compteur d'heures:
			nb_heure_simulation++;

			// Si mode_mur = 1 ET �a fait 24h, on v�rifie si le mur est encore actif:
			if (mode_mur && (nb_heure_simulation % 24 == 0)) mode_mur = mur_actif(&mur);

			
			// Traiter toutes les personnes:
			nb_malades = traitement(&liste_pers, prop_quarantaine,
				HAUTEUR, LARGEUR, mode_mur, &mur);
			// Obtenir le nombre de morts dans la liste de personnes:
			nb_morts = get_nb_morts(&liste_pers);

			// Ajuster le nombre maximal de malades si c'est le cas
			if (nb_malades > lemax) lemax = nb_malades;

			// On nettoie l'affichage:
			effacer_zone_environnement(LARGEUR, HAUTEUR);
			//imprimer les personnes
			imprimer_pers(&liste_pers);
			// si le mur est toujours actif, on l'affiche:
			if (mode_mur) dessiner_mur(&mur);

			// Cr�e un d�lai, utilise une constante pour simplifier la modification:
			delai_ecran(DELAY / FACTEUR);

			// Si le NB d'heures �coul�es est un multiple de 24:
			if ((nb_heure_simulation % 24) == 0) {
				jours++; // Vu que �a fait 24h, donc une journ�e s'est �coul�e
				afficher_infos(jours, nb_morts, nb_malades, lemax);
			}

			// MODIFIER POUR VOIR LA PROPORTION DE LA POPULATION QUI EST EN QUARANTAINE:
			// Si le NB d'heures �coul�es est un multiple de 4, alors
			// on affiche une barre du graphique pour la progression:
			if ((nb_heure_simulation % 4) == 0) {
				// Obtenir le nombre de personne qui sont en quarantaine:
				nb_en_quarantiane = get_nb_quarantaine(&liste_pers);
				// Calculer les malades qui ne sont pas en quarantaine
				// Si nb_en_quarantiane > nb_malades, alors certains sains sont en quarantaine
				int nb_malades_libres = nb_malades - nb_en_quarantiane;
				int nb_sains_ajustes = liste_pers.nb_sante;

				// Ajuster si des sains sont en quarantaine (nb_malades_libres sera n�gatif)
				if (nb_malades_libres < 0) {
					// Il y a des sains en quarantaine
					nb_sains_ajustes = liste_pers.nb_sante + nb_malades_libres; // nb_malades_libres est n�gatif
					nb_malades_libres = 0; // Tous les malades sont en quarantaine
				}

				afficher_graphe((nb_heure_simulation / 4),
					(double)nb_malades_libres / MAX_PERS,  // Malades NON en quarantaine
					(double)nb_sains_ajustes / MAX_PERS,   // Sains NON en quarantaine (ajust�)
					(double)nb_en_quarantiane / MAX_PERS   // Tous ceux en quarantaine (malades + sains)
				);
			}

			// S'il y a saisie d'une touche, on la met en m�moire:
			if (touche_pesee()) touche = obtenir_touche();

		} while (nb_malades > 0 && touche != 27);
		// ------------------------------ END LOOP --------------------------------------

		//nettoyer la zone, imprimer les personnes et affichage des infos final:
		effacer_zone_environnement(LARGEUR, HAUTEUR);
		imprimer_pers(&liste_pers);
		// si le mur est toujours actif, on l'affiche:
		if (mode_mur) dessiner_mur(&mur);
		afficher_infos(jours, nb_morts, nb_malades, lemax);

		// Pause, on attend la saisie d'une touche:
		while (touche_pesee() == 0);

		// Fermer le mode graphique et vidage de la liste:
		fermer_mode_graphique();
		vider_liste_personnne(&liste_pers);
	}
	else{ // Si on ne veux pas l'affichage, alors on utilisera un LOGFILE:
		
		// On ouvre/cr�e le fichier en mode �criture texte (Write Text) et on v�rifie:
		printf("Creation du fichier\n"); // --- DEBUG ---
		if ((fichier = fopen(NOM_FICHIER, "wt")) == NULL) {
			printf("Impossible de cr�er le fichier %s.\n", NOM_FICHIER);
			return EXIT_FAILURE;
		}
		printf("NOM du fichier: %s \n", NOM_FICHIER);// --- DEBUG ---

		// On vide la liste:
		vider_liste_personnne(&liste_pers);
		// On initialise des valeurs dans la liste:
		init_liste_personnes(&liste_pers, MAX_PERS, prop_quarantaine,
			HAUTEUR, LARGEUR, mode_mur, &mur);
		// On infecte une personne dans la liste:
		patient_zero(&liste_pers);

		// �criture dans le fichier -> Les Titres 
		printf("Ecriture dans le fichier -> Les Titres\n");// --- DEBUG ---
		fprintf(fichier, "JOUR POPU MORTS SANTE MALADES MAX\n");

		// ---------------------------------- LOOP --------------------------------------
		// TANT QUE: le NB de malades > 0 ET on est en dessous du NB de jours � it�rer:
		do {
			// On incr�mente le compteur d'heures:
			nb_heure_simulation++;

			// Traiter toutes les personnes:
			nb_malades = traitement(&liste_pers, prop_quarantaine,
				HAUTEUR, LARGEUR, mode_mur, &mur);
			// Obtention des valeurs de la liste
			nb_morts = get_nb_morts(&liste_pers);
			nb_sante = get_nb_sante(&liste_pers);
			population = get_nb_personnes(&liste_pers);

			// Ajuster le nombre maximal de malades si c'est le cas
			if (nb_malades > lemax) lemax = nb_malades;

			// Si le NB d'heures �coul�es est un multiple de 24:
			if ((nb_heure_simulation % 24) == 0) {
				jours++; // Vu que �a fait 24h, donc une journ�e s'est �coul�e
				// �criture dans le fichier -> Les Donn�es:
				fprintf(fichier, "%4d %4d %5d %5d %7d %3d\n",
					jours, population, nb_morts, nb_sante, nb_malades, lemax);
				// --- DEBUG --- :
				printf("Ecriture dans le fichier, JOUR #: %d\n", jours);
			}
		} while (nb_malades > 0 && jours < NB_JOURS_SIMULATION);
		// ------------------------------ END LOOP --------------------------------------

		// Derni�re ligne du logfile:
		// Calcul de la moyenne et impression des donn�es:
		printf("Ecriture dans le fichier -> derni�re ligne\n");// --- DEBUG ---
		moyenne = nb_moyen_inf(&liste_pers, &max_inf, &jamais_infectes);
		fprintf(fichier, "\nNB. MOYEN d'INFECTIONS: %.2f (Max. %d, Jamais_infect�es = %d)\n",
			moyenne, max_inf, jamais_infectes);

		printf("FERMETURE DU FICHIER\n");// --- DEBUG ---
		// On ferme le fichier:
		fclose(fichier);
		// Vidage de la liste
		vider_liste_personnne(&liste_pers);
	}

	// Fin du main:
	printf("\n\n");
	system("pause");
	return EXIT_SUCCESS;	
}