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
    serialBuffer = "";

    // Initalize Tabs
    setupChart();

}

MainWindow::~MainWindow()
{
    delete ui;
    disconnectArduino();
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

    foreach (QAction *action, comPortMenu->actions()) {
        connect(action, &QAction::triggered, this, [this, action]() {
            updateStatusBar(action);
        });
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

    qDebug() << "listing Bit Rates";
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

    foreach (QAction *action, bitRateMenu->actions()) {
        connect(action, &QAction::triggered, this, [this, action]() {
            updateStatusBar(action);
        });
    }
}

// Connect
void MainWindow::on_actionconnect_triggered()
{
    serial.setPortName(selectedCOMPort);
    serial.setBaudRate(QSerialPort::Baud9600);
    //serial.setDataBits(QSerialPort::Data8);
    //serial.setFlowControl(QSerialPort::NoFlowControl);
    //serial.setParity(QSerialPort::NoParity);
    //serial.setStopBits(QSerialPort::OneStop);

    if (serial.open(QIODevice::ReadOnly)) {
        // Connection successful
        statusBar()->showMessage("Connected to " + selectedCOMPort + " at " + QString::number(selectedBitRate) + " baud");
        //serial.write("\n");
        connect(&serial, &QSerialPort::readyRead, this, &MainWindow::readFromPort);
    } else {
        // Connection failed
        statusBar()->showMessage("Failed to connect to " + selectedCOMPort);
    }
}

// --- Status Bar Functions --- //
void MainWindow::updateStatusBar(QAction *action)
{
    if (action) {
        qDebug() << " Action Triggerd";
        statusBar()->showMessage(action->text());

    }

}

// --- Serial Functions --- //

void MainWindow::readFromPort()
{
    QStringList buffer_split = serialBuffer.split(","); //  split the serialBuffer string, parsing with ',' as the separator
    if(buffer_split.length() < 3){
        serialData = serial.readAll();
        serialBuffer += QString::fromStdString(serialData.toStdString());
        serialData.clear();
    }
    else
    {
        serialBuffer = "";
        qDebug() << buffer_split << "\n";
    }
    //qDebug() << serialBuffer;
    statusBar()->showMessage(serialBuffer);
}

void MainWindow::disconnectArduino()
{
    if (serial.isOpen()) {
        serial.close();
        statusBar()->showMessage("Disconnected from " + selectedCOMPort);
    }
}

// --- Chart Functions --- //
void MainWindow::setupChart()
{

    QLineSeries *series = new QLineSeries();
    series->append(2,6);
    series->append(4,8);


    QChart *chart = new QChart();
    //chart->legend()->hide();
    chart->addSeries(series);
    chart->setTitle("Lumen logger chart");

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setParent(this->ui->graphicsView);
}
