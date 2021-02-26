#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_Player.h"
#include "Download.h"

#include "qdebug.h"

class Player : public QMainWindow
{
    Q_OBJECT

public:
    Player(QWidget *parent = Q_NULLPTR);

private:
    Ui::PlayerClass ui;

public:
    QString addressFolder;




public slots:
    void pre_play();
    void selectFolder();
    void shapeReader();
};
