#include "bootLoader.h"
#include "main.h"
#include "crc.h"
#include "usart.h"
#include <string.h>


uint8_t blBuffer[BUFFER_SIZE];
uint8_t ansBuff[BUFFER_SIZE];
uint8_t reciveSize  = 0u;
/*
***** START FRAM - Size - UID  - DATA - 4bit CRC - End Frame ***
*/
void bootLoaderProcessing(void)
{
//  HAL_UART_Transmit(&huart2,"Start bootLoader Processing!\r\n",30,1000);
//  while(1)
//  {
//    reciveSize = 0;
//    HAL_UART_Receive(&huart2,&blBuffer[0],1,HAL_MAX_DELAY);
//    reciveSize = blBuffer[0] - '0';
//    HAL_UART_Receive(&huart2,&blBuffer[1],reciveSize,HAL_MAX_DELAY);
//    switch (blBuffer[1])
//    {
//      case BL_GET_VERSION:
//      {
//        bootLoaderGetVersionProcessing();
//      }break;
//      default:
//      {
//        memset(blBuffer,0,BUFFER_SIZE);
//      }break;
//      
//    }
//  }
}
/*
 return BootLoader Version
*/
static void bootLoaderGetVersionProcessing(void)
{
  //  Send Ack + answer size 
  ansBuff[0] = BL_VERSION;
  sendPositiveAnswer(BL_AS_GET_VERSION);
}

static void sendPositiveAnswer(uint8_t size)
{
  blBuffer[0] = size;
  blBuffer[1] += 0x40;
  HAL_UART_Transmit(&huart2,blBuffer,2,HAL_MAX_DELAY);
  for(uint8_t index = 0; index <= size; index++)
  {
    blBuffer[index] = ansBuff[index];
  }
  HAL_UART_Transmit(&huart2,blBuffer,size,HAL_MAX_DELAY);
  memset(blBuffer,0,BUFFER_SIZE);
}