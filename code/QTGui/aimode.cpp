#include "aimode.h"
#include "ui_aimode.h"

aiMode::aiMode(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::aiMode)
{
    ui->setupUi(this);
    connect(ui->easyMode  ,SIGNAL(clicked()),this,SLOT(handleEasy()));
    connect(ui->mediumMode,SIGNAL(clicked()),this,SLOT(handleMedium()));
    connect(ui->hardMode  ,SIGNAL(clicked()),this,SLOT(handleHard()));
    ui->easyMode->setStyleSheet("border-image: url(texture.jpeg); color: white;");
    ui->mediumMode->setStyleSheet("border-image: url(texture.jpeg);color: white;");
    ui->hardMode->setStyleSheet("border-image: url(texture.jpeg);color: white;");
    this->setStyleSheet("background-image: url(texture1.jpg);");

}

void aiMode::handleEasy()
{
    fstream fout;
    fout.open("out.txt");
    fout<<1<<"\n"<<0<<"\n"<<"AI"<<"\n"<<0<<"\n"<<"AI"<<"\n"<<0;
    fout.close();

    //system("cd ..");
    //system("cd AI");

    //system("./tankTrouble");
    this->close();

}
void aiMode::handleMedium()
{
    ofstream fout;
    fout.open("out.txt");
    fout<<2<<"\n"<<0<<"\n"<<"AI"<<"\n"<<0<<"\n"<<"AI"<<"\n"<<0;
    fout.close();

    this->close();

}
void aiMode::handleHard()
{
    ofstream fout;
    fout.open("out.txt");
    fout<<3<<"\n"<<0<<"\n"<<"AI"<<"\n"<<0<<"\n"<<"AI"<<"\n"<<0;
    fout.close();

    this->close();

}

aiMode::~aiMode()
{
    delete ui;
}
