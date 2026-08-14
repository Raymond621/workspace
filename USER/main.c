////////////////////////////////////////////////////////////////////////////////
/// @file    main.c
/// @author  AE TEAM
/// @brief   THIS FILE PROVIDES ALL THE SYSTEM FUNCTIONS.
////////////////////////////////////////////////////////////////////////////////
/// @attention
///
/// THE EXISTING FIRMWARE IS ONLY FOR REFERENCE, WHICH IS DESIGNED TO PROVIDE
/// CUSTOMERS WITH CODING INFORMATION ABOUT THEIR PRODUCTS SO THEY CAN SAVE
/// TIME. THEREFORE, MINDMOTION SHALL NOT BE LIABLE FOR ANY DIRECT, INDIRECT OR
/// CONSEQUENTIAL DAMAGES ABOUT ANY CLAIMS ARISING OUT OF THE CONTENT OF SUCH
/// HARDWARE AND/OR THE USE OF THE CODING INFORMATION CONTAINED HEREIN IN
/// CONNECTION WITH PRODUCTS MADE BY CUSTOMERS.
///
/// <H2><CENTER>&COPY; COPYRIGHT MINDMOTION </CENTER></H2>
////////////////////////////////////////////////////////////////////////////////

// Define to prevent recursive inclusion
#define _MAIN_C_

// Files includes
#include "led.h"
#include "delay.h"
#include "uart_print_loop.h"
#include "exti.h"
#include "i2c.h"
#include "stdio.h"
#include "stdlib.h"
#include "main.h"
#include "spi.h"
#include "ray.h"


////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Example_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MAIN
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MAIN_Exported_Constants
/// @{



////////////////////////////////////////////////////////////////////////////////
/// @brief  This function is main entrance.
/// @param  None.
/// @retval  0.
////////////////////////////////////////////////////////////////////////////////
char w;
uint8_t i2c_txbuffer[5]={0x00,0x01};
uint8_t i2c_rxbuffer[5];
char buf[4];
//int buf[];
//void	i2c_data_read(I2C_TypeDef* i2c,uint8_t regbuf,uint8_t *rxbuf,uint8_t len);
s32 main(void)
{
    LED_Init();
    DELAY_Init();
/****************************************************************************************************************************************
	UART Sample Code
	This function can used UART2 to send data 
	And it also have nvic to intterupt main function.
	*****************************************************************************************************************************************/	
		UART1_Loop_Init(115200);
/****************************************************************************************************************************************
	KEY EXTI Sample Code
	This function can used Key button to intterupt main function to toggle the LED.
	When you push the button it will into different IRQ channel
	*****************************************************************************************************************************************/
		EXTI_Config();
		KEY_Init();
	/****************************************************************************************************************************************
	I2C Sample Code
	i2c_data_read (I2Cx,Slave add,Received buffer,Len)
		I2Cx => select channel  , 
		Slave add => write reg address to read 
		Received buffer => save data to buffer
		Len => size of data
	I2C_WR_EepromInit()
		I2c init and Write Slave address
	*****************************************************************************************************************************************/
		SPI_FlashInit(0x300);
		I2C_WR_EepromInit();
    while(1) 
			{
        LED2_TOGGLE();//Make sure function worked
				LED3_TOGGLE();//Make sure function worked
				i2c_data_read(I2C1,0x00,i2c_rxbuffer,4);//This ffunction is to read I2C Slave data
				printf(" %x H: %x M: %x S day %x\n",i2c_rxbuffer[2],i2c_rxbuffer[1],i2c_rxbuffer[0],i2c_rxbuffer[3]);//Use debug port to see data
				DELAY_Ms(998);//delay one Sec to Read data(or use Timer
    }
	}

/// @}

/// @}

/// @}
