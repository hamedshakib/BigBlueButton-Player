#include "Player.h"

Player::Player(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);


    connect(ui.actionDownload_a_session, &QAction::triggered, this, [&]() {
                                                                        Download* download = new Download(this);
                                                                        download->setWindowFlag(Qt::WindowType::Dialog);
                                                                        download->setWindowFlag(Qt::WindowType::MSWindowsFixedSizeDialogHint);
                                                                        download->show();
                                                                         });
    

}


