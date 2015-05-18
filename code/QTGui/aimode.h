#ifndef AIMODE_H
#define AIMODE_H
#include "initialdialog.h"
#include "lobbywindow.h"
#include <QDialog>
#include <cstdlib>
#include <iostream>

using namespace std;

namespace Ui {
class aiMode;
}

class aiMode : public QDialog
{
    Q_OBJECT

public:
    explicit aiMode(QWidget *parent = 0);
    ~aiMode();
    int Mode;

public slots:
    void handleEasy();
    void handleMedium();
    void handleHard();

private:
    Ui::aiMode *ui;
    int mode;
};

#endif // AIMODE_H
