import serial.tools.list_ports
import BootLoader
from threading import Thread
import sys
from pathlib import Path

from PySide6.QtCore import QObject, Slot, Signal
from PySide6.QtGui import QGuiApplication
from PySide6.QtQml import QQmlApplicationEngine
from PySide6.QtQuickControls2 import QQuickStyle

ser_baud_rate = 115200
file_path = "Application.bin"
ser_port = "COM3"

if __name__ == "__main__":
    print("Start Application")

    bl = BootLoader.BootLoader(ser_port, file_path, ser_baud_rate)
    bootloader_thread = Thread(target=bl.main_process)
    bootloader_thread.start()

    app = QGuiApplication(sys.argv)
    QQuickStyle.setStyle("Material")
    engine = QQmlApplicationEngine()

    engine.rootContext().setContextProperty("bootloader", bl)

    qml_file = Path(__file__).resolve().parent / "main.qml"
    engine.load(qml_file)
    if not engine.rootObjects():
        sys.exit(-1)
    sys.exit(app.exec())
