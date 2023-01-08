#include "bootLoader.h"
#include "main.h"
#include "crc.h"
#include "usart.h"
#include "iwdg.h"
#include <string.h>

uint8_t receiveTempBuffer[BUFFER_SIZE] = {0};
uint8_t usart_buffer[BUFFER_SIZE] = {0};
uint8_t ansBuffer[BUFFER_SIZE];

uint8_t receiveComplete = 0;
uint8_t receiveLength = 0;
/**
* @brief  UART Interrupt Call Back
* @note   None
* @param  None
* @retval None
*/
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
  if(huart->Instance == USART2)
  {
    memcpy(usart_buffer,receiveTempBuffer,Size);  
    receiveComplete = 1;
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
void boot_loader_processing(void)
{
  HAL_UARTEx_ReceiveToIdle_DMA(&huart2,receiveTempBuffer,BUFFER_SIZE);
  __HAL_DMA_DISABLE_IT(huart2.hdmarx, DMA_IT_HT);
  while(1)
  {
    if(receiveComplete == 1)
    {
      receiveLength = usart_buffer[0];
      switch (usart_buffer[1])// switch on UID
      {
        case CMD_GET_VERSION:
        {
          get_version();
        }break;
        case CMD_JUMP_TO_APP:
        {
          jump_to_application();
        }break;
        case CMD_GET_MCU_DEVICE_ID_CODE:
        {
          get_mcu_device_id_code();
        }break;
        case CMD_GET_RDP_STATUS:
        {
          get_mcu_flash_protection_level();
        }break;
        case CMD_GET_FLAHS_SIZE:
        {
          get_mcu_flash_size_in_kb();
        }break;
        case CMD_GET_MCU_UNIQUE_ID_CODE:
        {
          get_mcu_unique_id_code();
        }break;
        case CMD_FLASH_ERASE:
        {
          flash_erase_process();
        }break;
        
        case CMD_FLASH_PROGRAM:
        {
          flash_program_process();
        }break;
        
        default:
        {
          send_negetive_answer();
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
static void get_version(void)
{
  
  
  if(clculate_crc() == CRC_OK)
  {
    //  Send Ack + answer size 
    ansBuffer[0] = BL_MAJOR_VERSION;
    ansBuffer[1] = BL_MINOR_VERSION;
    send_positive_answer(ANS_GET_VERSION);
  }
  else
  {
    send_negetive_answer();
  }
  
}
/**
* @brief  clear RAM data and software reset 
* @note   set 0x20004FFF - 0x20004FFC ram address to 0xFF
* @param  None
* @retval None
*/
static void jump_to_application(void)
{
   if(clculate_crc() == CRC_OK)
   {
    send_positive_answer(ANS_JUMP_TO_APP);
    *(volatile uint8_t*)0x20004FFF = 0xFFU;
    *(volatile uint8_t*)0x20004FFE = 0xFFU;
    *(volatile uint8_t*)0x20004FFD = 0xFFU;
    *(volatile uint8_t*)0x20004FFC = 0xFFU;
    HAL_NVIC_SystemReset();
   }
   else
   {
     send_negetive_answer();
   }
}

/**
* @brief  Get MCU device Id number and send it 
* @note   None
* @param  None
* @retval None
*/
static void get_mcu_device_id_code(void)
{
  if(clculate_crc() == CRC_OK)
  {
    uint32_t cid;//0xE004 2000
    cid = HAL_GetDEVID();
    ansBuffer[0] = cid >> 24;
    ansBuffer[1] = cid >> 16;
    ansBuffer[2] = cid >> 8;
    ansBuffer[3] = cid;
    send_positive_answer(ANS_GET_MCU_DEVICE_ID_CODE);
  }
  else
  {
    send_negetive_answer();
  }
  
}

/**
* @brief  Get MCU Unique Id number and send it 
* @note   None
* @param  None
* @retval None
*/
static void get_mcu_unique_id_code(void)
{
  if(clculate_crc() == CRC_OK)
  {
    uint32_t uid_0 = HAL_GetUIDw0();
    uint32_t uid_1 = HAL_GetUIDw1();
    uint32_t uid_2 = HAL_GetUIDw2();
    
    ansBuffer[0] = uid_0 >> 24;
    ansBuffer[1] = uid_0 >> 16;
    ansBuffer[2] = uid_0 >> 8;
    ansBuffer[3] = uid_0;
    
    ansBuffer[4] = uid_1 >> 24;
    ansBuffer[5] = uid_1 >> 16;
    ansBuffer[6] = uid_1 >> 8;
    ansBuffer[7] = uid_1;
    
    ansBuffer[8 ] = uid_2 >> 24;
    ansBuffer[9 ] = uid_2 >> 16;
    ansBuffer[10] = uid_2 >> 8;
    ansBuffer[11] = uid_2;
    
    send_positive_answer(ANS_GET_MCU_UNIQUE_ID_CODE);
  }
  else
  {
    send_negetive_answer();
  }
}
/**
* @brief  Return MCU Flash Protection Level
* @note   None
* @param  None
* @retval None
*/
static void get_mcu_flash_protection_level(void)
{
  if(clculate_crc() == CRC_OK)
  {
    uint8_t rdp_status=0;
    FLASH_OBProgramInitTypeDef  ob_handle;
    HAL_FLASHEx_OBGetConfig(&ob_handle);
    rdp_status = (uint8_t)ob_handle.RDPLevel;
    ansBuffer[0] = rdp_status;
    send_positive_answer(ANS_GET_RDP_STATUS);
  }
  else
  {
    send_negetive_answer();
  }
}
/**
* @brief  Return MCU Flash Size
* @note   None
* @param  None
* @retval None
*/
static void get_mcu_flash_size_in_kb(void)
{
  if(clculate_crc() == CRC_OK)
  {
    uint16_t flashSizeKb = *(const uint16_t*)FLASHSIZE_BASE;
    ansBuffer[0] = flashSizeKb >> 8;
    ansBuffer[1] = flashSizeKb;
    send_positive_answer(ANS_GET_FLAHS_SIZE);
  }
  else
  {
    send_negetive_answer();
  }
}
/**
* @brief  Erase Flash page
* @note   None
* @param  None
* @retval None
*/
static void flash_erase_process(void)
{
  if(clculate_crc() == CRC_OK)
  {
    uint32_t page_address = 0;
    uint32_t page_number = 0;
    uint32_t error = 0;
    FLASH_EraseInitTypeDef flash;
    HAL_StatusTypeDef state = HAL_ERROR;
    page_address = (usart_buffer[2] << 24) |
                   (usart_buffer[3] << 16) |
                   (usart_buffer[4] << 8)  |
                   (usart_buffer[5] << 0);
    
    page_number = usart_buffer[6];
    
    flash.TypeErase = FLASH_TYPEERASE_PAGES;
    flash.PageAddress = page_address;
    flash.NbPages = page_number;
    HAL_FLASH_Unlock();
    state = HAL_FLASHEx_Erase(&flash,&error);
    HAL_FLASH_Lock();
    if(state== HAL_OK)
    {
      ansBuffer[0] = error >> 24;
      ansBuffer[1] = error >> 16;
      ansBuffer[2] = error >> 8;
      ansBuffer[3] = error;
      send_positive_answer(ANS_FLASH_ERASE);
    }
    else
    {
      send_negetive_answer();
    }
  }
  else
  {
    send_negetive_answer();
  }
}

/**
* @brief  Program Flash page
* @note   None
* @param  None
* @retval None
* @frame : ------------------------------------------------------
*          [len][cmd][data_len][address 0-3][data 0-239][crc 0-3]
*             0   1      2         3-4-5-6     7--
*          -------------------------------------------------------
*/
static void flash_program_process(void)
{
    uint32_t address = 0;
    uint8_t data_length = 0;
    uint32_t * p_data;
  if(clculate_crc() == CRC_OK)
  {
    data_length = (usart_buffer[2]/4);
    address = (((usart_buffer[3] << 24) |
               (usart_buffer[4] << 16) |
               (usart_buffer[5] << 8)  |
               (usart_buffer[6] << 0)) + 
               APPLICATION_BASE_ADDRESS);
    
    p_data = ((uint32_t*)(&usart_buffer[7]));
     
    HAL_FLASH_Unlock();
    for(int i = 0; i < data_length ; i++)
    {
      HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD,address,*p_data);
      p_data++;
        address += 0x04u;
      if(1)
      {
        
      }
      else
      {
        //send_negetive_answer();
        //return 0;
      }
    }
    HAL_FLASH_Lock();
    send_positive_answer(ANS_FLASH_PROGRAM); 
    //return 0;
  }
  else
  {
    //send_negetive_answer();
  }
}


/*****************************************************************************/
/**
* @brief  Send Positive Answer
* @note   use ansBuffer to add answer to positive answer
* @param  uint8_t size (use Te_ANSWER_SIZE)
* @retval None
*/
static void send_positive_answer(uint8_t size)
{
  uint8_t msg_size = size + 2;// +2 for msg size and ack in sendig buffer
  usart_buffer[0] = msg_size;
  usart_buffer[1] += 0x40;
  for(uint8_t index = 0; index < msg_size; index++)
  {
    usart_buffer[index + 2] = ansBuffer[index];
  }
  HAL_UART_Transmit(&huart2,usart_buffer,msg_size,500);
  ready_for_new_command();
}

/**
* @brief  send Negetive Answert
* @note   None
* @param  None
* @retval None
*/
static void send_negetive_answer(void)
{
  usart_buffer[0] = 2;
  usart_buffer[1] = 0x7F;
  HAL_UART_Transmit(&huart2,usart_buffer,2,500);
  ready_for_new_command();
}
/**
* @brief  Clear the buffers and enable interrupt to recevie again
* @note   None
* @param  None
* @retval None
*/
static void ready_for_new_command(void)
{
  memset(usart_buffer,0,BUFFER_SIZE);
  receiveComplete = 0;
  receiveLength = 0;
  HAL_UARTEx_ReceiveToIdle_DMA(&huart2,receiveTempBuffer,BUFFER_SIZE);
}
/**
* @brief  Check The CRC
* @note   None
* @param  None
* @retval None
*/
static Te_CRC_Check clculate_crc(void)
{
  Te_CRC_Check crc_status = CRC_NOT_OK;
  uint8_t crc_index = receiveLength - 4u;
  uint8_t data_length = receiveLength - 5u;
  uint32_t calc_crc_value=0xff;
  uint32_t receive_crc_value=0xff;
  
  for (uint32_t i=0 ; i < data_length ; i++)
  {
    uint32_t i_data = usart_buffer[i + 1];
    calc_crc_value = HAL_CRC_Accumulate(&hcrc, &i_data, 1);
  }
  __HAL_CRC_DR_RESET(&hcrc);
  receive_crc_value = (usart_buffer[crc_index  ] << 24) |
                      (usart_buffer[crc_index+1] << 16) |
                      (usart_buffer[crc_index+2] << 8)  |
                      (usart_buffer[crc_index+3] << 0);
  
  if(receive_crc_value == calc_crc_value)
  {
    crc_status = CRC_OK;
  }
  else
  {
    crc_status = CRC_NOT_OK;
  }
  return crc_status;
}