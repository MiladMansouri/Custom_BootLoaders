import Commands as cmd
import serial.tools.list_ports
from time import sleep
import os

number_of_data_flash_programing = 240


class BootLoader:
    def __init__(self, comport, file_path, baud_rate):
        self.file_path = file_path
        self.ser = serial.Serial(port=comport,
                                 baudrate=baud_rate,
                                 timeout=0,
                                 parity=serial.PARITY_NONE)

    def calculate_crc(self, buff):
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

    def create_msg(self, command=None, data=None):
        if data is None:
            data_array = [command]
            length = 6
        else:
            data_array = [command] + data
            length = len(data) + 6
        return [length] + data_array + self.calculate_crc(data_array)

    def print_hex(self, buffer, sr):
        """sr -> send(1) receive(0)"""
        i = 0
        if sr:
            print('->> :', end=' ')
        else:
            print('<<- :', end=' ')
        for data in buffer:
            print(hex(data), end=' ', sep=',')
            i += 1
            if i > 20:
                print(' ')
                print('->>', end=' ')
                i = 0
        print(' ')

    def send_msg(self, command=None, data=None):
        arr = self.create_msg(command, data)
        self.ser.write(arr)
        self.print_hex(arr, 1)

    def process_command(self, command=None, data=None):
        self.send_msg(command, data)
        sleep(0.5)
        receive_data = self.ser.read_all()
        if len(receive_data) > 1:
            if receive_data[1] == (command + 0x40):
                print("successful")
                self.print_hex(receive_data, 0)
                return receive_data
            else:
                print("Unsuccessful")
                self.print_hex(receive_data, 0)
                return None
        else:
            print('Time Out Error')
        return None

    def flash_erasing(self):
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
                data = self.process_command(command=cmd.cmd_flash_erase,
                                            data=[((start_address & 0xFF000000) >> 24),
                                                  ((start_address & 0x00FF0000) >> 16),
                                                  ((start_address & 0x0000FF00) >> 8),
                                                  (start_address & 0x000000FF),
                                                  page_number])

    def get_all_elements_in_list_of_lists(self, list_e):
        count = 0
        for element in list_e:
            count += len(element)
        return count

    def program_flash(self):
        data_buffer = []
        temp_data = []
        index = 0

        f = open(self.file_path, 'rb')
        size = os.path.getsize(self.file_path)

        for i in range(size):
            temp_data.append(int.from_bytes(f.read(1), 'big'))

        if size % 4:
            for i in range((4 - (size % 4))):
                temp_data.append(0x00)

        for i in range(0, len(temp_data), 4):
            data_buffer.append(temp_data[i + 0])
            data_buffer.append(temp_data[i + 1])
            data_buffer.append(temp_data[i + 2])
            data_buffer.append(temp_data[i + 3])

        chunks = [data_buffer[x:x + number_of_data_flash_programing] for x in
                  range(0, len(data_buffer), number_of_data_flash_programing)]
        send_array = []
        data_buffer.clear()

        for index, item in enumerate(chunks):
            # send_array.clear()
            add = (index * number_of_data_flash_programing)
            send_array = [len(item),
                          ((add & 0xFF000000) >> 24),
                          ((add & 0x00FF0000) >> 16),
                          ((add & 0x0000FF00) >> 8),
                          (add & 0x000000FF)]
            send_array.extend(item)
            data_buffer.append(send_array)
        number_of_data = len(data_buffer) - 1
        number_of_writes = 0
        page = 1024

        print("=== Erasing Flash ===")
        start_address = 0x08008000
        page_number = 16
        ans = self.process_command(command=cmd.cmd_flash_erase,
                                   data=[((start_address & 0xFF000000) >> 24),
                                         ((start_address & 0x00FF0000) >> 16),
                                         ((start_address & 0x0000FF00) >> 8),
                                         (start_address & 0x000000FF),
                                         page_number])
        if ans is not None and ans[1] == (cmd.cmd_flash_erase + 0x40):
            print("Flash Erased successfully")
        else:
            print("Flash Erasing Error")
            return None

        programing_flag = True
        while programing_flag:
            print(f">>>>> {number_of_data} *** {number_of_writes} <<<<<")
            ans = self.process_command(cmd.cmd_flash_program, data_buffer[number_of_writes])
            if ans is not None and ans[1] == 0x66:
                number_of_writes += 1
                if number_of_writes > number_of_data:
                    programing_flag = False
                    print("🎉🎉 Flash Programed Successfully 🎉🎉")
            else:
                programing_flag = False
                print("⛔⛔ Abort Flash Programing ⛔⛔")

    def main_process(self):
        while True:
            print("<<< <<< START >>> >>>")
            input_command = input("Please Enter Command: ")
            if not input_command.isdigit():
                print("Please Enter Correct Command \r\n")
            else:
                command = int(input_command)
                if command == 0:
                    self.process_command(command=cmd.cmd_jump_to_bootloader, data=[0x0a])
                elif command == 1:
                    self.process_command(command=cmd.cmd_jump_to_application)
                elif command == 2:
                    self.process_command(command=cmd.cmd_get_version)
                elif command == 3:
                    data = self.process_command(command=cmd.cmd_get_mcu_device_id_code)
                elif command == 4:
                    data = self.process_command(command=cmd.cmd_get_rdp)
                    if data[2] == 0xa5:
                        print("OB_RDP_LEVEL_0")
                    elif data[2] == 0x00:
                        print("OB_RDP_LEVEL_1")
                elif command == 5:
                    data = self.process_command(command=cmd.cmd_get_flash_size)
                    flash_size = data[2] << 8 | data[3]
                    print(f'{flash_size} Kb')
                elif command == 6:
                    data = self.process_command(command=cmd.cmd_get_mcu_unique_id)
                elif command == 7:
                    self.flash_erasing()
                elif command == 8:
                    self.program_flash()
                sleep(0.1)
                print("<<< <<< END >>> >>> ")

# def read_data():
#     while True:
#         bytes_to_read = ser.in_waiting
#         if bytes_to_read > 0:
#             received_data = ser.read(bytes_to_read)
#             print(f"Receiving {bytes_to_read} bytes, {received_data.hex('-')}\n")
#             ser.reset_input_buffer()
