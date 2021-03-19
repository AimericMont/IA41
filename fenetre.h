#ifndef FENETRE_H
#define FENETRE_H
#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QLCDNumber>
#include <QLabel>
#include <QProgressBar>
#include <QLayout>
#include <QMouseEvent>

#include <QPixmap>
#include <QLabel>
#include "math.h"
#include <stdlib.h>
#include <QMessageBox>
#include <QDialog>
#include <QInputDialog>
//#include "noeud.h"
#include "joueur.h"
#include "ia.h"
#include "jeu.h"
#include <unistd.h>

//static Plateau plateau;//le plateau actuel

class MaFenetre : public QWidget // On hérite de QWidget (IMPORTANT)
{
    Q_OBJECT
private:

    QGridLayout *layout;        //permet l'affichage
    QLabel *imageCase[3][3];    //permet l'affichage des cases
    int selection[3][3];        //tableau representant les cases selectionnés 0=non selec 1=premiere case selec 2=deuxieme case select
    int nbclick;                //0si le joueur n'as pas joué si il a cliqué sur une case 2si il a cliqué sur 2 cases
    QLabel *imageJeton[12];     //image des jetons
    int tour;                   //couleur du tour
    joueur *j1,*j2;
    int finJeu;                 //=0 tant que le jeu est en cours =1si un joueur a gagné
    Plateau plateau;            //représente le plateau en cours

protected:

    void mousePressEvent(QMouseEvent* event) //detecte l'évenement du clique de la souris
    {
        click(event->x(),event->y());
    }


public:

    // *****************  méthode pour les joueur

    void victoire(joueur jou) //est appelé lorsqu'un joueur a gagné
    {
        finJeu=1;
        if (jou.getGolor()==2)
        {
            QMessageBox ::information(this,"fin du jeu",QString("victoire des bleus"));
        }
        else {
            QMessageBox ::information(this,"fin du jeu",QString("victoire des rouges"));
        }
        this->close();

    }

    void setTableauJoeur(joueur jou)
    {
        jou.resetListeControle();
        int defaite =1;
        for (int i=0;i<3;i++)
        {
            for (int j=0;j<3;j++)
            {
                if (jou.getGolor()==plateau.getNoeud(i,j).getColor())
                {
                    jou.setListeNoeudControle(i,j,1);
                    defaite=0;
                }
            }
        }
        if (defaite==1)
        {
            victoire(jou);
        }
    }

    void click(int X,int Y)//applique les evenements lors d'un clique
    {
        refreshImageCase();
        int x=0;
        int y=0;
        if( X<=206)
        {
            x=0;
        }
        else if (X>=412) {
            x=2;
        }
        else {
            x=1;
        }
        if( Y<=206)
        {
            y=0;
        }
        else if (Y>=412) {
            y=2;
        }
        else {
            y=1;
        }
        if (selection[x][y]==1)
        {
            selection[x][y]=0;
            nbclick=0;
            refreshImageCase();
        }
        else if (nbclick ==0) {
            if (plateau.getNoeud(x,y).getColor()==tour)
            {
                imageCase[x][y]->setPixmap(QPixmap(":/images/case.select.png"));
                selection[x][y]=1;
                nbclick=1;
            }
        }
        else {
            selection[x][y]=2;
            nbclick=2;
            demandeDeplacement();
        }
    }

    //          *************methode pour le deplacement *****************

    void deplacement(int xDepart,int yDepart,int xArrive, int yArrive, int nbJeton) //deplace n pions d'une case à une autre
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

    void initSelection() //initialise le tableau des cases cliquées
    {
        for(int i=0;i<3;i++)
        {
            for(int j=0;j<3;j++)
            {
                selection[i][j]=0;
            }
        }
    }

    void finTour() //est appelé à la fin d'un tour
    {
        initSelection();
        refreshImageCase();

        j1->resetListeControle();
        j2->resetListeControle();

        setTableauJoeur(*j1);
        setTableauJoeur(*j2);
        tour=3-tour;//si t=2->1 si t=1->2
        nbclick=0;
        affichage();
        this->repaint();


        if (tour==1 && finJeu==0 && j1->getLevel()!=0)//lance les tours de l'ia
        {
            sleep(1);//laisse 1 seconde de pause avant de jouer
            Plateau plateauIa;
            plateau=debutTourIa(plateau,tour,j1->getLevel());//lance les méthodes de l'ia
            affichage();//actualise l'affichage
            finTour();//fini le tour
        }
        if (tour==2 && finJeu==0 && j2->getLevel()!=0)
        {
            sleep(1);//laisse 1 seconde de pause avant de jouer
            Plateau plateauIa;
            plateau=debutTourIa(plateau,tour,j2->getLevel());
            affichage();
            finTour();
        }
    }


    void demandeDeplacement()//est appelé après 2 cliques du joueurs sur 2 cases différentes
    {
        int nbPionDeplacer;
        int xCase1=0,yCase1=0;//coordonnes premiere case
        int xCase2=0,yCase2=0;//coordonnes deuxieme case
        for(int i=0;i<3;i++)
        {
            for(int j=0;j<3;j++)
            {
                if (selection[i][j]==1)//recupère la premiere case selectionnée
                {
                    xCase1=i;
                    yCase1=j;
                }
                if (selection[i][j]==2)//recupère la deuxieme case selectionnée
                {
                    xCase2=i;
                    yCase2=j;
                }
            }
        }
        int dist=distance(xCase1,yCase1,xCase2,yCase2);//recupère la distance entre les 2 cases
        if(dist==2)//en deduit le nombre de jetons à deplacer
        {
            nbPionDeplacer=2;
        }
        else if (dist==3)
        {
            nbPionDeplacer=3;
        }
        else {
            if (plateau.getNoeud(xCase1,yCase1).getNbJeton()>=3)//demande si on veut deplacer 3 jetons ou 1 seul
            {
                nbPionDeplacer=demandeNbJeton();
            }
            else
            {
                nbPionDeplacer=1;
            }
        }
        if (deplacementPossible(xCase1,yCase1,xCase2,yCase2,nbPionDeplacer,plateau,tour)==1)
            //si le deplacement est possible , on le fait et on fini le tour
        {
            deplacement(xCase1,yCase1,xCase2,yCase2,nbPionDeplacer);
            finTour();
        }
        else {
            //sinon on affiche un message d'erreur et on recommence le tour
            nbclick=0;
            QMessageBox::information(this, "erreur", "Le deplacment n'est pas possible!");
            initSelection();
            affichage();
        }
    }

