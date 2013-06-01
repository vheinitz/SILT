#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QImage>
#include <QPixmap>
#include <QCryptographicHash>
#include <QMap>
#include <QFileInfo>
#include <QPainter>
#include <QProcess>
#include <QDir>
#include <QNetworkReply>
#include "persistence.h"



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    PERSISTENCE_INIT( "Heinitz-It", "SLITImport" );
    PERSISTENT("LastImage", ui->eImage, "text");
	PERSISTENT("LastImagesDir", ui->eImageDirPath, "text");
    PERSISTENT("OutputRoot", ui->eRoot, "text");
    PERSISTENT("OutConf", ui->tOutConf, "plainText");
    //PERSISTENT("tLabelCurl", ui->tLabelCurl, "plainText");
    PERSISTENT("tImgLabelCurl", ui->tImgLabelCurl, "plainText");
    PERSISTENT("tImageCurl", ui->tImageCurl, "plainText");
    PERSISTENT("Geometry", this, "geometry");
    PERSISTENT("From", ui->sbFrom, "value");
    PERSISTENT("To", ui->sbTo, "value");
    on_eImage_returnPressed();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_eImage_returnPressed()
{
    _img = QImage(ui->eImage->text());
    ui->lImage->setPixmap(QPixmap::fromImage(_img.scaled(640,480)));
}

void MainWindow::on_bGenerate_clicked()
{
    int from = ui->sbFrom->value();
    int to = ui->sbTo->value();
    if (from>to)
        return;//error
    

    //QString importCall= "C:/bin/curl.exe -X POST http://127.0.0.1:8000/SILT/default/add_images -d\"imageId=%1&_formkey=3db667ed-9e17-47a3-9b35-94ef9aca9c50&_formname=image/create\"";
    QPainter p;
    for (int i=from; i<to; ++i)
    {
	   QImage outImg = _img;
	   p.begin(&outImg);
       p.setFont(QFont("Arial",50));
       p.setBrush(QBrush(Qt::blue));
       p.setPen(QColor(Qt::blue));
       p.drawText(QPoint(100,100),"Image:"+QString::number(i));
       p.end();
	   QString imgName = importImage( outImg );

	   QString curl = ui->tImgLabelCurl->toPlainText();
	   curl.replace("$IMG",imgName);
	   curl.replace("$CNT",QString::number(i));
	   QProcess::startDetached(curl);
       qApp->processEvents();       
    }
}

QString MainWindow::importImage( QImage outImg )
{
	QString root = ui->eRoot->text();

    if(!QFileInfo(root).exists())
		return QString::null;//error

    QStringList outConf = ui->tOutConf->toPlainText().split("\n",QString::SkipEmptyParts);
    QMap<QString, QSize> outConfMap;
    foreach( QString s, outConf )
    {
        outConfMap[s.section(";",0,0)] = QSize( s.section(";",1).section("x",0,0).toInt(), s.section(";",1).section("x",1).toInt() );
    }
       
   QString imgName( QCryptographicHash::hash( QByteArray((const char*)(outImg.bits()),outImg.byteCount()), QCryptographicHash::Md5).toBase64().toHex() );
   outImg.save( root+"/"+imgName+".png" );
   for ( QMap<QString, QSize>::Iterator it = outConfMap.begin();
         it!=outConfMap.end(); ++it )
   {
        outImg.scaled( it.value() ).save( root+"/"+it.key()+"/"+imgName+".jpg" );
		qApp->processEvents(); 
   }  
   return imgName;
}

void MainWindow::on_bImport_clicked()
{
	QFileInfoList images = QDir(ui->eImageDirPath->text()).entryInfoList(QStringList()<<"*.png");
	foreach( QFileInfo fi, images )
	{
		QString fn = fi.absoluteFilePath();
		QImage img(fn);
		QString imgName = importImage( img );

		QString data = ui->tImageCurl->toPlainText();
		data.replace("$IMG",imgName);
		callApi( "http://127.0.0.1:8000/SILT/api/add_images", data );
		QStringList keys = img.textKeys();
		foreach( QString k, keys )
		{
			
		}
	}
}

void MainWindow::callApi( QString surl, QString data )
{
	QUrl url(surl);


#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
    QUrl postData;
#else
    QUrlQuery postData;
#endif
    postData.addQueryItem("data",data);
    /*for( QMap<QString,QString>::iterator it = _sendDataList.begin(); it !=_sendDataList.end();++it )
    {
        postData.addQueryItem( it.key(), it.value() );
    }*/



	//_sendDataList.clear();

	QNetworkRequest request(url);
	request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
	ui->tImgLabelCurl->appendPlainText( "POST: " + url.toString() );
	ui->tImgLabelCurl->appendPlainText( "  DATA: " + postData.encodedQuery());	
	QNetworkReply *reply = _qnam.post(request, postData.encodedQuery());

    connect(reply, SIGNAL(finished()),
         this, SLOT(httpFinished()));
    connect(reply, SIGNAL(readyRead()),
         this, SLOT(httpReadyRead()));

}

void MainWindow::httpFinished()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());
    if (!reply) 
		return;
    
    reply->deleteLater();
}

void MainWindow::httpReadyRead()
{
	QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());
	ui->tImgLabelCurl->appendPlainText( "GOT DATA: " + reply->readAll() );
}