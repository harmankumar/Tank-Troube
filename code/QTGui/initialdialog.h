#ifndef INITIALDIALOG_H
#define INITIALDIALOG_H

#include "lobbywindow.h"
#include "aimode.h"
#include <QDialog>
#include <QInputDialog>
#include <QMessageBox>
#include <QDir>
#include <cstdlib>
#include <iostream>
#include <fstream>

using namespace std;
namespace Ui {
class initialDialog;
}

class initialDialog : public QDialog
{
    Q_OBJECT

public:
    explicit initialDialog(QWidget *parent = 0);
    ~initialDialog();
    int selfPort;
public slots:
    void startbeginGame();
    void joinGame();
private:
    Ui::initialDialog *ui;
};

#endif // INITIALDIALOG_H
