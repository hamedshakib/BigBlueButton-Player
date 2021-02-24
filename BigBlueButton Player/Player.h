#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_Player.h"

class Player : public QMainWindow
{
    Q_OBJECT

public:
    Player(QWidget *parent = Q_NULLPTR);

private:
    Ui::PlayerClass ui;
};
