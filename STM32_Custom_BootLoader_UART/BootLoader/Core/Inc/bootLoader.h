#ifndef __BOOT_LOADER_H
#define __BOOT_LOADER_H
#include "stdint.h"

#define BUFFER_SIZE 255
#define BL_MAJOR_VERSION 1.0
#define BL_MINOR_VERSION 2.0
#define APPLICATION_BASE_ADDRESS 0x08008000
typedef enum
{
  CMD_JUMP_TO_APP = 0x11U,
  CMD_GET_VERSION = 0x20U,
  CMD_GET_MCU_DEVICE_ID_CODE = 0x21U,
  CMD_GET_RDP_STATUS = 0x22U,
  CMD_GET_FLAHS_SIZE = 0x23U,
  CMD_GET_MCU_UNIQUE_ID_CODE = 0x24U,
  CMD_FLASH_ERASE = 0x25u,
  CMD_FLASH_PROGRAM = 0x26u,
}Te_BL_UID;

typedef enum
{
  ANS_GET_VERSION = 2U,
  ANS_JUMP_TO_APP = 0U,
  ANS_GET_MCU_DEVICE_ID_CODE = 4U,
  ANS_GET_RDP_STATUS = 1U,
  ANS_GET_FLAHS_SIZE = 2U,
  ANS_GET_MCU_UNIQUE_ID_CODE = 12u,
  ANS_FLASH_ERASE = 4u,
  ANS_FLASH_PROGRAM = 0u,
}Te_BL_ANSWER_SIZE;

typedef enum
{
    CRC_NOT_OK = 0u,
    CRC_OK = 1U
}Te_CRC_Check;

void boot_loader_processing(void);

static void jump_to_application(void);
static void get_version(void);
static void get_mcu_device_id_code(void);
static void get_mcu_flash_protection_level(void);
static void get_mcu_flash_size_in_kb(void);
static void get_mcu_unique_id_code(void);
static void flash_erase_process(void);
static uint8_t flash_program_process(void);
  
static Te_CRC_Check clculate_crc(void);
static void send_positive_answer(uint8_t size);
static void send_negetive_answer(void);
static void ready_for_new_command(void);
#endif
