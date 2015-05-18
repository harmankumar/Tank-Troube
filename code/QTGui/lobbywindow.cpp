#include "lobbywindow.h"
#include "ui_lobbywindow.h"
#include <QFileDialog>
#include <QFileInfo>
#include <iostream>
#include <QDebug>
#include <QMessageBox>

LobbyWindow::LobbyWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::LobbyWindow)
{
    ui->setupUi(this);
    QPixmap pix("tanktrouble.jpg");
    ui->image->setPixmap(pix);

    connect(ui->game2    ,SIGNAL(clicked()),this,SLOT(start2Player()));
    connect(ui->game3    ,SIGNAL(clicked()),this,SLOT(start3Player()));
    connect(ui->game4    ,SIGNAL(clicked()),this,SLOT(start4Player()));
    connect(ui->gameN   ,SIGNAL(clicked()),this,SLOT(startN()));
    ui->game2->setStyleSheet("border-image: url(texture.jpeg);");
    ui->game3->setStyleSheet("border-image: url(texture.jpeg);");
    ui->game4->setStyleSheet("border-image: url(texture.jpeg);");
    ui->gameN->setStyleSheet("border-image: url(texture.jpeg);");
    this->setStyleSheet("background-image: url(texture1.jpg);");



}

void LobbyWindow::start2Player()
{
    // receive just 2 confirmations
    this->close();
    bool ok;
    QString Ip2;

    Ip2 = QInputDialog::getText(this, tr("SELF Description"),
                                         tr("IP Add :"), QLineEdit::Normal,
                                         QDir::home().dirName(), &ok);

    if(!ok)
         return;

    ofstream fout;
    fout.open("out.txt");
    fout<<1<<"\n"<<2<<"\n"<<Ip2.toStdString()<<"\n"<<this->selfPort<<"\n"<<"targetIp"<<"\n"<<0;
    fout.close();
    return;
    //initialiseStart(1,2,"",this->selfPort,"",0);
}

void LobbyWindow::start3Player()
{
  //receive just three
    this->close();
    cout<<this->selfPort<<endl;
    cout<<"what just happen"<<endl;

    bool ok;
    QString Ip2;

    Ip2 = QInputDialog::getText(this, tr("SELF Description"),
                                         tr("IP Add :"), QLineEdit::Normal,
                                         QDir::home().dirName(), &ok);

    if(!ok)
         return;

    ofstream fout;
    fout.open("out.txt");
    fout<<1<<"\n"<<3<<"\n"<<Ip2.toStdString()<<"\n"<<this->selfPort<<"\n"<<"targetIp"<<"\n"<<0;
    fout.close();
    return;
    //initialiseStart(1,3,"",this->selfPort,"",0);

}
 void LobbyWindow::start4Player()
{
    //
     this->close();
     bool ok;
     QString Ip2;

     Ip2 = QInputDialog::getText(this, tr("SELF Description"),
                                          tr("IP Add :"), QLineEdit::Normal,
                                          QDir::home().dirName(), &ok);

     if(!ok)
          return;

     ofstream fout;
     fout.open("out.txt");
     fout<<1<<"\n"<<4<<"\n"<<Ip2.toStdString()<<"\n"<<this->selfPort<<"\n"<<"targetIp"<<"\n"<<0;
     fout.close();
     return;
     //initialiseStart(1,4,"",this->selfPort,"",0);

 }
 void LobbyWindow::startN()
{
     //playMode=5;
     this->close();
     bool ok;
     QString Ip2;

     Ip2 = QInputDialog::getText(this, tr("SELF Description"),
                                          tr("IP Add :"), QLineEdit::Normal,
                                          QDir::home().dirName(), &ok);

     if(!ok)
          return;

     QString Num = QInputDialog::getText(this, tr("SELF Description"),
                                          tr("IP Add :"), QLineEdit::Normal,
                                          QDir::home().dirName(), &ok);

     if(!ok)
          return;

     ofstream fout;
     fout.open("out.txt");
     fout<<1<<"\n"<<Num.toInt()<<"\n"<<Ip2.toStdString()<<"\n"<<this->selfPort<<"\n"<<"targetIp"<<"\n"<<0;
     fout.close();
     return;

 }



LobbyWindow::~LobbyWindow()
{
    delete ui;
}
