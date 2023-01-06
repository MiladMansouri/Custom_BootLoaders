import binascii
import struct
import Commands as cmd
import serial.tools.list_ports
from threading import Thread
from time import sleep

ports = serial.tools.list_ports.comports()
for port in ports:
    print(port[0])

ser_port = "COM6"
ser_baud_rate = 115200
ser = serial.Serial(port=ser_port, baudrate=ser_baud_rate, timeout=0, parity=serial.PARITY_NONE)


def calculate_crc(buff):
    crc = 0xFFFFFFFF
    for data in buff:
        crc = crc ^ data
        for _ in range(32):
            if crc & 0x80000000:
                crc = (crc << 1) ^ 0x04c11db7
            else:
                crc = (crc << 1)
    return [((crc & 0xFF000000) >> 24), ((crc & 0x00FF0000) >> 16), ((crc & 0x0000FF00) >> 8),
            (crc & 0x000000FF)]


def create_msg(command=None, data=None):
    if data is None:
        data_array = [command]
        length = 6
    else:
        data_array = [command] + data
        length = len(data) + 6
    return [length] + data_array + calculate_crc(data_array)


def print_hex(buffer, sr):
    """sr -> send(1) receive(0)"""
    if sr:
        print('-> :', end=' ')
    else:
        print('<- :', end=' ')
    for data in buffer:
        print(hex(data), end=' ')
    print(' ')


def send_msg(command=None, data=None):
    arr = create_msg(command, data)
    ser.write(arr)
    print_hex(arr, 1)


def process_command(command=None, data=None):
    send_msg(command, data)
    sleep(0.5)
    receive_data = ser.read_all()
    if len(receive_data) > 1:
        if receive_data[1] == (command + 0x40):
            print("successful")
            print_hex(receive_data, 0)
        else:
            print("Unsuccessful")
            print_hex(receive_data, 0)
    else:
        print('Time Out Error', end='')
    print(' ')


def main_process():
    while True:
        input_command = input("Please Enter Command: ")
        if not input_command.isdigit():
            print("Please Enter Correct Command \r\n")
        else:
            command = int(input_command)
            if command == 1:
                process_command(command=cmd.cmd_jump_to_application)
            elif command == 2:
                process_command(command=cmd.cmd_get_version)
            elif command == 3:
                process_command(command=cmd.cmd_get_cid)
            elif command == 4:
                process_command(command=cmd.cmd_get_rdp)
            elif command == 5:
                process_command(command=cmd.cmd_get_flash_size)
            sleep(0.1)


main_process()
# receive_thread = Thread(target=read_data, args=())
# command_thread = Thread(target=process_command, args=())
# receive_thread.start()
# command_thread.start()

# def read_data():
#     while True:
#         bytes_to_read = ser.in_waiting
#         if bytes_to_read > 0:
#             received_data = ser.read(bytes_to_read)
#             print(f"Receiving {bytes_to_read} bytes, {received_data.hex('-')}\n")
#             ser.reset_input_buffer()
