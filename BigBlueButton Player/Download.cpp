#include "Download.h"

Download::Download(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
    add_hexTounicode();
    showDialog();
    connect(Download_btn, &QPushButton::clicked, this, &Download::download_clicked);
    connect(Cancel_btn, &QPushButton::clicked, this, [&](bool) {this->close(); });
}

Download::~Download()
{

}

void Download::showDialog()
{
    this->resize(635, 90);
    gridLayout_2 = new QGridLayout(this);
    gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
    gridLayout = new QGridLayout();
    gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
    Cancel_btn = new QPushButton(this);
    Cancel_btn->setObjectName(QString::fromUtf8("pushButton_2"));

    gridLayout->addWidget(Cancel_btn, 1, 3, 1, 1);

    Download_btn = new QPushButton(this);
    Download_btn->setObjectName(QString::fromUtf8("pushButton"));

    gridLayout->addWidget(Download_btn, 1, 2, 1, 1);

    label = new QLabel(this);
    label->setObjectName(QString::fromUtf8("label"));

    gridLayout->addWidget(label, 0, 0, 1, 1);

    downloadAll_checkBox = new QCheckBox(this);
    downloadAll_checkBox->setObjectName(QString::fromUtf8("checkBox"));

    gridLayout->addWidget(downloadAll_checkBox, 1, 0, 1, 1);

    horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    gridLayout->addItem(horizontalSpacer, 1, 1, 1, 1);

    Url_lineedit = new QLineEdit(this);
    Url_lineedit->setObjectName(QString::fromUtf8("lineEdit"));

    gridLayout->addWidget(Url_lineedit, 0, 1, 1, 3);


    gridLayout_2->addLayout(gridLayout, 0, 0, 1, 1);

    this->setWindowTitle(QCoreApplication::translate("dialog", "Form", nullptr));
    Cancel_btn->setText(QCoreApplication::translate("dialog", "Cancel", nullptr));
    Download_btn->setText(QCoreApplication::translate("dialog", "Download", nullptr));
    label->setText(QCoreApplication::translate("dialog", "URL: ", nullptr));
    downloadAll_checkBox->setText(QCoreApplication::translate("dialog", "Download All slides", nullptr));
}




void Download::showProgressbar()
{
    delete Cancel_btn;
    delete Download_btn;
    delete label;
    delete downloadAll_checkBox;
    delete Url_lineedit;


    delete gridLayout;
    delete gridLayout_2;


    this->resize(629, 73);
    gridLayout_12 = new QGridLayout(this);
    gridLayout_12->setObjectName(QString::fromUtf8("gridLayout_2"));
    gridLayout10 = new QGridLayout();
    gridLayout10->setObjectName(QString::fromUtf8("gridLayout"));
    label_12 = new QLabel(this);
    label_12->setObjectName(QString::fromUtf8("label_2"));

    gridLayout10->addWidget(label_12, 1, 0, 1, 1);

    DownloadingLabel = new QLabel(this);
    DownloadingLabel->setObjectName(QString::fromUtf8("label"));

    gridLayout10->addWidget(DownloadingLabel, 0, 0, 1, 1);
    horizontalSpacer10 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    gridLayout10->addItem(horizontalSpacer10, 1, 1, 1, 1);

    CancelDownload_btn = new QPushButton(this);
    CancelDownload_btn->setObjectName(QString::fromUtf8("pushButton"));

    gridLayout10->addWidget(CancelDownload_btn, 1, 2, 1, 1);
    label_13 = new QLabel(this);
    label_13->setObjectName(QString::fromUtf8("label_3"));
    label_13->setMinimumSize(QSize(25, 0));

    gridLayout10->addWidget(label_13, 1, 3, 1, 1);
    progressBar10 = new QProgressBar(this);
    progressBar10->setObjectName(QString::fromUtf8("progressBar"));
    progressBar10->setValue(0);


    gridLayout10->addWidget(progressBar10, 0, 1, 1, 3);
    label_12->setText(QString());
    DownloadingLabel->setText(QCoreApplication::translate("progressbar", "Downloading: ", nullptr));
    CancelDownload_btn->setText(QCoreApplication::translate("progressbar", "Cancel", nullptr));
    label_13->setText(QString());

    gridLayout_12->addLayout(gridLayout10, 0, 1, 1, 1);
}


