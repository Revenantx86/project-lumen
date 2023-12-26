#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //Startup Menu Initialization

    populateCOMPorts();
    populateBitRates();
}

MainWindow::~MainWindow()
{
    delete ui;
}




// --- Serial Connection Functions -- //

// Initializing the comports at startup
void MainWindow::populateCOMPorts()
{
    // Find or create 'Connection' menu
    /*
    QMenu *connectionMenu = menuBar()->findChild<QMenu*>(tr("&Connection"));
    if (!connectionMenu) {
        connectionMenu = menuBar()->addMenu(tr("&Connection"));
    }
    */
    QMenu *parentmenu = this->findChild<QMenu*>("menuConnection");

    // Find COM Port submenu
    QMenu *comPortMenu = parentmenu->findChild<QMenu*>("menuCOM_Ports");

    // Clean the menu
    comPortMenu->clear();

    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        comPortMenu->addAction(info.portName());
    }
}

// Initializing the bitrates available for connection
void MainWindow::populateBitRates()
{
    /*
    // Find or create 'Connection' menu
    QMenu *connectionMenu = menuBar()->findChild<QMenu*>(tr("&Connection"));
    if (!connectionMenu) {
        connectionMenu = menuBar()->addMenu(tr("&Connection"));
    }
    */

    QMenu *parentmenu = this->findChild<QMenu*>("menuConnection");

    // Create 'Bit Rates' submenu
    QMenu *bitRateMenu = parentmenu->findChild<QMenu*>("menuBit_Rates");

    // Clean the menu
    bitRateMenu->clear();

    // List of common bit rates
    QList<int> bitRates = {1200, 2400, 4800, 9600, 19200, 38400, 57600, 115200};

    // Populate 'Bit Rates' submenu with available bit rates
    for (int rate : bitRates) {
        bitRateMenu->addAction(QString::number(rate));
    }
}
