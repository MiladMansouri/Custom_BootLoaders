from past.builtins import xrange
import Commands as cmd
import serial.tools.list_ports
from time import sleep
import os

file_path = "E:\\Practical-Project\\Embedded\\Custom_BootLoaders\\STM32_Custom_BootLoader_UART\\BootLoader\\EWARM\\bootLoader\\Exe\\bootLoader.bin"
ser_port = "COM3"
ser_baud_rate = 115200

ports = serial.tools.list_ports.comports()
for port in ports:
    print(port[0])

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
        print('->> :', end=' ')
    else:
        print('<<- :', end=' ')
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
            return receive_data
        else:
            print("Unsuccessful")
            print_hex(receive_data, 0)
            return None
    else:
        print('Time Out Error')
    return None


def flash_erasing():
    address = input("please enter start address: 0x")
    if not address.isdigit():
        print("Please Enter Correct address \r\n")
    else:
        start_address = int(address, 16)
        num = input("please enter number of page to erase: ")
        if not num.isdigit():
            print("Please Enter Correct page number \r\n")
        else:
            page_number = int(num)
            print(start_address, page_number)
            data = process_command(command=cmd.cmd_flash_erase,
                                   data=[((start_address & 0xFF000000) >> 24),
                                         ((start_address & 0x00FF0000) >> 16),
                                         ((start_address & 0x0000FF00) >> 8),
                                         (start_address & 0x000000FF),
                                         page_number])


def get_all_elements_in_list_of_lists(list_e):
    count = 0
    for element in list_e:
        count += len(element)
    return count


def program_flash():
    data_buffer = []
    temp_data = []
    index = 0

    f = open(file_path, 'rb')
    size = os.path.getsize(file_path)

    for i in range(size):
        temp_data.append(int.from_bytes(f.read(1), 'big'))

    if size % 4:
        for i in range((4 - (size % 4))):
            temp_data.append(0x00)

    for i in range(0, len(temp_data), 4):
        data_buffer.append(temp_data[i + 3])
        data_buffer.append(temp_data[i + 2])
        data_buffer.append(temp_data[i + 1])
        data_buffer.append(temp_data[i + 0])

    chunks = [data_buffer[x:x + 240] for x in xrange(0, len(data_buffer), 240)]
    send_array = []
    data_buffer.clear()

    for index, item in enumerate(chunks):
        # send_array.clear()
        add = (index * 240)
        send_array = [len(item),
                      ((add & 0xFF000000) >> 24),
                      ((add & 0x00FF0000) >> 16),
                      ((add & 0x0000FF00) >> 8),
                      (add & 0x000000FF)]
        send_array.extend(item)
        data_buffer.append(send_array)
    number_of_data = len(data_buffer)
    number_of_writes = 0
    programing_flag = True
    while programing_flag:
        ans = process_command(cmd.cmd_flash_program, data_buffer[number_of_writes])
        programing_flag = False


def main_process():
    while True:
        print("<<< <<< START >>> >>>")
        input_command = input("Please Enter Command: ")
        if not input_command.isdigit():
            print("Please Enter Correct Command \r\n")
        else:
            command = int(input_command)
            if command == 0:
                process_command(command=cmd.cmd_jump_to_bootloader, data=[0x0a])
            elif command == 1:
                process_command(command=cmd.cmd_jump_to_application)
            elif command == 2:
                process_command(command=cmd.cmd_get_version)
            elif command == 3:
                data = process_command(command=cmd.cmd_get_mcu_device_id_code)
            elif command == 4:
                data = process_command(command=cmd.cmd_get_rdp)
                if data[2] == 0xa5:
                    print("OB_RDP_LEVEL_0")
                elif data[2] == 0x00:
                    print("OB_RDP_LEVEL_1")
            elif command == 5:
                data = process_command(command=cmd.cmd_get_flash_size)
                flash_size = data[2] << 8 | data[3]
                print(f'{flash_size} Kb')
            elif command == 6:
                data = process_command(command=cmd.cmd_get_mcu_unique_id)
            elif command == 7:
                flash_erasing()
            elif command == 8:
                program_flash()
            sleep(0.1)
            print("<<< <<< END >>> >>> ")


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
