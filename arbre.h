#ifndef ARBRE_H
#define ARBRE_H

#include "noeud.h"
#include "jeu.h"
class arbre //class representant un plateau et les plateaux qui en decoule aux tours suivants
{
private:
    Plateau pInit;

    vector<arbre> suite;
    double note;

public:
    arbre()
    {
        note=0;

        for (int i=0;i<3;i++)
        {
            for (int j =0;j<3;j++)
            {
                pInit.setCoordonnees(i,j);
            }
        }
    }

    arbre(arbre const& a)
    {
        note=a.note;
        pInit=a.pInit;

        if(a.suite.size()!=0)
        {
            for (unsigned k=0;k<a.suite.size();k++)
            {
                this->suite.push_back(arbre(a.suite.at(k)));
            }
        }
    }

    void setNote(double n)
    {
        note=n;
    }

    double getNote()
    {
        return  note;
    }

    arbre getArbre(int place)
    {
        return suite.at(unsigned (place));
    }

    Plateau getPlateau(int place)
    {
        if (place==-1)
        {
            return pInit;
        }
        return suite.at(unsigned(place)).pInit;
    }

    void setPlateau(Plateau plateau)
    {
        pInit=plateau;
    }

    arbre& operator=(const arbre& a)
    {
        this->note=a.note;
        this->pInit=a.pInit;

        if (a.suite.size()!=0)
        {
            for (unsigned k=0;k<a.suite.size();k++)
            {
                this->suite.push_back(arbre(a.suite.at(k)));
            }
        }
        return *this;
    }

    int estVide()
    {
        return suite.empty();
    }

    int getTaille ()
    {
        int k=suite.size();
        return k;
    }

    noeud getNoeud(int x,int y)
    {
        noeud n;
        n=pInit.getNoeud(x,y);
        return n;
    }


    void ajouterArbre(arbre& a) //ajoute un plateau suivant (avec les plateau qui en decoulent)
    {
        arbre temp;
        temp.note=a.note;
        temp.pInit=a.pInit;
        unsigned taille=this->suite.size();
        this->suite.push_back(temp);

        if (a.suite.empty()!=1)
        {
            for (unsigned int k=0;k<a.suite.size();k++)
            {
                this->suite.at(taille).ajouterArbre(a.suite.at(k));
            }
        }
    }


    void affichage() //affiche l'arbre complet
    {
        cout<<"note: "<<this->note<<endl;
        if (suite.empty()==1)
        {
            cout<<" ->  ";
            pInit.affichage();
        }
        else {
            pInit.affichage();
            for(unsigned int k=0;k<suite.size();k++)
            {
                cout<<k<<endl;
                suite.at(k).affichage();
            }
        }
    }

    void affichageFin() //affiche les plateau final de l'arbre
    {
        if (suite.empty()==1)
        {
            cout<<" ->  ";
            pInit.affichage();
        }
        else {
            for(unsigned int k=0;k<suite.size();k++)
            {
                cout<<k<<endl;
                suite.at(k).affichage();
            }
        }
    }

    void deplacement(int xDepart,int yDepart,int xArrive, int yArrive, int nbJeton) //deplace n jeton d'une case à une autre
    {
        int nbJetonArrive=pInit.getNoeud(xArrive,yArrive).getNbJeton();
        int nbJetonDepart=pInit.getNoeud(xDepart,yDepart).getNbJeton();
        int position=0;
        int couleur=0;
        int i;

        for (i=0;i<nbJeton;i++)
        {
            couleur=pInit.getNoeud(xDepart,yDepart).getJeton(unsigned(nbJetonDepart-nbJeton+i)).getColor();
            position=nbJetonArrive+i+1;
            pInit.ajouterJeton(xArrive,yArrive,jeton(position,couleur));
        }
        for (i=0;i<nbJeton;i++)
        {
            pInit.retirer(xDepart,yDepart);
        }
    }

