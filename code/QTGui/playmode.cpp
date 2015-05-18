#include "playmode.h"
#include "ui_playmode.h"

playMode::playMode(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::playMode)
{
    ui->setupUi(this);
    connect(ui->startNet,SIGNAL(clicked()),this,SLOT(startNetwork()));
    connect(ui->startAi,SIGNAL(clicked()),this,SLOT(startAi()));
    ui->startNet->setStyleSheet("border-image: url(texture.jpeg);");
    ui->startAi->setStyleSheet("border-image: url(texture.jpeg);");
    this->setStyleSheet("background-image: url(texture1.jpg);");
}

playMode::~playMode()
{
    delete ui;
}


int playMode::startNetwork()
{
      this->close();
      bool ok;
      QInputDialog inport;


      QString Ip = inport.getText(NULL, QObject::tr("HOST Description"),
                                           QObject::tr("PORT No. :"), QLineEdit::Normal,
                                           QDir::home().dirName(),&ok);
      if(ok)
      {
      int selfPort=atoi(Ip.toStdString().c_str());

      initialDialog * infoWin = new initialDialog;
      infoWin->selfPort=selfPort;
      infoWin->show();
      }
      else
          return 0;

      return 0;

}

int playMode::startAi()
{
    this->close();
    //start AI take any parameters from user
    QMessageBox::information(this,tr("Message"),tr("Please select the Gameplay Mode ") );
    aiMode *modeWindow = new aiMode ;
    this->close();
    modeWindow->show();


    return 0;
}
