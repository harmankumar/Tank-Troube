#include "lobbywindow.h"
#include <QApplication>
#include"aimode.h"
#include"initialdialog.h"
#include"playmode.h"
#include <cstdlib>

using namespace std;

int main(int argc, char *argv[])
{
    ofstream fout;
    fout.open("out.txt");
    fout<<-1<<"\n"<<-1<<"\n"<<"-1"<<"\n"<<-1<<"\n"<<-1<<"\n"<<-1;
    fout.close();
    QApplication a(argc, argv);
    cout<<"started"<<endl;
    ///////getting userport//
  /*  bool ok;
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
        return 0;*/
    playMode * start=new playMode;
    start->show();


    return a.exec();
}



