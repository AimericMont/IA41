#ifndef JEU_H
#define JEU_H
#include <math.h>
#include "noeud.h"

#include <iostream>

int distance(int xDepart,int yDepart,int xArrive, int yArrive);

int deplacementPossible(int xDepart,int yDepart,int xArrive, int yArrive, int nbJeton,Plateau plateau,int tour);

void deplacement(int xDepart,int yDepart,int xArrive, int yArrive, int nbJeton,Plateau plateau);
#endif // JEU_H
