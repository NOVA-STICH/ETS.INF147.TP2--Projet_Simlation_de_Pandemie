/*=========================================================*/
// ALEA_POP.H 
// E.Thé & H.Saulnier, Ete2025 pour les etudiants en C
/*=========================================================*/
#include "alea_pop.h"
/*=========================================================*/

/*=========================================================*/
// Définitions des fonctions :
/*=========================================================*/
// des six fonctions d'interface du générateur
/*=========================================================*/
double randf(void) {
	// calcul du rapport de la valeur issue de rand au maximum
	// emissible + epsilon réel 
	return rand() / ((1.0 + EPSILON) * RAND_MAX);
}

/*=========================================================*/
int randi(int n) {
	// si n<= 0 retour 0 sinon
	// remarquez le typecast (int)
	//(int)(n * randf()) donne un entier dans [0,n-1]
	// et avec +1, on l'amene dans [1,n]
	return (n <= 0) ? 0 : (int)(n * randf()) + 1;
}

/*=========================================================*/
double randf_bornes(double b1, double b2) {
	// de [0,1] vers l'intervalle désigné
	// maths : c'est une dilatation ((b2 - b1) * randf())
	// suivie d'une translation
	return b1 + ((b2 - b1) * randf());
}

/*==========================================================*/
/*	attention ici, ce code dépasse vos connaissance actuelles
	l'algo obtenu de la méthode de Marsaglia bien décrite sur wiki
*/
double randz(void) {
	static int test_regener = 1;
	static double v1;
	static double v2;
	static double w;
	static double facteur;
	double sortie;

	if (test_regener) {
		do {
			v1 = randf_bornes(-1, 1);
			v2 = randf_bornes(-1, 1);
			w = v1 * v1 + v2 * v2;

		} while (w >= 1);
		facteur = sqrt(-2 * log(w) / w);

		sortie = v1 * facteur;
	}
	else {
		sortie = v2 * facteur;
	}
	test_regener = (test_regener + 1) % 2;
	return sortie;
}

/*==========================================================*/
double rand_normal(double mu, double sigma) {
	return mu + randz() * sigma;
}

/*==========================================================*/
int rand_age_canada(void) {
	double prob_groupe = randf();
	int i = 0;

	for (i = 0; prob_groupe > PROP_CUMULATIVES[i]; ++i) {};

	return ((i * 10) - 1) + randi(10);
}

/*==========================================================*/
void srand_sys(void) {
	unsigned int secondes;

	// décalage secondes d'une date fixée (Unix timestamp)
	secondes = (unsigned int)time(NULL);
	// init du generateur
	srand(secondes);
	// trad.  suivi d'un premier appel au generateur
	secondes = rand(); // = un artifice stupide pour éviter un warning
}

/*=========================================================*/
/*=========================================================*/
