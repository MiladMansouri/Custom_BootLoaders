import serial.tools.list_ports
import BootLoader
from threading import Thread

ports = serial.tools.list_ports.comports()
for port in ports:
    print(port[0])

ser_baud_rate = 115200
file_path = "Application.bin"
ser_port = "COM3"

bl = BootLoader.BootLoader(ser_port, file_path, ser_baud_rate)

bootloader_thread = Thread(target=bl.main_process)

if __name__ == '__main__':
    print("Start Application")
    bootloader_thread.start()
