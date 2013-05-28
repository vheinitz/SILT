#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QImage>

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

private:
    Ui::MainWindow *ui;
    QImage _img;
};

#endif // MAINWINDOW_H
