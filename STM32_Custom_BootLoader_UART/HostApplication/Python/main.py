import serial.tools.list_ports
import BootLoader
from threading import Thread

ser_baud_rate = 115200
file_path = "Application.bin"


def get_port():
    while True:
        print("Available Ports: ")
        ports = serial.tools.list_ports.comports()
        for index, port in enumerate(ports, start=1):
            print(f"{index} - {port[0]}")
        selected_port = input("Please Select a Port: ")
        if selected_port.isdigit() and int(selected_port) <= len(ports) and int(selected_port) != 0:
            print(f"Port {ports[int(selected_port) - 1]} Selected")
            return ports[int(selected_port) - 1][0]
        else:
            print("Please Enter Valid Port")


if __name__ == "__main__":
    print("Start Application")
    ser_port = get_port()
    bl = BootLoader.BootLoader(ser_port, file_path, ser_baud_rate)
    bootloader_thread = Thread(target=bl.main_process)
    bootloader_thread.start()
