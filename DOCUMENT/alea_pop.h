/*==============================================================*/
// ALEA_POP.H 
// E.Th� & H.Saulnier, Ete2025 pour les etudiants en C
// Offre de diverses fonctions al�atoires pour le TP2 en INF147.
/*==============================================================*/
#ifndef __ALEAPOP_2025__
#define __ALEAPOP_2025__

/*
	Voici sept fonctions offertes pour r�aliser certains
	des exercices ou travaux  en simulations num�riques
	qui ont besoin d'une g�n�ration de nombres pseudo-al�atoires.

	Avant de savoir utiliser un bon generateur, ces fonctions
	interfacent le pauvre vieux g�n�rateur du C bien connu pour
	ses fonctions rand(), srand() et son obsolescence

	Trois fonctions randi(), randf() et randf_bornes() civilisent un peu
	ce vieux rand() et �mettent dans la distribution uniforme sp�cifi�e.

	Deux fonctions randz(), rand_normal() �mettent dans la distribution
	normale sp�cifi�e.

	La fonction rand_age_canada() donnera un age al�atoire � un canadien
	en respectant une distribution observ�e en 2018.

	srand_sys() rend bien plus simple l'initialisation de ce vieux machin
	en utilisant l'horloge interne de la machine.
*/

/*=========================================================*/
/*==========================================================*/
//permet de d�sactiver certains warnings de visual
#define _CRT_SECURE_NO_WARNINGS   
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>

#define EPSILON (0.00000001)	//pour comparer deux r�els

/*=========================================================*/
/* proportions observ�es dans la population canadienne 2018 (STATS CAN)
  selon l'age [0-9, 10-19, 20-29, 30-39, ..... , 80-89, >=90]

  Vous avez les proportions et sa cumulative :
*/
const double PROP_POPULATION[10] = 
             { 0.106, 0.106, 0.137, 0.14, 0.129, 0.14, 0.123, 0.077, 0.034, 0.008 };
const double PROP_CUMULATIVES[10] = 
             { 0.106, 0.212, 0.349, 0.489, 0.618 , 0.758, 0.881, 0.958, 0.992, 1.0 };


/*==========================================================*/
//  D�CLARATIONS DES FONCTIONS
/*==========================================================*/
/*  obtenir un age al�atoire pour un canadien en
	respectant la distribution de StatsCan 2018

PARAMETRE(s) : aucun

SORTIE : la valeur entiere obtenue du g�n�rateur

SPECS : aucune
*/
int rand_age_canada(void);

/*=========================================================*/
/*  srand_sys initialise le g�n�rateur de la valeur de l'horloge
	interne de la machine
	PARAMETRES :  Rien
	RETOUR : Rien

	SPECIFICATIONS : DANS UNE SIMULATION NUM�RIQUE, ON NE DOIT JAMAIS
		INITIALISER PLUS D'UNE FOIS UN G�N�RATEUR
*/
void srand_sys(void);

/*=========================================================*/
/*  randf donne un nombre r�el dans l'intervalle  [0,1]
	PARAMETRES :  aucun
	RETOUR : une valeur r�elle dans cet intervalle

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
/*  randf_bornes donne un nombre r�el dans l'intervalle sp�cifi�
	PARAMETRES :  deux r�els quelconques
	RETOUR : une valeur r�elle dans l'intervalle d�fini par ses
			ces deux valeurs

	SPECIFICATIONS : l'ordre des param�tres est sans importance
*/
double randf_bornes(double b1, double b2); 
#define  RANDF_BORNES(b1, b2)   ( (b1) + ( ((b2) - (b1)) * RANDF ))  //en MACRO

/*=========================================================*/

/* La fonction randz

L'�mission d'un nombre r�el dans la distribution normale Z,
la plus classique des normales,   de moyenne 0 et d'�cart type 1

PARAMETRE(s) : Aucun

SORTIE : la valeur r�elle �mise par le g�n�rateur

SPECS : celle-ci a une parit� dans l'�mission
ce qui peut d�concerter si on r�initialise le g�n�rateur
en cours d'ex�cution ( ce qui en r�alit� ne doit PAS �tre fait)
*/
double randz(void);

/*==========================================================*/
/* rand_normal emet un nombre r�el dans une distribution normale quelconque,
  de moyenne mu  et d'�cart-type sigma donn�s

PARAMETRE(s) : mu d�finit la moyenne  et sigma l'�cart-type

SORTIE : la valeur r�elle �mise par le g�n�rateur

SPECS : on traite sigma en valeur absolue n'a pas
*/
double rand_normal(double mu, double sigma);
#define RAND_NORMAL(MU,SIGMA)  	( (double)(mu) + (randz() * (double)(sigma)))  //en MACRO

/*=========================================================*/
#endif
