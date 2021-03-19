#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "fenetre.h"
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->groupBox->setVisible(0);
    ui->groupBox_2->setVisible(0);
    setWindowTitle("Pogo");
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_radioButton_2_clicked()
{
   ui->groupBox->setVisible(1);

}


void MainWindow::on_radioButton_3_clicked()
{
    ui->groupBox_2->setVisible(1);
}

void MainWindow::on_radioButton_clicked()
{
    ui->groupBox->setVisible(0);
}

void MainWindow::on_radioButton_4_clicked()
{
    ui->groupBox_2->setVisible(0);
}


void MainWindow::on_pushButton_clicked()
{
    int lvlIa1,lvlIa2;

    if (ui->radioButton->isChecked())
    {
        lvlIa1=0;
    }else if (ui->radioButton_5->isChecked())
    {
        lvlIa1=1;
    }else if (ui->radioButton_7->isChecked())
    {
        lvlIa1=2;
    }  else
    {
        lvlIa1=3;
    }

    if (ui->radioButton_4->isChecked())
    {
        lvlIa2=0;
    }else if (ui->radioButton_8->isChecked())
    {
        lvlIa2=1;
    }else if (ui->radioButton_9->isChecked())
    {
        lvlIa2=2;
    }  else
    {
        lvlIa2=3;
    }

    this->hide();

    MaFenetre *fenetre =new MaFenetre(620,620,lvlIa1,lvlIa2);

    fenetre->lancementJeu();

    fenetre->show();
}

void MainWindow::on_pushButton_2_clicked()
{
    this->close();
}
