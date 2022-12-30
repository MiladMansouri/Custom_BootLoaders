#ifndef __BOOT_LOADER_H
#define __BOOT_LOADER_H
#include "stdint.h"

#define BUFFER_SIZE 255
#define BL_VERSION 0x41 

typedef enum
{
  BL_GET_VERSION = 0x20U,
  BL_JUMP_TO_APP = 0x21U,
}Te_UID;

typedef enum
{
  ANS_GET_VERSION = 0x1U,
  ANS_JUMP_TO_APP = 0x0U,
  
}Te_ANSWER_SIZE;

void bootLoaderProcessing(void);

static void bootLoaderGetVersionProcessing(void);
static void bootLoaderJumpToApplication(void);
static void sendPositiveAnswer(uint8_t size);

#endif
