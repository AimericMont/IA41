#include <QApplication>
#include <QPushButton>

#include "fenetre.h"
#include <vector>
#include "mainwindow.h"


using namespace std;


int main(int argc, char *argv[])
{


    QApplication app(argc, argv);

    MainWindow *menu= new MainWindow ();
    menu->show();


    return app.exec();
}