    int demandeNbJeton()//demande si on veut 3 pion ou 1
    {
        int nb=QInputDialog::getInt(this,"nb","combien voulez vous deplacer de jetons?");
        while (!(nb==1 || nb ==3))
        {
            nb=QInputDialog::getInt(this,"nb","veillez entrer un nombre en 1 et 3");
        }
        return nb;
    }


    //                              ***  methode pour l'affichage   ***

    void refreshImageCase()//affiche les cases carrées du plateau
    {
        for (int i=0;i<3;i++)
        {
            for(int j=0;j<3;j++)
            {
                imageCase[i][j]->setPixmap(QPixmap(":/images/case.png"));
            }
        }

    }

    void setImageCase()//initialise les images des cases
    {
        layout=new QGridLayout;
        layout->setVerticalSpacing(0);
        layout->setHorizontalSpacing(0);

        for (int i=0;i<3;i++)
        {
            for(int j=0;j<3;j++)
            {
                imageCase[i][j]=new QLabel;
                layout->addWidget(imageCase[i][j],j,i);
            }
        }
        setLayout(layout);
        refreshImageCase();
    }

    void affichage()       //rafraichi l'affichage (des pions +des cases) +affiche dans la console le plateau
    {
        int p=0;
        for (int i=0;i<3;i++)
        {
            for(int j=0;j<3;j++)
            {
                cout<<" [";
                if (plateau.getNoeud(i,j).estVide()==0)
                {
                    for (int k=0; k<plateau.getNoeud(i,j).getNbJeton(); k++)
                    {
                        int color=plateau.getNoeud(i,j).getJeton(unsigned(k)).getColor();
                        cout <<color<<"-";
                        if (plateau.getNoeud(i,j).getJeton(unsigned(k)).getColor()==1)//le jeton est bleu
                        {
                            this->imageJeton[p]->setPixmap(QPixmap(":/images/pion_bleu.png"));
                            p++;
                        }
                        else
                        {
                            this->imageJeton[p]->setPixmap(QPixmap(":/images/pion_rouge.png"));
                            p++;
                        }
                        this->imageJeton[p-1]->move(100+200*i,-50+200*j-20*(plateau.getNoeud(i,j).getJeton(unsigned(k)).getPosition()-1));//place les pions au bon endroit
                    }

                }
                cout<<"] ";

            }
            cout<<endl;
        }
        this->show();
        this->repaint();
    }
    void initPlateau()      //initialise les 12 jetons sur le plateau
    {
        for (int i=0;i<3;i++)//boucle permettant de fixer les coordonnées de chaque noeud du plateau
        {
            for (int j=0;j<3;++j)
            {

                plateau.getNoeud(i,j).setCoordonnees(i,j);
            }
        }
        plateau.ajouterJeton(0,0,(jeton(1,1)));          //les pions bleus
        plateau.ajouterJeton(0,0,(jeton(2,1)));
        plateau.ajouterJeton(1,0,(jeton(1,1)));
        plateau.ajouterJeton(1,0,(jeton(2,1)));
        plateau.ajouterJeton(2,0,(jeton(1,1)));
        plateau.ajouterJeton(2,0,(jeton(2,1)));

        plateau.ajouterJeton(0,2,(jeton(1,2)));          //les pions rouges
        plateau.ajouterJeton(0,2,(jeton(2,2)));
        plateau.ajouterJeton(1,2,(jeton(1,2)));
        plateau.ajouterJeton(1,2,(jeton(2,2)));
        plateau.ajouterJeton(2,2,(jeton(1,2)));
        plateau.ajouterJeton(2,2,(jeton(2,2)));
    }

    void lancementJeu() //debut du jeu
    {
       initPlateau();

       for (int i=0;i<12;++i)
        {
            imageJeton[i] = new QLabel();
            imageJeton[i]->setParent(this);
        }
        click(200,200);
        click(200,200);
        affichage();
        QMessageBox ::information(this,"Pogo",QString("Lancement du jeu"));
        if (j1->getLevel()!=0) //si le premier joueur est une ia , on lance son tour
        {
            Plateau plateauIa;
            plateau=debutTourIa(plateau,tour,j1->getLevel());

            finTour();
        }
    }

    //                                  *** constructeur ****
    MaFenetre(int h, int l,int levelIa1,int levelIa2)
    {

        finJeu=0;
        tour=1;
        nbclick=0;
        j1 = new joueur(1,levelIa1); //init les 2 joueurs
        j2 = new joueur(2,levelIa2);
        initSelection();
        setFixedSize(h, l);//fixe la taille de la fenetre
        setWindowTitle("Pogo");
        setImageCase();//est là pour l'affichage des bords noirs ou bleus quand la case est selectionnée
    }

    ~MaFenetre()
    {

    }
};      //fin de la classe



#endif // FENETRE_H
