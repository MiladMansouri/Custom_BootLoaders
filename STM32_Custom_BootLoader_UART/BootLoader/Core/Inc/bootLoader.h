#ifndef __BOOT_LOADER_H
#define __BOOT_LOADER_H
#include "stdint.h"

#define BUFFER_SIZE 255
#define BL_VERSION 0x41 

typedef enum
{
  BL_GET_VERSION = 0x20,
  
}Te_UID;

typedef enum
{
  BL_AS_GET_VERSION = 0x1,
  
}Te_ANSWER_SIZE;

void bootLoaderProcessing(void);

static void bootLoaderGetVersionProcessing(void);


static void sendPositiveAnswer(uint8_t size);

#endif
