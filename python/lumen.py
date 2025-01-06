import sys
from PyQt5.QtWidgets import QMainWindow, QApplication, QMenu, QAction, QMenuBar, QStatusBar, QFileDialog
from PyQt5.QtSerialPort import QSerialPort, QSerialPortInfo
from PyQt5.QtCore import pyqtSlot
from pyqtgraph import PlotWidget
import pyqtgraph as pg
import csv

class MainWindow(QMainWindow):
    def __init__(self):
        super().__init__()

        # UI Initialization
        self.init_ui()

        # Serial Variables
        self.serial = QSerialPort()
        self.serial_buffer = ""
        self.selected_com_port = None
        self.plotting = False

        # Startup Menu Initialization
        self.populate_com_ports()
        self.populate_bit_rates()

    def init_ui(self):
        self.setWindowTitle("Serial Connection")

        # Menu Bar
        menu_bar = self.menuBar()

        # Connection Menu
        connection_menu = menu_bar.addMenu("&Connection")
        self.com_port_menu = connection_menu.addMenu("COM Ports")
        self.bit_rate_menu = connection_menu.addMenu("Bit Rates")

        connect_action = QAction("Connect", self)
        connect_action.triggered.connect(self.on_action_connect_triggered)
        connection_menu.addAction(connect_action)

        # Data Menu
        data_menu = menu_bar.addMenu("&Data")

        start_action = QAction("Start Plotting", self)
        start_action.triggered.connect(self.start_plotting)
        data_menu.addAction(start_action)

        stop_action = QAction("Stop Plotting", self)
        stop_action.triggered.connect(self.stop_plotting)
        data_menu.addAction(stop_action)

        export_action = QAction("Export as CSV", self)
        export_action.triggered.connect(self.export_data_as_csv)
        data_menu.addAction(export_action)

        # Status Bar
        self.status_bar = QStatusBar()
        self.setStatusBar(self.status_bar)

        # Chart Setup
        self.chart = PlotWidget()
        self.setCentralWidget(self.chart)
        self.setup_chart()

    def populate_com_ports(self):
        self.com_port_menu.clear()

        for info in QSerialPortInfo.availablePorts():
            action = QAction(info.portName(), self)
            action.triggered.connect(lambda checked, a=action: self.select_com_port(a))
            self.com_port_menu.addAction(action)

    def populate_bit_rates(self):
        self.bit_rate_menu.clear()

        bit_rates = [1200, 2400, 4800, 9600, 19200, 38400, 57600, 115200]
        for rate in bit_rates:
            action = QAction(str(rate), self)
            action.triggered.connect(lambda checked, a=action: self.update_status_bar(a))
            self.bit_rate_menu.addAction(action)

    def select_com_port(self, action):
        self.selected_com_port = action.text()
        self.status_bar.showMessage(f"Selected COM Port: {self.selected_com_port}")

    @pyqtSlot()
    def on_action_connect_triggered(self):
        if not self.selected_com_port:
            self.status_bar.showMessage("No COM port selected.")
            return

        self.serial.setPortName(self.selected_com_port)
        self.serial.setBaudRate(QSerialPort.Baud9600)

        if self.serial.open(QSerialPort.ReadOnly):
            self.status_bar.showMessage(f"Connected to {self.selected_com_port} at 9600 baud")
            self.serial.readyRead.connect(self.read_from_port)
        else:
            self.status_bar.showMessage(f"Failed to connect to {self.selected_com_port}")

    def update_status_bar(self, action):
        if action:
            self.status_bar.showMessage(action.text())

    def read_from_port(self):
        if not self.plotting:
            return

        data = self.serial.readAll().data().decode("utf-8")
        self.serial_buffer += data

        while "," in self.serial_buffer:
            split_index = self.serial_buffer.index(",")
            value = self.serial_buffer[:split_index]
            self.serial_buffer = self.serial_buffer[split_index + 1:]

            try:
                value = int(value)
                self.update_chart(value)
            except ValueError:
                self.status_bar.showMessage("Invalid data received")

        self.status_bar.showMessage(f"Buffer: {self.serial_buffer}")  # Debug statement

    def setup_chart(self):
        self.chart.setBackground('w')
        self.chart.addLegend()

        self.plot_data = self.chart.plot(pen=pg.mkPen(color=(40, 110, 255), width=2), name="Data")
        self.x_data = []
        self.y_data = []

    def update_chart(self, value):
        self.x_data.append(len(self.x_data))
        self.y_data.append(value)

        self.plot_data.setData(self.x_data, self.y_data)

    def start_plotting(self):
        self.plotting = True
        self.status_bar.showMessage("Plotting started.")

    def stop_plotting(self):
        self.plotting = False
        self.status_bar.showMessage("Plotting stopped.")

    def export_data_as_csv(self):
        options = QFileDialog.Options()
        file_path, _ = QFileDialog.getSaveFileName(self, "Export Data as CSV", "", "CSV Files (*.csv);;All Files (*)", options=options)
        if file_path:
            try:
                with open(file_path, 'w', newline='') as file:
                    writer = csv.writer(file)
                    writer.writerow(["Index", "Value"])
                    writer.writerows(zip(self.x_data, self.y_data))
                self.status_bar.showMessage("Data exported successfully.")
            except Exception as e:
                self.status_bar.showMessage(f"Failed to export data: {e}")

    def closeEvent(self, event):
        if self.serial.isOpen():
            self.serial.close()
            self.status_bar.showMessage("Disconnected")
        event.accept()

if __name__ == "__main__":
    app = QApplication(sys.argv)
    window = MainWindow()
    window.show()
    sys.exit(app.exec_())