void Download::download_clicked(bool is_clicked)
{

    if (Url_lineedit->text().isEmpty())
        return;

    if (downloadAll_checkBox->isChecked())
        download_All_slide = true;

    mainUrl = Url_lineedit->text();
    pre_download();
}

void Download::pre_download()
{
    str_Url = mainUrl.toString();
    if(str_Url.indexOf("?", 0)!=-1)
    meetingID = str_Url.mid(str_Url.indexOf("/2.3/", 0) + 5, str_Url.length() - str_Url.indexOf("?", 0) - 11);
    else
        meetingID = str_Url.mid(str_Url.indexOf("/2.3/", 0) + 5, str_Url.length()- str_Url.indexOf("/2.3/", 0)-5);
    //initial define
    QNetworkRequest request1;
    QNetworkAccessManager* manager1 = new QNetworkAccessManager(this);
    QNetworkReply* replay1;
    QEventLoop eventLoop;
    QObject::connect(manager1, &QNetworkAccessManager::finished, &eventLoop, &QEventLoop::quit);


/////////////////////////////////
    QString MetaData = "https://blue.aut.ac.ir/presentation/" + meetingID + "/metadata.xml";
    request1.setUrl(MetaData);
    replay1 = manager1->get(request1);
    eventLoop.exec();
    metadata = replay1->readAll();
    metadata = convertTounicode(metadata);
    qint64 srarttime = metadata.mid(metadata.indexOf("<start_time>", 0) + 12, metadata.indexOf("</start_time>", 0) - metadata.indexOf("<start_time>", 0) - 12).toULongLong();
    QString className = metadata.mid(metadata.indexOf("<bbb-context-label>", 0) + 18, metadata.indexOf("</bbb-context-label>", 0) - metadata.indexOf("<bbb-context-label>", 0) - 18);
    QString classId=className.mid(6, 8).replace(" ","");

    QCalendar cal(QLatin1String("jalali"));
    QDateTime datetime1;
    datetime1 = QDateTime().fromMSecsSinceEpoch(srarttime);
    QString date_time_start = datetime1.toString("yyyy-MM-dd_hh-mm-ss", cal);
    NameFolder = classId + "_" + date_time_start;


    if (!QDir("Downloaded/" + NameFolder).exists())
    {
        QDir().mkdir("Downloaded/");
        QDir().mkdir("Downloaded/" + NameFolder);
    }


    QString ChatUrl = "https://blue.aut.ac.ir/presentation/" + meetingID + "/slides_new.xml";
    request1.setUrl(ChatUrl);
    replay1 = manager1->head(request1);
    eventLoop.exec();

    All_size = 0;
    All_size += replay1->header(QNetworkRequest::ContentLengthHeader).toULongLong();

    QString CursorUrl = "https://blue.aut.ac.ir/presentation/" + meetingID + "/cursor.xml";
    request1.setUrl(CursorUrl);
    replay1 = manager1->head(request1);
    eventLoop.exec();
    All_size += replay1->header(QNetworkRequest::ContentLengthHeader).toULongLong();

    QString DeskshareUrl = "https://blue.aut.ac.ir/presentation/" + meetingID + "/deskshare.xml";
    request1.setUrl(DeskshareUrl);
    replay1 = manager1->head(request1);
    eventLoop.exec();
    All_size += replay1->header(QNetworkRequest::ContentLengthHeader).toULongLong();

    QString DeskshareUrl1 = "https://blue.aut.ac.ir/presentation/" + meetingID + "/deskshare/deskshare.webm";
    request1.setUrl(DeskshareUrl1);
    replay1 = manager1->head(request1);
    eventLoop.exec();
    All_size += replay1->header(QNetworkRequest::ContentLengthHeader).toULongLong();

    QString webcamsUrl1 = "https://blue.aut.ac.ir/presentation/" + meetingID + "/video/webcams.webm";
    request1.setUrl(webcamsUrl1);
    replay1 = manager1->head(request1);
    eventLoop.exec();
    All_size += replay1->header(QNetworkRequest::ContentLengthHeader).toULongLong();


    QString panzoomsUrl = "https://blue.aut.ac.ir/presentation/" + meetingID + "/panzooms.xml";
    request1.setUrl(panzoomsUrl);
    replay1 = manager1->head(request1);
    eventLoop.exec();
    All_size += replay1->header(QNetworkRequest::ContentLengthHeader).toULongLong();





    QString Moving = "https://blue.aut.ac.ir/presentation/" + meetingID + "/shapes.svg";
    request1.setUrl(Moving);
    replay1 = manager1->get(request1);
    eventLoop.exec();


    shapes=replay1->readAll();

    extractshapeslide();
    if (download_All_slide)
    {
        extractAllAddress();
        for (int i = 0; i < justaddressurl.count(); i++)
        {
            for (int j = 0; j < 1000; j++)
            {
                request1.setUrl("https://blue.aut.ac.ir/presentation/" + meetingID+"/presentation/"+ justaddressurl[i]+"/"+"slide-"+ QString::number(j + 1) + ".png");
                replay1 = manager1->head(request1);
                eventLoop.exec();
                QVariant status_code = replay1->attribute(QNetworkRequest::HttpStatusCodeAttribute);
                if (status_code.isValid()) {
                    // Print or catch the status code
                    QString status = status_code.toString();
                    if (status == "404")
                        break;
                }
                All_size += replay1->header(QNetworkRequest::ContentLengthHeader).toULongLong();
            }

        }
    }
    else
    {
        foreach (QString temp, addressfiles)
        {
            request1.setUrl("https://blue.aut.ac.ir/presentation/"+meetingID + "/presentation/" + temp);
            replay1 = manager1->head(request1);
            eventLoop.exec();
            All_size += replay1->header(QNetworkRequest::ContentLengthHeader).toULongLong();
        }
    }



    //
    showProgressbar();
    downloading();
    
}

