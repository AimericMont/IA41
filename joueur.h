#ifndef JOUEUR_H
#define JOUEUR_H


#include <vector>

class joueur
{
private:

    int couleur;//1=bleu, 2=rouge
    int listeNoeudControle[3][3];//0=pas controlé 1=controlé
    int level;

public:
    joueur(int Couleur,int lvl)
    {
        couleur=Couleur;
        level=lvl;
    }
    int getLevel()
    {
        return level;
    }
    int getGolor()
    {
        return couleur;
    }

    int getNoeud(int x,int y)
    {
        return listeNoeudControle[x][y];
    }

    void setListeNoeudControle(int x, int y,int controle)
    {
        listeNoeudControle[x][y]=controle;
    }

    void resetListeControle()
    {
        for (int i=0;i<3;i++)
        {
            for (int j=0;j<3;j++)
            {
                listeNoeudControle[i][j]=0;
            }
        }
    }
};

#endif // JOUEUR_H
