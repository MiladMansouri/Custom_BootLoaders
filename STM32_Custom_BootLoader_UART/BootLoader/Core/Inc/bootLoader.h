#ifndef __BOOT_LOADER_H
#define __BOOT_LOADER_H
#include "stdint.h"

#define BUFFER_SIZE 255
#define BL_VERSION 0x10

typedef enum
{
  CMD_GET_VERSION = 0x20U,
  CMD_JUMP_TO_APP = 0x21U,
  CMD_GET_MCU_CID = 0x22U,
  CMD_GET_RDP_STATUS = 0x23U,
  CMD_GET_FLAHS_SIZE = 0x24U,
}Te_BL_UID;

typedef enum
{
  ANS_GET_VERSION = 0x01U,
  ANS_JUMP_TO_APP = 0x00U,
  ANS_GET_MCU_CID = 0x02U,
  ANS_GET_RDP_STATUS = 0x01U,
  ANS_GET_FLAHS_SIZE = 0x02U,
}Te_BL_ANSWER_SIZE;

void bootLoaderProcessing(void);

static void bootLoaderGetVersionProcessing(void);
static void bootLoaderJumpToApplication(void);
static void getMcuCidNumberProcessing(void);
static void getMcuFlashProtectionLevel(void);
static void getMcuFlashSizeinKb(void);


static void sendPositiveAnswer(uint8_t size);
static void sendNegetiveAnswer(void);
static void readyForNewCommand(void);
#endif
