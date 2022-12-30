#include "bootLoader.h"
#include "main.h"
#include "crc.h"
#include "usart.h"
#include "iwdg.h"
#include <string.h>

uint8_t Rx_data[2] = {0}, receiveBuffer[BUFFER_SIZE] = {0};
uint8_t receiveComplete = 0, Rx_indx = 0;

uint8_t ansBuffer[BUFFER_SIZE];

//XModem
/**
  * @brief  UART Interrupt Call Back
  * @note   None
  * @param  None
  * @retval None
  */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if(huart->Instance == USART2)
    {
        if (Rx_indx == 0) 
        {
            for (uint8_t i = 0; i < BUFFER_SIZE; i++) 
                receiveBuffer[i] = 0; 
        }else{} 
        if(Rx_data[0] != '\n') //if received data different from ascii 13 (enter)
        {
            receiveBuffer[Rx_indx++] = Rx_data[0];     //add data to receiveBuffer
        }
        else            //if received data = 13
        {
            Rx_indx = 0;
            receiveComplete = 1; //received complete, data is ready to read
            HAL_UART_AbortReceive_IT(&huart2);
        }
        HAL_UART_Receive_IT(&huart2, Rx_data, 1);
    }
    else
    {}
}
/**
  * @brief  Process Boot Loader Commends
  * @note   None
  * @param  None
  * @retval None
  */
void bootLoaderProcessing(void)
{
    HAL_UART_Transmit(&huart2,"Start bootLoader Processing!\r\n",30,1000);
    HAL_UART_Receive_IT(&huart2, receiveBuffer, 1);
    while(1)
    {
        if(receiveComplete == 1)
        {
            switch (receiveBuffer[0])// switch on UID
            {
                case CMD_GET_VERSION:
                {
                    bootLoaderGetVersionProcessing();
                }break;
                case CMD_JUMP_TO_APP:
                {
                    bootLoaderJumpToApplication();
                }break;
                case CMD_GET_MCU_CID:
                {
                    getMcuCidNumberProcessing();
                }break;
                case CMD_GET_RDP_STATUS:
                {
                    getMcuFlashProtectionLevel();
                }break;
                case CMD_GET_FLAHS_SIZE:
                {
                    getMcuFlashSizeinKb();
                }break;
                
                default:
                {
                    sendNegetiveAnswer();
                }break;
                
            }
        }
        else
        {
            
        }
        HAL_IWDG_Refresh(&hiwdg);
    }
}
/**
  * @brief  return boot loader version to user
  * @note   None
  * @param  None
  * @retval None
  */
static void bootLoaderGetVersionProcessing(void)
{
    //  Send Ack + answer size 
    ansBuffer[0] = BL_VERSION;
    sendPositiveAnswer(ANS_GET_VERSION);
}
/**
  * @brief  clear RAM data and software reset 
  * @note   set 0x20004FFF - 0x20004FFC ram address to 0xFF
  * @param  None
  * @retval None
  */
static void bootLoaderJumpToApplication(void)
{
    sendPositiveAnswer(ANS_JUMP_TO_APP);
    *(volatile uint8_t*)0x20004FFF = 0xFFU;
    *(volatile uint8_t*)0x20004FFE = 0xFFU;
    *(volatile uint8_t*)0x20004FFD = 0xFFU;
    *(volatile uint8_t*)0x20004FFC = 0xFFU;
    HAL_NVIC_SystemReset();
}

/**
  * @brief  Get MCU CID number and send it 
  * @note   None
  * @param  None
  * @retval None
  */
static void getMcuCidNumberProcessing(void)
{
    uint16_t cid;
	cid = (uint16_t)(DBGMCU->IDCODE) & 0x0FFF;
    ansBuffer[0] = cid >> 8;
    ansBuffer[1] = cid;
    sendPositiveAnswer(ANS_GET_MCU_CID);
}
/**
  * @brief  Return MCU Flash Protection Level
  * @note   None
  * @param  None
  * @retval None
  */
static void getMcuFlashProtectionLevel(void)
{
    uint8_t rdp_status=0;
	FLASH_OBProgramInitTypeDef  ob_handle;
	HAL_FLASHEx_OBGetConfig(&ob_handle);
	rdp_status = (uint8_t)ob_handle.RDPLevel;
    ansBuffer[0] = rdp_status;
    sendPositiveAnswer(ANS_GET_RDP_STATUS);
}

static void getMcuFlashSizeinKb(void)
{
    
    uint16_t flashSizeKb = *(const uint16_t*)FLASHSIZE_BASE;
    ansBuffer[0] = flashSizeKb >> 8;
    ansBuffer[1] = flashSizeKb;
    sendPositiveAnswer(ANS_GET_FLAHS_SIZE);
}
/*****************************************************************************/
/**
  * @brief  Send Positive Answer
  * @note   use ansBuffer to add answer to positive answer
  * @param  uint8_t size (use Te_ANSWER_SIZE)
  * @retval None
  */
static void sendPositiveAnswer(uint8_t size)
{
    receiveBuffer[0] += 0x40;
    for(uint8_t index = 0; index <= size; index++)
    {
        receiveBuffer[index + 1] = ansBuffer[index];
    }
    HAL_UART_Transmit(&huart2,receiveBuffer,size + 1,500);
    HAL_UART_Transmit(&huart2,"\n",1,500);
    readyForNewCommand();
}

/**
  * @brief  send Negetive Answert
  * @note   None
  * @param  None
  * @retval None
  */
static void sendNegetiveAnswer(void)
{
    receiveBuffer[0] = 0x7F;
    HAL_UART_Transmit(&huart2,receiveBuffer,1,500);
    HAL_UART_Transmit(&huart2,"\n",1,500);
    readyForNewCommand();
}
/**
  * @brief  Clear the buffers and enable interrupt to recevie again
  * @note   None
  * @param  None
  * @retval None
  */
static void readyForNewCommand(void)
{
    memset(receiveBuffer,0,BUFFER_SIZE);
    receiveComplete = 0;
    HAL_UART_Receive_IT(&huart2, Rx_data, 1);
}