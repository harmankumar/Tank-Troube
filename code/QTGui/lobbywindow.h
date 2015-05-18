#ifndef LOBBYWINDOW_H
#define LOBBYWINDOW_H
#include "aimode.h"
#include "initialdialog.h"
#include <cstdlib>
#include <QMainWindow>
#include <iostream>

using namespace std;

namespace Ui {
class LobbyWindow;
}

class LobbyWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit LobbyWindow(QWidget *parent = 0);
    ~LobbyWindow();
    int playMode;
    int selfPort;


public slots:
    void start2Player();
    void start3Player();
    void start4Player();    //Just put a limit on the number of extensions
    void startN();         //take input about modes

private:
    Ui::LobbyWindow *ui;
};

#endif // LOBBYWINDOW_H
