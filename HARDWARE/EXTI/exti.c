////////////////////////////////////////////////////////////////////////////////
/// @file    exti.c
/// @author  AE TEAM
/// @brief   key intput.
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
#define _EXTI_C_

// Files includes
#include "exti.h"
#include "delay.h"
#include "led.h"


////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Hardware_Abstract_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup KEY
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup KEY_Exported_Functions
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @brief  initialize KEY GPIO pin
/// @note   if use jtag/swd interface GPIO PIN as KEY, need to be careful,
///         can not debug or program.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void KEY_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;

    RCC_AHBPeriphClockCmd(RCC_AHBENR_GPIOA | RCC_AHBENR_GPIOB | RCC_AHBENR_GPIOC, ENABLE);
#if defined (MINIBOARD)
    GPIO_StructInit(&GPIO_InitStruct);
    GPIO_InitStruct.GPIO_Pin  = SW1_Pin;                                 //PC13��K1
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;                               //set as pull up input
    GPIO_Init(SW1_GPIO_Port, &GPIO_InitStruct);

    GPIO_InitStruct.GPIO_Pin  = SW2_Pin;                                  //PA0,K2��WK_UP��
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPD;                               //set as pull down input
    GPIO_Init(SW2_GPIO_Port, &GPIO_InitStruct);
#endif
#if defined (EMINIBOARD)
    GPIO_StructInit(&GPIO_InitStruct);
    GPIO_InitStruct.GPIO_Pin  = SW1_Pin;                                 //PB1，K1
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPD;                               //set as pull up input
    GPIO_Init(SW1_GPIO_Port, &GPIO_InitStruct);

    GPIO_InitStruct.GPIO_Pin  = SW2_Pin;                                  //PB2, K2
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;                               //set as pull down input
    GPIO_Init(SW2_GPIO_Port, &GPIO_InitStruct);

#endif
    GPIO_InitStruct.GPIO_Pin  = SW3_Pin | SW4_Pin;
    //PB10 K3,PB11 K4

    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;                               //set as pull up input
    GPIO_Init(SW4_GPIO_Port, &GPIO_InitStruct);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Initialize exti nvic config
/// @note   if use jtag/swd interface GPIO PIN as KEY, need to be careful,
///         can not debug or program.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
static void EXTI_NVIC_Init(void)
{
    NVIC_InitTypeDef NVIC_InitStruct;

    NVIC_InitStruct.NVIC_IRQChannel = EXTI4_15_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPriority = 0x02;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);


    NVIC_InitStruct.NVIC_IRQChannel = EXTI0_1_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPriority = 0x01;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);

#if defined (EMINIBOARD)
    NVIC_InitStruct.NVIC_IRQChannel = EXTI2_3_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPriority = 0x03;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);
#endif
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Configurate EXTI NVIC parameter.
/// @note   if use jtag/swd interface GPIO PIN as KEY, need to be careful,
///         can not debug or program.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
static void EXTI_NVIC_Config(void)
{
    EXTI_InitTypeDef EXTI_InitStruct;

    RCC_APB2PeriphClockCmd(RCC_APB2ENR_SYSCFG, ENABLE);
#if defined (MINIBOARD)
    //PC.13 use EXTI line 13
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource13);
    EXTI_StructInit(&EXTI_InitStruct);
    EXTI_InitStruct.EXTI_Line = EXTI_Line13;
    EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStruct.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStruct);
    //PA.0 use EXTI line 0
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0);
    EXTI_InitStruct.EXTI_Line = EXTI_Line0;
    EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising;
    EXTI_InitStruct.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStruct);
#endif
#if defined (EMINIBOARD)
    //PC.13 use EXTI line 13
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource1);
    EXTI_StructInit(&EXTI_InitStruct);
    EXTI_InitStruct.EXTI_Line = EXTI_Line1;
    EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising;
    EXTI_InitStruct.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStruct);
    //PA.0 use EXTI line 0
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource2);
    EXTI_InitStruct.EXTI_Line = EXTI_Line2;
    EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStruct.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStruct);
#endif
    //PB.10 use EXTI line 10
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource10);
    EXTI_InitStruct.EXTI_Line = EXTI_Line10;
    EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStruct.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStruct);
    //PB.11 use EXTI line 11
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource11);
    EXTI_InitStruct.EXTI_Line = EXTI_Line11;
    EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStruct.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStruct);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  This function EXTI configuration.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void EXTI_Config(void)
{
    EXTI_NVIC_Config();
    EXTI_NVIC_Init();
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  This function is EXTI0 1 Handler
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void EXTI0_1_IRQHandler(void)
{
    u16 EXTI_DELAY = 0;
    for(EXTI_DELAY = 0; EXTI_DELAY < 1000; EXTI_DELAY++);
#if defined (MINIBOARD)
    if(WK_UP == 1) {
        LED2_TOGGLE();
    }
    EXTI_ClearFlag(EXTI_Line0);
#endif
#if defined (EMINIBOARD)
    if(KEY1 == 1) {
        LED1_TOGGLE();
    }
    EXTI_ClearFlag(EXTI_Line1);
#endif
}

#if defined (EMINIBOARD)
////////////////////////////////////////////////////////////////////////////////
/// @brief  This function is EXTI0 1 Handler
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void EXTI2_3_IRQHandler(void)
{
    u16 EXTI_DELAY = 0;
    for(EXTI_DELAY = 0; EXTI_DELAY < 1000; EXTI_DELAY++);
    if(WK_UP == 0) {
        LED2_TOGGLE();
    }
    EXTI_ClearFlag(EXTI_Line2);
}
#endif

////////////////////////////////////////////////////////////////////////////////
/// @brief  This function is EXTI4 15 Handler
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void EXTI4_15_IRQHandler(void)
{
    u16 EXTI_DELAY = 0;
    for(EXTI_DELAY = 0; EXTI_DELAY < 1000; EXTI_DELAY++);
    if(KEY3 == 0) {
        LED3_TOGGLE();
    }
#if defined (MINIBOARD)
    else if(KEY1 == 0) {
        LED1_TOGGLE();
    }
#endif
    else if(KEY4 == 0) {
        LED4_TOGGLE();
    }
    EXTI_ClearFlag(EXTI_Line10 | EXTI_Line11 | EXTI_Line13);
}


/// @}


/// @}

/// @}

