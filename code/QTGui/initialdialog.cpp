#include "initialdialog.h"
#include "ui_initialdialog.h"


initialDialog::initialDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::initialDialog)
{
    ui->setupUi(this);
    connect(ui->joinGame,SIGNAL(clicked()),this,SLOT(joinGame()));
    connect(ui->initiateGame,SIGNAL(clicked()),this,SLOT(startbeginGame()));
    ui->joinGame->setStyleSheet("border-image: url(texture.jpeg);");
    ui->initiateGame->setStyleSheet("border-image: url(texture.jpeg);");
    this->setStyleSheet("background-image: url(texture1.jpg);");

}

initialDialog::~initialDialog()
{
    delete ui;
}


void initialDialog::startbeginGame()
{
   //send a ping to all users to start their game
    QMessageBox::information(this,tr("Message"),tr("initializing the game .") );

    LobbyWindow *optionWin = new LobbyWindow;
    optionWin->selfPort=this->selfPort;
    this->close();
    optionWin->show();

}
void initialDialog::joinGame()
{
   //send confirmation to the gamestarter
    QString Ip;
    bool ok;
    Ip = QInputDialog::getText(this, tr("Target Description"),
                                         tr("IP Add :"), QLineEdit::Normal,
                                         QDir::home().dirName(), &ok);
    if(!ok)
         return;

    QString Port;
    Port = QInputDialog::getText(this, tr("Target Description"),
                                         tr("PORT No. :"), QLineEdit::Normal,
                                         QDir::home().dirName(), &ok);
    if(!ok)
         return;

    QString Ip2;

    Ip2 = QInputDialog::getText(this, tr("SELF Description"),
                                         tr("IP Add :"), QLineEdit::Normal,
                                         QDir::home().dirName(), &ok);

    if(!ok)
         return;

    //call start with suitable arguments
    cout<<endl;
    cout<<Ip2.toStdString()<<"  "<<Ip.toStdString()<<" "<<Port.toInt()<<endl;
    this->close();
    ofstream fout;
    fout.open("out.txt");
    fout<<2<<"\n"<<0<<"\n"<<Ip2.toStdString()<<"\n"<<this->selfPort<<"\n"<<Ip.toStdString()<<"\n"<<Port.toInt();
    fout.close();

    return;

    //initialiseStart(2,0,Ip2.toStdString(),this->selfPort,Ip.toStdString(),Port.toInt());    //joining a game
}



