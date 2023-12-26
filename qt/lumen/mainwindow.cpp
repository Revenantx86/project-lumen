#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //Startup Menu Initialization
    populateCOMPorts();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::populateCOMPorts()
{
    // Create or find 'Connection' menu
    QMenu *connectionMenu = menuBar()->addMenu(tr("&Connection"));

    // Create 'COM Ports' submenu
    QMenu *comPortMenu = connectionMenu->addMenu(tr("COM Ports"));

    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        comPortMenu->addAction(info.portName());
    }
}
