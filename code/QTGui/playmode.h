#ifndef PLAYMODE_H
#define PLAYMODE_H

#include <QDialog>
#include "initialdialog.h"
#include "aimode.h"
namespace Ui {
class playMode;
}

class playMode : public QDialog
{
    Q_OBJECT

public:
    explicit playMode(QWidget *parent = 0);
    ~playMode();
public slots:
    int startNetwork();
    int startAi();
private:
    Ui::playMode *ui;
};

#endif // PLAYMODE_H