void Download::extractshapeslide()
{
        int position = 0;
        while ((position = shapes.indexOf(" xlink:href=\"presentation/", position)) != -1)
        {
            QString fileid = shapes.mid(position + 26, shapes.indexOf("\" width=\"", position + 26) - position - 26);
            if (!addressfiles.contains(fileid))
            {
                addressfiles.append(fileid);
            }
            position += 1;
        }
    
}

void Download::extractAllAddress()
{
    foreach(QString addressUrl,addressfiles)
    {
        QString temp = addressUrl.mid(0, addressUrl.indexOf("/"));
        if (!justaddressurl.contains(temp))
        {
            justaddressurl.append(temp);
        }
    }
}

void Download::downloading()
{
    downloaded_complited = 0;
    connect(CancelDownload_btn, &QPushButton::clicked, this, [&](bool cliked) {this->close(); });
    //initial define
    QNetworkRequest request1;
    QNetworkAccessManager* manager1 = new QNetworkAccessManager(this);
    QNetworkReply* replay1;
    QEventLoop eventLoop;
    QObject::connect(manager1, &QNetworkAccessManager::finished, &eventLoop, &QEventLoop::quit);


    //shapes
    QString namefileshape = "Downloaded/" + NameFolder + "/" + "shapes.xml";
    shapesfile.setFileName(namefileshape);
    if (shapesfile.open(QIODevice::ReadWrite))
    {
        shapesfile.write(shapes);
        shapesfile.close();
    }
    shapes.clear();

    //metadata
    QString namefilemetadata = "Downloaded/" + NameFolder + "/" + "metadata.xml";
    metadatafile.setFileName(namefilemetadata);
    if (metadatafile.open(QIODevice::ReadWrite))
    {
        metadatafile.write(metadata);
        metadatafile.close();
    }
    metadata.clear();


    //chats
    QString ChatUrl = "https://blue.aut.ac.ir/presentation/" + meetingID + "/slides_new.xml";
    request1.setUrl(ChatUrl);
    replay1 = manager1->get(request1);
    connect(replay1, &QNetworkReply::downloadProgress, this, &Download::manageSizeDownload);
    eventLoop.exec();
    chats = replay1->readAll();
    chats = convertTounicode(chats);
    qDebug() << chats;
    QString namefileChat = "Downloaded/" + NameFolder + "/" + "chats.xml";
    chatsfile.setFileName(namefileChat);
    if (chatsfile.open(QIODevice::ReadWrite))
    {
        chatsfile.write(chats);
        chatsfile.close();
    }
    downloaded_complited += replay1->header(QNetworkRequest::ContentLengthHeader).toULongLong();
    progressBar10->setValue((float)(downloaded_complited) / All_size);
    chats.clear();


    //cursor
    QString CursorUrl = "https://blue.aut.ac.ir/presentation/" + meetingID + "/cursor.xml";
    request1.setUrl(CursorUrl);
    replay1 = manager1->get(request1);
    connect(replay1, &QNetworkReply::downloadProgress, this, &Download::manageSizeDownload);
    eventLoop.exec();
    cursor = replay1->readAll();
    QString namefileCursor = "Downloaded/" + NameFolder + "/" + "cursor.xml";
    cursorfile.setFileName(namefileCursor);
    if (cursorfile.open(QIODevice::ReadWrite))
    {
        cursorfile.write(cursor);
        cursorfile.close();
    }


    downloaded_complited += replay1->header(QNetworkRequest::ContentLengthHeader).toULongLong();
    progressBar10->setValue((float)(downloaded_complited) / All_size);
    cursor.clear();



    //desktopshareXML
    QString DeskshareUrl = "https://blue.aut.ac.ir/presentation/" + meetingID + "/deskshare.xml";
    request1.setUrl(DeskshareUrl);
    replay1 = manager1->get(request1);
    connect(replay1, &QNetworkReply::downloadProgress, this, &Download::manageSizeDownload);
    eventLoop.exec();
    QByteArray desksharexml = replay1->readAll();
    QString namefileDetailDeskshare1 = "Downloaded/" + NameFolder + "/" + "deskshare1.xml";
    desksharexmlfile.setFileName(namefileDetailDeskshare1);
    if (desksharexmlfile.open(QIODevice::ReadWrite))
    {
        desksharexmlfile.write(desksharexml);
        desksharexmlfile.close();
    }

    downloaded_complited += replay1->header(QNetworkRequest::ContentLengthHeader).toULongLong();
    progressBar10->setValue((float)((downloaded_complited) * 100 / All_size));
    desksharexml.clear();



//pak
    //deskshare_Asli just share screen
    QString DeskshareUrl1 = "https://blue.aut.ac.ir/presentation/" + meetingID + "/deskshare/deskshare.webm";
    request1.setUrl(DeskshareUrl1);
    QString namefileDeskshare1 = "Downloaded/" + NameFolder + "/" + "deskshare1.webm";
    desksharevideofile.setFileName(namefileDeskshare1);
    desksharevideofile.resize(0);
    if (desksharevideofile.open(QIODevice::ReadWrite))
    {
        replay1 = manager1->get(request1);
        connect(replay1, &QNetworkReply::downloadProgress, this, &Download::manageSizeDownload);
        connect(replay1, &QNetworkReply::readyRead, this, [&]() {qDebug() << "runnnnn";
                                                                 desksharevideofile.close();
                                                                 if (desksharevideofile.open(QIODevice::WriteOnly | QIODevice::Append))
                                                                 {
                                                                  qDebug() << "agra";
                                                                  desksharevideofile.write(replay1->readAll());
                                                                 }});




        eventLoop.exec();

    }
    if (desksharevideofile.isOpen())
        desksharevideofile.close();


    downloaded_complited += replay1->header(QNetworkRequest::ContentLengthHeader).toULongLong();
    progressBar10->setValue((float)((downloaded_complited) * 100 / All_size));
//end pak



/*
//pak
    //webcams_Asli and sound
    QString webcamsUrl1 = "https://blue.aut.ac.ir/presentation/" + meetingID + "/video/webcams.webm";
    request1.setUrl(webcamsUrl1);
    QString namefileWebcams = "Downloaded/" + NameFolder + "/" + "webcams.webm";
    webcamsfile.setFileName(namefileWebcams);
    webcamsfile.resize(0);
    if (webcamsfile.open(QIODevice::ReadWrite))
    {
        replay1 = manager1->get(request1);
        connect(replay1, &QNetworkReply::downloadProgress, this, &Download::manageSizeDownload);
        connect(replay1, &QNetworkReply::readyRead, this, [&]() {qDebug() << "runnnnn";
                                                                webcamsfile.close();
                                                                if (webcamsfile.open(QIODevice::WriteOnly |QIODevice::Append))
                                                                { 
                                                                 qDebug() << "agra";
                                                                 webcamsfile.write(replay1->readAll());
                                                                  }});


        eventLoop.exec();

    }
    if(webcamsfile.isOpen())
    webcamsfile.close();


    downloaded_complited += replay1->header(QNetworkRequest::ContentLengthHeader).toULongLong();
    progressBar10->setValue((float)((downloaded_complited) * 100 / All_size));
//end pak
*/


    //download panzooms
    QString panzoomUrl = "https://blue.aut.ac.ir/presentation/" + meetingID + "/panzooms.xml";
    request1.setUrl(panzoomUrl);
    replay1 = manager1->get(request1);
    eventLoop.exec();
    connect(replay1, &QNetworkReply::downloadProgress, this, &Download::manageSizeDownload);
    panzooms = replay1->readAll();
    QString namefilepanzooms = "Downloaded/" + NameFolder + "/" + "panzooms.xml";
    panzoomsfile.setFileName(namefilepanzooms);
    if (panzoomsfile.open(QIODevice::ReadWrite))
    {
        panzoomsfile.write(panzooms);
        panzoomsfile.close();
    }



    downloaded_complited += replay1->header(QNetworkRequest::ContentLengthHeader).toULongLong();
    progressBar10->setValue((float)((downloaded_complited) * 100 / All_size));
    panzooms.clear();




    
    //download slides
    if (download_All_slide)
    {
        //download all slides
        extractAllAddress();
        for (int i = 0; i < justaddressurl.count(); i++)
        {
            for (int j = 0; j < 1000; j++)
            {

                request1.setUrl("https://blue.aut.ac.ir/presentation/" + meetingID + "/presentation/" + justaddressurl[i] + "/" + "slide-" + QString::number(j + 1) + ".png");
                replay1 = manager1->get(request1);
                connect(replay1, &QNetworkReply::downloadProgress, this, &Download::manageSizeDownload);
                eventLoop.exec();
                QByteArray tempbyte = replay1->readAll();
                QVariant status_code = replay1->attribute(QNetworkRequest::HttpStatusCodeAttribute);
                if (status_code.isValid()) {
                    // Print or catch the status code
                    QString status = status_code.toString();
                    if (status == "404")
                        break;
                }
                if (!QDir("Downloaded/" + NameFolder + "/Slides").exists())
                    QDir().mkdir("Downloaded/" + NameFolder + "/Slides");
                if (!QDir("Downloaded/" + NameFolder + "/Slides/" + QString::number(i)).exists())
                {
                    //make this dir
                    QDir().mkdir("Downloaded/" + NameFolder + "/Slides/" + QString::number(i));
                }
                QFile filetemp;
                filetemp.setFileName("Downloaded/" + NameFolder + "/Slides/" +QString::number(i) +"/slide-" + QString::number(j + 1)+".png");
                if (filetemp.open(QIODevice::WriteOnly))
                {
                    filetemp.write(tempbyte);
                    filetemp.close();
                }
                QFile filetemp2;
                QString tempurl = justaddressurl[i] + "/" + "slide-" + QString::number(j + 1) + ".png";
                if (addressfiles.contains(tempurl))
                {
                    filetemp2.setFileName("Downloaded/" + NameFolder + "/Slides/" +justaddressurl[i]+"_" + "slide-" + QString::number(j + 1)+".png");
                    if (filetemp2.open(QIODevice::WriteOnly))
                    {
                        filetemp2.write(tempbyte);
                        filetemp2.close();
                    }
                }
                downloaded_complited += replay1->header(QNetworkRequest::ContentLengthHeader).toULongLong();
            }

        }
    }
    else 
    {
        //download slides used
        for (int i = 0; i < addressfiles.count(); i++)
        {
            if (!QDir("Downloaded/" + NameFolder + "/Slides").exists())
                QDir().mkdir("Downloaded/" + NameFolder + "/Slides");
            request1.setUrl("https://blue.aut.ac.ir/presentation/" + meetingID + "/presentation/"+ addressfiles[i]);
            replay1 = manager1->get(request1);
            connect(replay1, &QNetworkReply::downloadProgress, this, &Download::manageSizeDownload);
            eventLoop.exec();
            QByteArray tempbyte = replay1->readAll();
            QString addressurl= addressfiles[i].mid(0, addressfiles[i].indexOf("/"));
            QString namefile=addressfiles[i].mid(addressfiles[i].indexOf("/")+1);;
            QFile filetemp5;
            filetemp5.setFileName("Downloaded/"+ NameFolder+"/"+"/Slides/"+addressurl + "_" + namefile);
            if (filetemp5.open(QIODevice::WriteOnly))
            {
                filetemp5.write(tempbyte);
                filetemp5.close();
            }
            downloaded_complited += replay1->header(QNetworkRequest::ContentLengthHeader).toULongLong();
        }

    }

    this->close();






}


