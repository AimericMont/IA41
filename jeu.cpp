#include "jeu.h"
#include "joueur.h"
#include <QMessageBox>

int distance(int xDepart,int yDepart,int xArrive, int yArrive)//retourne la distance entre 2 cases
{
    return abs(xArrive-xDepart)+abs (yArrive-yDepart);
}

int deplacementPossible(int xDepart,int yDepart,int xArrive, int yArrive, int nbJeton,Plateau plateau,int tour)//retourne 1 si le deplacement est possible
{
    int nb=plateau.getNoeud(xDepart,yDepart).getNbJeton();

    int color=plateau.getNoeud(xDepart,yDepart).getColor();

    if (distance(xDepart,yDepart,xArrive,yArrive)!=0 && color==tour && nbJeton <= nb && (nbJeton == distance(xDepart,yDepart,xArrive,yArrive) || nbJeton == distance(xDepart,yDepart,xArrive,yArrive)+2))
    {
        return 1;
    }

    return 0;
}

void deplacement(int xDepart,int yDepart,int xArrive, int yArrive, int nbJeton,Plateau plateau)//deplace les jetons
{
    int nbJetonArrive=plateau.getNoeud(xArrive,yArrive).getNbJeton();
    int nbJetonDepart=plateau.getNoeud(xDepart,yDepart).getNbJeton();
    int position=0;
    int couleur=0;
    int i;

    for (i=0;i<nbJeton;i++)
    {
        couleur=plateau.getNoeud(xDepart,yDepart).getJeton(unsigned(nbJetonDepart-nbJeton+i)).getColor();

        position=nbJetonArrive+i+1;

        plateau.ajouterJeton(xArrive,yArrive,jeton(position,couleur));
    }
    for (i=0;i<nbJeton;i++)
    {
        plateau.retirer(xDepart,yDepart);
    }


}
