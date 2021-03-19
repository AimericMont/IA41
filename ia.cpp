#include "ia.h"

using namespace std;


Plateau choixPlateau(arbre& a)//retourne le plateau avec la valeur de jeu la plus élevée
{
    Plateau p;
    double note=-1;

    for (int k=0;k<a.getTaille();k++)
    {
        if(a.getArbre(k).getNote()>note)
        {
            note=a.getArbre(k).getNote();
            p=a.getPlateau(k);
            p.affichage();
        }
    }
    return p;
}

Plateau debutTourIa(Plateau plateauInit,int tour, int levelIa)//déroulement du tour de l'ia
{
    Plateau plateauChoisi;
    arbre AInit;

    AInit.setPlateau(plateauInit);
    AInit.creationArbre(levelIa,tour,tour);//création de l'arbre des positions possibles

    AInit.remonteEvaluation(1,levelIa);//algo min-max

    plateauChoisi=choixPlateau(AInit);
    plateauChoisi.affichage();
    return  plateauChoisi;
}