void Download::manageSizeDownload(qint64 bytesReceived, qint64 bytesTotal)
{
    int value = (float)((bytesReceived + downloaded_complited) * 100 / (All_size));
    progressBar10->setValue(value);
}

void Download::add_hexTounicode()
{
    hexTounicode.insert(QString("&#x620;"), QString(u8"ي"));
    hexTounicode.insert(QString("&#x621;"), QString(u8"ء"));
    hexTounicode.insert(QString("&#x622;"), QString(u8"آ"));
    hexTounicode.insert(QString("&#x623;"), QString(u8"أ"));
    hexTounicode.insert(QString("&#x624;"), QString(u8"ؤ"));
    hexTounicode.insert(QString("&#x625;"), QString(u8"إ"));
    hexTounicode.insert(QString("&#x626;"), QString(u8"ئ"));
    hexTounicode.insert(QString("&#x627;"), QString(u8"ا"));
    hexTounicode.insert(QString("&#x628;"), QString(u8"ب"));
    hexTounicode.insert(QString("&#x629;"), QString(u8"ة"));
    hexTounicode.insert(QString("&#x630;"), QString(u8"ذ"));
    hexTounicode.insert(QString("&#x631;"), QString(u8"ر"));
    hexTounicode.insert(QString("&#x632;"), QString(u8"ز"));
    hexTounicode.insert(QString("&#x633;"), QString(u8"س"));
    hexTounicode.insert(QString("&#x634;"), QString(u8"ش"));
    hexTounicode.insert(QString("&#x635;"), QString(u8"ص"));
    hexTounicode.insert(QString("&#x636;"), QString(u8"ض"));
    hexTounicode.insert(QString("&#x637;"), QString(u8"ط"));
    hexTounicode.insert(QString("&#x638;"), QString(u8"ظ"));
    hexTounicode.insert(QString("&#x639;"), QString(u8"ع"));
    hexTounicode.insert(QString("&#x640;"), QString(u8"@"));
    hexTounicode.insert(QString("&#x641;"), QString(u8"ف"));
    hexTounicode.insert(QString("&#x642;"), QString(u8"ق"));
    hexTounicode.insert(QString("&#x643;"), QString(u8"ک"));
    hexTounicode.insert(QString("&#x6A9;"), QString(u8"ک"));
    hexTounicode.insert(QString("&#x644;"), QString(u8"ل"));
    hexTounicode.insert(QString("&#x645;"), QString(u8"م"));
    hexTounicode.insert(QString("&#x646;"), QString(u8"ن"));
    hexTounicode.insert(QString("&#x647;"), QString(u8"ه"));
    hexTounicode.insert(QString("&#x648;"), QString(u8"و"));
    hexTounicode.insert(QString("&#x649;"), QString(u8"ی"));
    hexTounicode.insert(QString("&#x661;"), QString(u8"1"));
    hexTounicode.insert(QString("&#x662;"), QString(u8"2"));
    hexTounicode.insert(QString("&#x663;"), QString(u8"3"));
    hexTounicode.insert(QString("&#x664;"), QString(u8"4"));
    hexTounicode.insert(QString("&#x665;"), QString(u8"5"));
    hexTounicode.insert(QString("&#x666;"), QString(u8"6"));
    hexTounicode.insert(QString("&#x667;"), QString(u8"7"));
    hexTounicode.insert(QString("&#x668;"), QString(u8"8"));
    hexTounicode.insert(QString("&#x669;"), QString(u8"9"));
    hexTounicode.insert(QString("&#x686;"), QString(u8"چ"));
    hexTounicode.insert(QString("&#x698;"), QString(u8"ژ"));
    hexTounicode.insert(QString("&#x62D;"), QString(u8"ح"));
    hexTounicode.insert(QString("&#x67E;"), QString(u8"پ"));
    hexTounicode.insert(QString("&#x63A;"), QString(u8"غ"));
    hexTounicode.insert(QString("&#x62E;"), QString(u8"خ"));
    hexTounicode.insert(QString("&#x62C;"), QString(u8"ج"));
    hexTounicode.insert(QString("&#x62D;"), QString(u8"ح"));
    hexTounicode.insert(QString("&#x6AF;"), QString(u8"گ"));
    hexTounicode.insert(QString("&#x62A;"), QString(u8"ت"));
    hexTounicode.insert(QString("&#x62F;"), QString(u8"د"));
    hexTounicode.insert(QString("&#x6CC;"), QString(u8"ی"));
    hexTounicode.insert(QString("&#x62B;"), QString(u8"ث"));
    hexTounicode.insert(QString("&#x2E;"), QString(u8"."));
    hexTounicode.insert(QString("&#x21;"), QString(u8"!"));
    hexTounicode.insert(QString("&#x23;"), QString(u8"#"));
    hexTounicode.insert(QString("&#x24;"), QString(u8"$"));
    hexTounicode.insert(QString("&#x25;"), QString(u8"%"));
    hexTounicode.insert(QString("&#x5E;"), QString(u8"^"));
    hexTounicode.insert(QString("&#x26;"), QString(u8"&"));
    hexTounicode.insert(QString("&#x2A;"), QString(u8"*"));
    hexTounicode.insert(QString("&#x28;"), QString(u8"("));
    hexTounicode.insert(QString("&#x29;"), QString(u8")"));
    hexTounicode.insert(QString("&#x3C;"), QString(u8"<"));
    hexTounicode.insert(QString("&#x3E;"), QString(u8">"));
    hexTounicode.insert(QString("&#x3F;"), QString(u8"?"));
    hexTounicode.insert(QString("&#x2C;"), QString(u8","));
    hexTounicode.insert(QString("&#x7E;"), QString(u8"~"));
    hexTounicode.insert(QString("&#x5F;"), QString(u8"_"));
    hexTounicode.insert(QString("&#x2B;"), QString(u8"+"));
    hexTounicode.insert(QString("&#x3D;"), QString(u8"="));
    hexTounicode.insert(QString("&#x2D;"), QString(u8"-"));
    hexTounicode.insert(QString("&#x61F;"), QString(u8"؟"));
    hexTounicode.insert(QString("&#x3A;"), QString(u8":"));
    hexTounicode.insert(QString("&#x22;"), QString(u8""""));
    hexTounicode.insert(QString("&#x7B;"), QString(u8"{"));
    hexTounicode.insert(QString("&#x7D;"), QString(u8"}"));
    hexTounicode.insert(QString("&#x7C;"), QString(u8"|"));
    hexTounicode.insert(QString("&#x2F;"), QString(u8"/"));
    hexTounicode.insert(QString("&#x5C;"), QString(u8"\\"));
    hexTounicode.insert(QString("&#x3B;"), QString(u8";"));
    hexTounicode.insert(QString("&#x27;"), QString(u8"'"));
    hexTounicode.insert(QString("&#x30;"), QString(u8"0"));
    hexTounicode.insert(QString("&#x61B;"), QString(u8"؛"));
    hexTounicode.insert(QString("&#x60C;"), QString(u8"،"));
    hexTounicode.insert(QString("&#x5D;"), QString(u8"]"));
    hexTounicode.insert(QString("&#x5B;"), QString(u8"["));

    hexTounicode.insert(QString("&#x51;"), QString(u8"Q"));
    hexTounicode.insert(QString("&#x71;"), QString(u8"q"));
    hexTounicode.insert(QString("&#x57;"), QString(u8"W"));
    hexTounicode.insert(QString("&#x77;"), QString(u8"w"));
    hexTounicode.insert(QString("&#x45;"), QString(u8"E"));
    hexTounicode.insert(QString("&#x65;"), QString(u8"e"));
    hexTounicode.insert(QString("&#x52;"), QString(u8"R"));
    hexTounicode.insert(QString("&#x72;"), QString(u8"r"));
    hexTounicode.insert(QString("&#x54;"), QString(u8"T"));
    hexTounicode.insert(QString("&#x74;"), QString(u8"t"));
    hexTounicode.insert(QString("&#x59;"), QString(u8"Y"));
    hexTounicode.insert(QString("&#x79;"), QString(u8"y"));
    hexTounicode.insert(QString("&#x55;"), QString(u8"U"));
    hexTounicode.insert(QString("&#x75;"), QString(u8"u"));
    hexTounicode.insert(QString("&#x49;"), QString(u8"I"));
    hexTounicode.insert(QString("&#x69;"), QString(u8"i"));
    hexTounicode.insert(QString("&#x4F;"), QString(u8"O"));
    hexTounicode.insert(QString("&#x6F;"), QString(u8"o"));
    hexTounicode.insert(QString("&#x50;"), QString(u8"P"));
    hexTounicode.insert(QString("&#x70;"), QString(u8"p"));
    hexTounicode.insert(QString("&#x41;"), QString(u8"A"));
    hexTounicode.insert(QString("&#x61;"), QString(u8"a"));
    hexTounicode.insert(QString("&#x53;"), QString(u8"S"));
    hexTounicode.insert(QString("&#x73;"), QString(u8"s"));
    hexTounicode.insert(QString("&#x44;"), QString(u8"D"));
    hexTounicode.insert(QString("&#x64;"), QString(u8"d"));
    hexTounicode.insert(QString("&#x46;"), QString(u8"F"));
    hexTounicode.insert(QString("&#x66;"), QString(u8"f"));
    hexTounicode.insert(QString("&#x47;"), QString(u8"G"));
    hexTounicode.insert(QString("&#x67;"), QString(u8"g"));
    hexTounicode.insert(QString("&#x48;"), QString(u8"H"));
    hexTounicode.insert(QString("&#x68;"), QString(u8"h"));
    hexTounicode.insert(QString("&#x4A;"), QString(u8"J"));
    hexTounicode.insert(QString("&#x6A;"), QString(u8"j"));
    hexTounicode.insert(QString("&#x4B;"), QString(u8"K"));
    hexTounicode.insert(QString("&#x6B;"), QString(u8"k"));
    hexTounicode.insert(QString("&#x4C;"), QString(u8"L"));
    hexTounicode.insert(QString("&#x6C;"), QString(u8"l"));
    hexTounicode.insert(QString("&#x5A;"), QString(u8"Z"));
    hexTounicode.insert(QString("&#x7A;"), QString(u8"z"));
    hexTounicode.insert(QString("&#58;"), QString(u8"X"));
    hexTounicode.insert(QString("&#78;"), QString(u8"x"));
    hexTounicode.insert(QString("&#x43;"), QString(u8"C"));
    hexTounicode.insert(QString("&#x63;"), QString(u8"c"));
    hexTounicode.insert(QString("&#x56;"), QString(u8"V"));
    hexTounicode.insert(QString("&#x76;"), QString(u8"v"));
    hexTounicode.insert(QString("&#x42;"), QString(u8"B"));
    hexTounicode.insert(QString("&#x62;"), QString(u8"b"));
    hexTounicode.insert(QString("&#x4E;"), QString(u8"N"));
    hexTounicode.insert(QString("&#x6E;"), QString(u8"n"));
    hexTounicode.insert(QString("&#x4D;"), QString(u8"M"));
    hexTounicode.insert(QString("&#x6D;"), QString(u8"m"));
}

QByteArray Download::convertTounicode(QByteArray before)
{
    while (before.indexOf("&#x") != -1)
    {
        QString temp = before.mid(before.indexOf("&#"), before.indexOf(";") - before.indexOf("&#") + 1);
        before.replace(temp, hexTounicode.value(temp).toUtf8());
    }
    return before;
}
