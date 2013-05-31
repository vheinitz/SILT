#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QImage>
#include <QPixmap>
#include <QCryptographicHash>
#include <QMap>
#include <QFileInfo>
#include <QPainter>
#include <QProcess>
#include "persistence.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    PERSISTENCE_INIT( "Heinitz-It", "SLITImport" );
    PERSISTENT("LastImage", ui->eImage, "text");
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
    QString root = ui->eRoot->text();

    if(!QFileInfo(root).exists())
        return;//error

    QStringList outConf = ui->tOutConf->toPlainText().split("\n",QString::SkipEmptyParts);
    QMap<QString, QSize> outConfMap;
    foreach( QString s, outConf )
    {
        outConfMap[s.section(";",0,0)] = QSize( s.section(";",1).section("x",0,0).toInt(), s.section(";",1).section("x",1).toInt() );
    }

    //QString importCall= "C:/bin/curl.exe -X POST http://127.0.0.1:8000/SILT/default/add_images -d\"imageId=%1&_formkey=3db667ed-9e17-47a3-9b35-94ef9aca9c50&_formname=image/create\"";
    QPainter p;
    for (int i=from; i<to; ++i)
    {
       qApp->processEvents();
       QImage outImg = _img;

       p.begin(&outImg);
       p.setFont(QFont("Arial",50));
       p.setBrush(QBrush(Qt::blue));
       p.setPen(QColor(Qt::blue));
       p.drawText(QPoint(100,100),"Image:"+QString::number(i));
       p.end();
       QString imgName( QCryptographicHash::hash( QByteArray((const char*)(outImg.bits()),outImg.byteCount()), QCryptographicHash::Md5).toBase64().toHex() );
       outImg.save( root+"/"+imgName+".png" );
       for ( QMap<QString, QSize>::Iterator it = outConfMap.begin();
             it!=outConfMap.end(); ++it )
       {
            outImg.scaled( it.value() ).save( root+"/"+it.key()+"/"+imgName+".jpg" );
       }
       QString curl = ui->tImageCurl->toPlainText();
       curl.replace("$IMG",imgName);
       curl.replace("$CNT",QString::number(i));
       QProcess::startDetached(curl);

       curl = ui->tImgLabelCurl->toPlainText();
       curl.replace("$IMG",imgName);
       curl.replace("$CNT",QString::number(i));
       QProcess::startDetached(curl);
    }
}

void MainWindow::on_bImport_clicked()
{

}
