
#ifndef NOEUD_H
#define NOEUD_H
#include "jeton.h"
#include <iostream>

#include <vector>

using namespace std;



class noeud
{
private:

    int x;//coordonnées du noeud sur le plateau de 3*3 (commence a [0,0] jusqu'a [2,2]
    int y;

    vector<jeton> listeJeton;//liste des 12 jetons possible 0=pas de pion 1=bleu 2=rouge

public:


    noeud()
    {
    }

    void setCoordonnees(int X,int Y)
    {
        x=X;
        y=Y;
    }

    void ajouterJeton(jeton Jeton)//ajoute un jeton
    {
        listeJeton.push_back(Jeton);
    }

    noeud(noeud& n)
    {
        if (n.estVide()!=1)
        {
            for (int k=0;k<n.getNbJeton();k++)
            {
                this->ajouterJeton(n.getJeton(unsigned(k)));
            }
        }
    }

    jeton getJeton(unsigned place)
    {
        if (this->estVide())
        {
            return jeton(4,8);
        }
        jeton *temp=new jeton(listeJeton.at(place));
        return *temp;
    }

    void retirer()//retire le premier jeton de la pile
    {
        listeJeton.pop_back();
    }

    int getColor()//retourne la couleur du pion le plus haut du noeud
    {
        if (listeJeton.empty())
        {
            return 0;
        }
        return listeJeton.at(listeJeton.size()-1).getColor();
    }

    int getNbJeton()
    {
        return int(listeJeton.size());
    }

    int estVide()//retourne 1 si il n'y a pas de jeton sur le noeud
    {
        return listeJeton.empty();
    }

    noeud& operator=(const noeud &n)
    {
        this->x=n.x;
        this->y=n.y;
        this->listeJeton=n.listeJeton;

        return *this;
    }


};

class Plateau
{
private:
    noeud tableau[3][3];

public:

    Plateau()
    {
        for (int i=0;i<3;i++)
        {
            for (int j =0;j<3;j++)
            {
                tableau[i][j].setCoordonnees(i,j);
            }
        }
    }

    noeud getNoeud(int x,int y)
    {
        return tableau[x][y];
    }

    Plateau& operator=(const Plateau& p)
    {
        for (int i=0;i<3;i++)
        {
            for (int j=0;j<3;j++)
            {
                this->tableau[i][j]=p.tableau[i][j];
            }
        }
        return *this;
    }

    void ajouterJeton(int x, int y,jeton j)
    {
        tableau[x][y].ajouterJeton(j);
    }

    void retirer(int x,int y)
    {
        tableau[x][y].retirer();
    }

    void setCoordonnees(int X,int Y)
    {
        tableau[X][Y].setCoordonnees(X,Y);
    }

    void affichage()
    {
        for (int i=0;i<3;i++)
        {
            for (int j=0;j<3;j++)
            {
                cout<<"[";
                if (tableau[i][j].estVide()!=1)
                {
                    for (int k=0;k<tableau[i][j].getNbJeton();k++)
                    {
                        int color=tableau[i][j].getJeton(unsigned(k)).getColor();
                        cout<<color<<"-";
                    }
                }
                cout<<"] ";
            }
            cout<<endl;
        }
    }
};


#endif // NOEUD_H
