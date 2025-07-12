/*==============================================================*/
// ALEA_POP.H 
// E.Thé & H.Saulnier, Ete2025 pour les etudiants en C
// Offre de diverses fonctions aléatoires pour le TP2 en INF147.
/*==============================================================*/
#ifndef __ALEAPOP_2025__
#define __ALEAPOP_2025__

/*
	Voici sept fonctions offertes pour réaliser certains
	des exercices ou travaux  en simulations numériques
	qui ont besoin d'une génération de nombres pseudo-aléatoires.

	Avant de savoir utiliser un bon generateur, ces fonctions
	interfacent le pauvre vieux générateur du C bien connu pour
	ses fonctions rand(), srand() et son obsolescence

	Trois fonctions randi(), randf() et randf_bornes() civilisent un peu
	ce vieux rand() et émettent dans la distribution uniforme spécifiée.

	Deux fonctions randz(), rand_normal() émettent dans la distribution
	normale spécifiée.

	La fonction rand_age_canada() donnera un age aléatoire à un canadien
	en respectant une distribution observée en 2018.

	srand_sys() rend bien plus simple l'initialisation de ce vieux machin
	en utilisant l'horloge interne de la machine.
*/

/*=========================================================*/
/*==========================================================*/
//permet de désactiver certains warnings de visual
#define _CRT_SECURE_NO_WARNINGS   
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>

#define EPSILON (0.00000001)	//pour comparer deux réels

/*=========================================================*/
/* proportions observées dans la population canadienne 2018 (STATS CAN)
  selon l'age [0-9, 10-19, 20-29, 30-39, ..... , 80-89, >=90]

  Vous avez les proportions et sa cumulative :
*/
const double PROP_POPULATION[10] = 
             { 0.106, 0.106, 0.137, 0.14, 0.129, 0.14, 0.123, 0.077, 0.034, 0.008 };
const double PROP_CUMULATIVES[10] = 
             { 0.106, 0.212, 0.349, 0.489, 0.618 , 0.758, 0.881, 0.958, 0.992, 1.0 };


/*==========================================================*/
//  DÉCLARATIONS DES FONCTIONS
/*==========================================================*/
/*  obtenir un age aléatoire pour un canadien en
	respectant la distribution de StatsCan 2018

PARAMETRE(s) : aucun

SORTIE : la valeur entiere obtenue du générateur

SPECS : aucune
*/
int rand_age_canada(void);

/*=========================================================*/
/*  srand_sys initialise le générateur de la valeur de l'horloge
	interne de la machine
	PARAMETRES :  Rien
	RETOUR : Rien

	SPECIFICATIONS : DANS UNE SIMULATION NUMÉRIQUE, ON NE DOIT JAMAIS
		INITIALISER PLUS D'UNE FOIS UN GÉNÉRATEUR
*/
void srand_sys(void);

/*=========================================================*/
/*  randf donne un nombre réel dans l'intervalle  [0,1]
	PARAMETRES :  aucun
	RETOUR : une valeur réelle dans cet intervalle

	SPECIFICATIONS : aucune
*/
double randf(void);
#define RANDF (( rand() / ((1.0 + EPSILON) * RAND_MAX)))   //en MACRO

/*=========================================================*/
/*  randi donne un entier positif entre 1 et le parametre recu
	PARAMETRES :  nombre , un entier positif
	RETOUR : 0  ou  une valeur correcte entre 1 et n

	SPECIFICATIONS : recevant nombre <= 0 retour automatique de 0
*/
int randi(int nombre);
#define RANDI(n)    (((n) <= 0) ? 0 : (int)((n) * RANDF ) + 1)   //en MACRO

/*=========================================================*/
/*  randf_bornes donne un nombre réel dans l'intervalle spécifié
	PARAMETRES :  deux réels quelconques
	RETOUR : une valeur réelle dans l'intervalle défini par ses
			ces deux valeurs

	SPECIFICATIONS : l'ordre des paramètres est sans importance
*/
double randf_bornes(double b1, double b2); 
#define  RANDF_BORNES(b1, b2)   ( (b1) + ( ((b2) - (b1)) * RANDF ))  //en MACRO

/*=========================================================*/

/* La fonction randz

L'émission d'un nombre réel dans la distribution normale Z,
la plus classique des normales,   de moyenne 0 et d'écart type 1

PARAMETRE(s) : Aucun

SORTIE : la valeur réelle émise par le générateur

SPECS : celle-ci a une parité dans l'émission
ce qui peut déconcerter si on réinitialise le générateur
en cours d'exécution ( ce qui en réalité ne doit PAS être fait)
*/
double randz(void);

/*==========================================================*/
/* rand_normal emet un nombre réel dans une distribution normale quelconque,
  de moyenne mu  et d'écart-type sigma donnés

PARAMETRE(s) : mu définit la moyenne  et sigma l'écart-type

SORTIE : la valeur réelle émise par le générateur

SPECS : on traite sigma en valeur absolue n'a pas
*/
double rand_normal(double mu, double sigma);
#define RAND_NORMAL(MU,SIGMA)  	( (double)(mu) + (randz() * (double)(sigma)))  //en MACRO

/*=========================================================*/
#endif