    void creationArbre(int profondeur,int tour,int monTour) //instatie tout l'arbre avec une profondeur définis
    {
        int nb=0;
        if (profondeur!=0)
        {
            for (int i=0;i<3;i++)//parcours du plateau
            {
                for (int j = 0; j < 3; ++j)
                {
                    if (this->pInit.getNoeud(i,j).getColor()==tour)//si le jeton au dessus de la pile est de la couleur du joueur
                    {
                        for (int I=0;I<3;I++)//alors on va reparcourir tout le plateau
                        {
                            for (int J=0;J<3;J++)
                            {
                                if (J!=j || I!=i)//si on changé de case
                                {
                                    for(int k=0; k<min(this->getNoeud(i,j).getNbJeton(),3)+1;k++)//on teste si il y a un deplacement possible sur les 3 premiers jetons
                                    {
                                        if (deplacementPossible(i,j,I,J,k,this->pInit,tour)==1)//si le deplacement est possible
                                        {
                                            arbre temp;//crée un arbre

                                            temp.setPlateau(this->pInit);//on crée un arbre

                                            temp.deplacement(i,j,I,J,k);//definis le nouveaux plateau comme le plateau actuel auquel on effectue un deplacement
                                            temp.setEvaluation(monTour);//on evalue le plateau

                                            this->ajouterArbre(temp);//on ajoute le plateau à l'arbre

                                            if(profondeur!=1)//puis on crée les arbres recursivement
                                            {
                                                this->suite.at(unsigned(nb)).creationArbre(profondeur-1,3-tour,monTour); //on crée l'arbre du pateau que l'on vient d'ajouter
                                            }
                                            nb++;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }

        }

    }

    void setEvaluation(int tour)// revois un nombre en 0 et 1, plus le nombre est grand plus la position est favorable au joueur (s'il est egal à 1000 c'est qu'eele est gagnante et si =0 est perdante)
    {

        double nbTourControle=0.0;
        double nbTourAdversaire=0.0;
        double nbJetonControle=0.0;
        for (int i=0;i<3;i++)
        {
            for (int j=0;j<3;j++)//parcours le plateau et augmente les 3 variables en fonction de ce qu'il y a sur la case
            {
                nbTourControle=nbTourControle+(this->pInit.getNoeud(i,j).getColor()==tour);
                nbTourAdversaire=nbTourAdversaire+(this->pInit.getNoeud(i,j).getColor()==3-tour);
                nbJetonControle=nbJetonControle+this->pInit.getNoeud(i,j).getNbJeton()*(this->pInit.getNoeud(i,j).getColor()==tour);
            }
        }
        double eval;
        //instancie éval comme la valeur de jeu (entre 0 et 1)

        eval=((nbTourControle/(2*nbTourAdversaire))/(nbTourControle+nbTourAdversaire)+(nbJetonControle/12));



        if(nbTourAdversaire==0)//pour empecher que eval=inf (car nbTourAdversaire peut etre égal à zéro
        {
            eval=1000;
        }
        this->note=eval;
    }

    void remonteEvaluation(int monTour,int profondeur)//applique l'algorythme min-max pour remonter toute les valeurs
    {
        if (suite.empty()!=1)//si l'arbre possede une suite alors on applique le min-max
        {
            for(unsigned int k=0;k<suite.size();k++)
            {
                if(suite.at(k).note==1000)//si la note est gagnante, elle mise en valeur par rapport aux autres notes gagnantes plus profondes
                {
                    suite.at(k).note=1000+profondeur;
                }

                suite.at(k).remonteEvaluation(1-monTour,profondeur-1);//on applique cette meme methode en recursif aux arbres suivants
            }

            for(unsigned int k=0;k<suite.size();k++)
            {
                suite.at(k).remonteEvaluation(1-monTour,profondeur-1);
            }

            note=suite.at(0).note;
            for(unsigned int k=0;k<suite.size();k++)
            {
                if (monTour)//si on regarde un coup jouer par nous on prends la valeur max
                {
                    note=max(note,suite.at(k).note);
                }
                else {//sinon on prend la min
                    note=min(note,suite.at(k).note);
                }
            }
        }
    }
};

#endif // ARBRE_H
