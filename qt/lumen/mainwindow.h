#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
//Serial Port Lib
#include <qserialport.h>
#include <QSerialPortInfo>
//Menu bar lib
#include <QMenu>
#include <QMenuBar>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    // Connection functions
    void populateCOMPorts();
    void populateBitRates();
};
#endif // MAINWINDOW_H
