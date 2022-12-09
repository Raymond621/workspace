////////////////////////////////////////////////////////////////////////////////
/// @file    uart_print_loop.c
/// @author  AE TEAM
/// @brief   THIS FILE PROVIDES ALL THE SYSTEM FUNCTIONS.
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
#define _UART_PRINT_LOOP_C_

// Files includes
#include "uart_print_loop.h"


////////////////////////////////////////////////////////////////////////////////
/// @brief  UART GPIO initialization
/// @param  None
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void UART1_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;

    RCC_AHBPeriphClockCmd(RCC_AHBENR_GPIOA, ENABLE);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_1); //baba  UART2 GPIOA 2
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_1); //baba  UART2 GPIOA 3
		GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_1); //baba  UART1 GPIOA 9
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_1);//baba  UART1 GPIOA 10
    //UART2_TX   GPIOA.2
    GPIO_StructInit(&GPIO_InitStruct);
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2;//baba
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStruct);

    //UART1_RX    GPIOA.3
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3;//baba
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOA, &GPIO_InitStruct);
	
		//UART2_TX 		GPIOA.9
		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;//baba
		GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStruct);
		
		//UART2_RX  	GPIOA.10
		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;//baba
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOA, &GPIO_InitStruct);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  UART loop initialization
/// @note   None.
/// @param  baudrate: Baud rate
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void UART1_Loop_Init(u32 baudrate)
{
    UART_InitTypeDef UART_InitStruct;
    RCC_APB2PeriphClockCmd(RCC_APB2ENR_UART1, ENABLE);
		RCC_APB1PeriphClockCmd(RCC_APB1ENR_UART2, ENABLE);
    UART1_GPIO_Init();
    UART_StructInit(&UART_InitStruct);
    UART_InitStruct.BaudRate = baudrate;
    //The word length is in 8-bit data format.
    UART_InitStruct.WordLength = UART_WordLength_8b;
    UART_InitStruct.StopBits = UART_StopBits_1;
    //No even check bit.
    UART_InitStruct.Parity = UART_Parity_No;
    //No hardware data flow control.
    UART_InitStruct.HWFlowControl = UART_HWFlowControl_None;
    UART_InitStruct.Mode = UART_Mode_Rx | UART_Mode_Tx;

    UART_Init(UART2, &UART_InitStruct);
		UART_Init(UART1, &UART_InitStruct);
	
		NVIC_InitTypeDef NVIC_InitStructure;
		UART_ITConfig(UART2, UART_IT_RXIEN, ENABLE);//中斷
		NVIC_InitStructure.NVIC_IRQChannel = UART2_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPriority = 3;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);
		
		UART_Cmd(UART2, ENABLE);
		UART_Cmd(UART1, ENABLE);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Output received data.
/// @note   None.
/// @param  uartx: UART1/UART2
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void UART_RxTest(UART_TypeDef* uartx)
{
    u8 temp;
    temp = Receice_Byte(uartx);
    if(temp != 0) {
        UART1_Send_Byte(temp);
    }
}
////////////////////////////////////////////////////////////////////////////////
/// @brief  UART received data.
/// @note   None.
/// @param  uartx: UART1/UART2
/// @retval temp :data.
////////////////////////////////////////////////////////////////////////////////
u8 Receice_Byte(UART_TypeDef* uartx)
{
    u8 temp;
 //  while(1) {
//        if(UART_GetITStatus(uartx, UART_IT_RXIEN)) {
            temp = (u8)UART_ReceiveData(uartx);
            UART_ClearITPendingBit(uartx, UART_IT_RXIEN);
 //          break;
 //      }
 //   }
   if(temp == 0xd) {
        return 0;
    }
    return temp;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  UART send byte.
/// @note   None.
/// @param  dat(A byte data).
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void UART1_Send_Byte(u8 dat)
{
    UART_SendData(UART2, dat);
		UART_SendData(UART1, dat);
    while(!UART_GetFlagStatus(UART2, UART_FLAG_TXEPT));
		while(!UART_GetFlagStatus(UART1, UART_FLAG_TXEPT));
}
void UART2_IRQHandler(void)  //中?服?函?

{

    u8 Res;

    if(UART_GetITStatus(UART2, UART_IT_RXIEN)  != RESET)  //接收中??生

    {

        UART_ClearITPendingBit(UART2,UART_IT_RXIEN);//清中??志

        Res =UART_ReceiveData(UART2);

        UART_SendData (UART2,Res); 

    }

}


