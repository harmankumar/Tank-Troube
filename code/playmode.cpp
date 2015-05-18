#include "playmode.h"
#include "ui_playmode.h"

playMode::playMode(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::playMode)
{
    ui->setupUi(this);
}

playMode::~playMode()
{
    delete ui;
}
