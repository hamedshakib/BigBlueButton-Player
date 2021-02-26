#pragma once

#include <QWidget>
#include "ui_Download.h"

#include "ui_download dialog.h"
#include "ui_download progressbar.h"
#include "qgridlayout.h"

#include "qdebug.h"
#include "qurl.h"
#include "qstring.h"
#include "qnetworkaccessmanager.h"
#include "qnetworkrequest.h"
#include "qnetworkreply.h"
#include "qeventloop.h"
#include "qfiledialog.h"
#include "qfileinfo.h"
#include "qcalendar.h"
#include "qtextdocument.h"
#include "qbytearray.h"
#include "qvector.h"
#include "qmap.h"




#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QWidget>
#include <QtWidgets/qprogressbar.h>


class Download : public QWidget
{
	Q_OBJECT

public:
	Download(QWidget *parent = Q_NULLPTR);
	~Download();

private:
	Ui::Download ui;




private:
	Ui::dialog ui_dialog;
	Ui::progressbar ui_progressbar;

public:
	QGridLayout mainLayout;


	QGridLayout* gridLayout_2;
	QGridLayout* gridLayout;
	QPushButton* Cancel_btn;
	QPushButton* Download_btn;
	QLabel* label;
	QCheckBox* downloadAll_checkBox;
	QSpacerItem* horizontalSpacer;
	QLineEdit* Url_lineedit;


	//
	QGridLayout* gridLayout_12;
	QGridLayout* gridLayout10;
	QLabel* label_12;
	QLabel* DownloadingLabel;
	QSpacerItem* horizontalSpacer10;
	QPushButton* CancelDownload_btn;
	QLabel* label_13;
	QProgressBar* progressBar10;


public:
	QMap<QString, QString>  hexTounicode;
	bool download_All_slide = false;
	QUrl mainUrl;
	QString meetingID;
	QString str_Url;
	QString NameFolder;

	

	QFile shapesfile;
	QFile metadatafile;
	QFile chatsfile;
	QFile cursorfile;
	QFile desksharexmlfile;
	QFile desksharevideofile;
	QFile webcamsfile;
	QFile panzoomsfile;

	void showDialog();
	void showProgressbar();

public slots:
	void download_clicked(bool is_clicked);
	void pre_download();
	void extractshapeslide();
	void extractAllAddress();
	void downloading();
	void manageSizeDownload(qint64 bytesReceived, qint64 bytesTotal);
	void add_hexTounicode();
	QByteArray convertTounicode(QByteArray before);



public:
	QByteArray metadata;
	QByteArray shapes;
	QByteArray chats;
	QByteArray cursor;
	QByteArray panzooms;
	QStringList addressfiles;
	QStringList justaddressurl;

	

	long long int All_size=0;
	long long int tempdownloaded = 0;
	long long int downloaded_complited = 0;
};
