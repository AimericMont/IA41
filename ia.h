#ifndef IA_H
#define IA_H

#include "arbre.h"
#include "jeu.h"


//float Evaluation(Plateau p, int tour);

Plateau debutTourIa(Plateau plateauInit, int tour, int levelIa);

Plateau choixPlateau(arbre& a);


#endif // IA_H
