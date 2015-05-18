#ifndef PLAYMODE_H
#define PLAYMODE_H

#include <QDialog>

namespace Ui {
class playMode;
}

class playMode : public QDialog
{
    Q_OBJECT

public:
    explicit playMode(QWidget *parent = 0);
    ~playMode();

private:
    Ui::playMode *ui;
};

#endif // PLAYMODE_H
