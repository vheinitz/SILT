#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QImage>

#include <QNetworkAccessManager>
#include <QUrl>

class QSslError;
class QAuthenticator;
class QNetworkReply;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private slots:
    void on_eImage_returnPressed();

    void on_bGenerate_clicked();

    void on_bImport_clicked();

	QString importImage( QImage );

    void callApi( QString surl, QString data );
    void httpFinished();
    void httpReadyRead();

private:
    Ui::MainWindow *ui;
    QImage _img;
	QNetworkAccessManager _qnam;
};

#endif // MAINWINDOW_H
