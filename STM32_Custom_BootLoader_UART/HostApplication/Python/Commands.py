Flash_HAL_OK = 0x00
Flash_HAL_ERROR = 0x01
Flash_HAL_BUSY = 0x02
Flash_HAL_TIMEOUT = 0x03
Flash_HAL_INV_ADDR = 0x04

# BL Commands
cmd_get_version = 0x20
cmd_get_cid = 0x21
cmd_get_rdp = 0x22
cmd_get_flash_size = 0x23
cmd_jump_to_application = 0x11
# len details of the command
BL_GET_VER_LEN = 6
BL_GET_CID_LEN = 6
