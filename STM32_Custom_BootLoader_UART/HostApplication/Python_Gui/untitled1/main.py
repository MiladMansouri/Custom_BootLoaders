# This Python file uses the following encoding: utf-8
import sys
import BootLoader
import os
from pathlib import Path

from PySide6.QtGui import QGuiApplication
from PySide6.QtQml import QQmlApplicationEngine
from PySide6.QtCore import QObject,Signal,Slot

ser_baud_rate = 115200
file_path = "Application.bin"
ser_port = "COM3"
CURRENT_DIRECTORY = Path(__file__).resolve().parent

if __name__ == "__main__":
    app = QGuiApplication(sys.argv)
    engine = QQmlApplicationEngine()

    bl = BootLoader.BootLoader(ser_port,file_path,ser_baud_rate)

    engine.rootContext().setContextProperty("applicationDirPath", os.fspath(CURRENT_DIRECTORY))
    engine.rootContext().setContextProperty("bootloader", bl)

    qml_file = Path(__file__).resolve().parent / "main.qml"
    engine.load(qml_file)
    if not engine.rootObjects():
        sys.exit(-1)
    sys.exit(app.exec())
