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
    
    connect(ui.actionOpen_session, &QAction::triggered, this, &Player::selectFolder);
    shapeReader();
}

void Player::pre_play()
{
    //read panzooms

}

void Player::selectFolder()
{
    addressFolder = QFileDialog::getExistingDirectory(this, tr("Open Directory"), QApplication::applicationDirPath() + "/Downloaded/",
        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

}

void Player::shapeReader()
{
    QString addr1 = "F:/c programer/winner 99.12/BigBlueButton Player/BigBlueButton Player/Downloaded/2515420_1399-12-06_15-13-09/shapes_2.svg";
    QFile file1;
    file1.setFileName(addr1);
    QByteArray shape;
    if (file1.open(QIODevice::ReadOnly))
    {
        for(int i=0;i<3;i++)
        /*qDebug() <<*/ file1.readLine();


        while (!file1.atEnd())
        {

            QByteArray temp = file1.readLine();
            if (temp.startsWith("  <image id=\""))
            {
                QString id = temp.mid(13, temp.indexOf("\" ") - 13);
                float in = temp.mid(temp.indexOf(" in=\"") + 5, temp.indexOf("\" ", temp.indexOf(" in=\"")) - temp.indexOf(" in=\"") - 5).toFloat();
                float out = temp.mid(temp.indexOf(" out=\"") + 6, temp.indexOf("\" ", temp.indexOf(" out=\"")) - temp.indexOf(" out=\"") - 6).toFloat();
                QString name = temp.mid(temp.indexOf("xlink:href=\"presentation/") + 25, temp.indexOf("\" ", temp.indexOf("xlink:href=\"presentation/")) - temp.indexOf("xlink:href=\"presentation/") - 25);
                int width = temp.mid(temp.indexOf(" width=\"") + 8, temp.indexOf("\" ", temp.indexOf(" width=\"")) - temp.indexOf(" width=\"") - 8).toInt();
                int height = temp.mid(temp.indexOf(" height=\"") + 9, temp.indexOf("\" ", temp.indexOf(" height=\"")) - temp.indexOf(" height=\"") - 9).toInt();
                int x = temp.mid(temp.indexOf(" x=\"") + 4, temp.indexOf("\" ", temp.indexOf(" x=\"")) - temp.indexOf(" x=\"") - 4).toInt();
                int y = temp.mid(temp.indexOf(" y=\"") + 4, temp.indexOf("\" ", temp.indexOf(" y=\"")) - temp.indexOf(" y=\"") - 4).toInt();
                qDebug() << "in " << in << " out:" << out;
            }
            else if (temp.startsWith("  <g class=\"canvas\""))
            {
                //do not any thing
            }
            else if (temp.startsWith("    <g id=\""))
            {
                QString imagenumber = temp.mid(11, temp.indexOf("-") - 11);
                QString timestamp = temp.mid(temp.indexOf(" timestamp=\"") + 12, temp.indexOf("\" ", temp.indexOf(" timestamp=\"")) - temp.indexOf(" timestamp=\"") - 12);
                QString undo = temp.mid(temp.indexOf(" undo=\"") + 7, temp.indexOf("\" ", temp.indexOf(" undo=\"")) - temp.indexOf(" undo=\"") - 7);
                QString style = temp.mid(temp.indexOf(" style=\"") + 8, temp.indexOf("\">", temp.indexOf(" style=\"")) - temp.indexOf(" style=\"") - 8);
                qDebug() <<"imagenumber:"<< imagenumber;
                QString temp2;
                while ((temp2 = file1.readLine()) != "    </g>\n")
                {
                    if (temp2 == "<switch>")
                        continue;
                    if (temp2.startsWith("        <foreignObject width=\""))
                    {
                        float width = temp2.mid(30, temp2.indexOf("\" ") - 30).toFloat();
                        float height = temp2.mid(temp2.indexOf(" height=\"") + 9, temp2.indexOf("\" ", temp2.indexOf(" height=\"")) - temp2.indexOf(" height=\"") - 9).toFloat();
                        float x = temp2.mid(temp2.indexOf(" x=\"") + 4, temp2.indexOf("\" ", temp2.indexOf(" x=\"")) - temp2.indexOf(" x=\"") - 4).toFloat();
                        float y; temp2.mid(temp2.indexOf(" y=\"") + 4, temp2.indexOf("\">", temp2.indexOf(" y=\"")) - temp2.indexOf(" y=\"") - 4).toFloat();
                        qDebug() << "with:" << width << " height:" << height;
                    }
                    else if (temp2.startsWith("          <p xmlns=\"http://www.w3.org/1999/xhtml\" "))
                    {
                        QString style = temp2.mid(temp2.indexOf(" style=\"") + 8, temp2.indexOf("\">", temp2.indexOf(" style=\"")) - temp2.indexOf(" style=\"") - 8);
                        if (temp2.contains("</p>")) {
                            QString p = temp2.mid(temp2.indexOf("\">") + 4, temp2.indexOf("</p>", temp2.indexOf("\">")) - temp2.indexOf("\">") - 4);
                            qDebug() << p;
                        }

                    }
                    else if (temp2.startsWith("      <path d=\""))
                    {
                        QString path = temp2.mid(15, temp2.indexOf("\"/>") - 15);
                        qDebug() << path;
                    }
                    else if (temp2.startsWith("      <circle cx=\""))
                    {
                        QString cx = temp2.mid(18, temp2.indexOf("\" ") - 18);
                        QString cy = temp2.mid(temp2.indexOf(" cy=\"") + 5, temp2.indexOf("\" ", temp2.indexOf(" cy=\"")) - temp2.indexOf(" cy=\"") - 5);
                        QString r = temp2.mid(temp2.indexOf(" r=\"") + 4, temp2.indexOf("\"/>", temp2.indexOf(" r=\"")) - temp2.indexOf(" r=\"") - 4);
                        qDebug() << "circle x:" << cx << " y:" << cy<<" r:"<<r;
                    }



                    //qDebug() << "finish";
                }




            }


        }

    }
    
}
