#include "bootLoader.h"
#include "main.h"
#include "crc.h"
#include "usart.h"
#include "iwdg.h"
#include <string.h>

#define bufferSize 255
uint8_t blBuffer[bufferSize] = {0};
uint8_t receiveComplete = 0u;
uint8_t reciveSize  = 0u;
uint8_t cmd = 0x00u;

uint8_t ansBuff[BUFFER_SIZE];

//XModem
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if(huart->Instance == USART2)
    {
        receiveComplete = 1;   
    }
    else
    {}
}

void bootLoaderProcessing(void)
{
    HAL_UART_Transmit(&huart2,"Start bootLoader Processing!\r\n",30,1000);
    HAL_UART_Receive_IT(&huart2, blBuffer, 2);
    while(1)
    {
        if(receiveComplete == 1)
        {
            receiveComplete = 0;
            cmd = blBuffer[0];
            reciveSize = blBuffer[1];
        }
        else
        {
            
        }
        switch (blBuffer[0])
        {
            case BL_GET_VERSION:
            {
                bootLoaderGetVersionProcessing();
            }break;
            case BL_JUMP_TO_APP:
            {
                bootLoaderJumpToApplication();
            }break;
            default:
            {
                memset(blBuffer,0,BUFFER_SIZE);
                HAL_UART_Receive_IT(&huart2, blBuffer, 2);
                receiveComplete = 0;
            }break;
            
        }
        
        HAL_IWDG_Refresh(&hiwdg);
    }
}
/*
return BootLoader Version
*/
static void bootLoaderGetVersionProcessing(void)
{
    //  Send Ack + answer size 
    ansBuff[0] = BL_VERSION;
    sendPositiveAnswer(ANS_GET_VERSION);
}

static void bootLoaderJumpToApplication(void)
{
    sendPositiveAnswer(ANS_JUMP_TO_APP);
    
    *(volatile uint8_t*)0x20004FFF = 0xFFU;
    *(volatile uint8_t*)0x20004FFE = 0xFFU;
    *(volatile uint8_t*)0x20004FFD = 0xFFU;
    *(volatile uint8_t*)0x20004FFC = 0xFFU;
    HAL_NVIC_SystemReset();
}

static void sendPositiveAnswer(uint8_t size)
{
    blBuffer[0] = size;
    blBuffer[1] += 0x40;
    HAL_UART_Transmit(&huart2,blBuffer,2,500);
    for(uint8_t index = 0; index <= size; index++)
    {
        blBuffer[index] = ansBuff[index];
    }
    HAL_UART_Transmit(&huart2,blBuffer,size,500);
    memset(blBuffer,0,BUFFER_SIZE);
    
    HAL_UART_Receive_IT(&huart2, blBuffer, 2);
}