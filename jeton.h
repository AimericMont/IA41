#ifndef JETON_H
#define JETON_H


class jeton
{
private:

    int position ;  //posision dans la pile de jeton
    int couleur;    //1=bleu 2=rouge

public:

    jeton(int Position , int Couleur)
    {
        position=Position;
        couleur=Couleur;
    }

    jeton (const jeton &j)
    {
        this->couleur=j.couleur;
        this->position=j.position;
    }

    int getColor()
    {
        return couleur;
    }

    int getPosition()
    {
        return position;
    }

    void setPosition(int Position)
    {
        this->position=Position;
    }
};

#endif // JETON_H
