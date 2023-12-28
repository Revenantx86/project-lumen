#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
//Serial Port Lib
#include <qserialport.h>
#include <QSerialPortInfo>
#include <string>
//Menu bar lib
#include <QMenu>
#include <QMenuBar>
#include <QtCharts>

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

private slots:
    void updateStatusBar(QAction *action);

    void on_actionconnect_triggered();\

    void readFromPort();

private:
    Ui::MainWindow *ui;

    // Connection functions
    void populateCOMPorts();
    void populateBitRates();
    void disconnectArduino();

    //Graph Function
    void setupChart();

    // Private variables
    QSerialPort serial;

    QString selectedCOMPort = "/dev/tty.usbmodem21101";
    int selectedBitRate =9600;
    QByteArray serialData;
    QString serialBuffer;

};
#endif // MAINWINDOW_H
