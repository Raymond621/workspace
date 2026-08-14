/**
 * @file    startup_mm32f0140_gcc.S
 * @brief   GCC startup file for MindMotion MM32F0140 (Cortex-M0).
 *
 * Provides the initial vector table, the reset handler (copies .data,
 * zeroes .bss, calls SystemInit then main) and weak default interrupt
 * handlers. The vector layout mirrors the vendor IAR/KEIL startup files.
 */

    .syntax unified
    .cpu cortex-m0
    .fpu softvfp
    .thumb

.global g_pfnVectors
.global Default_Handler

/* Symbols provided by the linker script. */
.word _sidata     /* Start of .data init values in FLASH. */
.word _sdata      /* Start of .data in RAM.               */
.word _edata      /* End of .data in RAM.                 */
.word _sbss       /* Start of .bss in RAM.                */
.word _ebss       /* End of .bss in RAM.                  */

    .section .text.Reset_Handler
    .weak Reset_Handler
    .type Reset_Handler, %function
Reset_Handler:
    ldr   r0, =_estack
    mov   sp, r0

    /* Copy the data segment initializers from flash to SRAM. */
    ldr   r0, =_sdata
    ldr   r1, =_edata
    ldr   r2, =_sidata
    movs  r3, #0
    b     LoopCopyDataInit

CopyDataInit:
    ldr   r4, [r2, r3]
    str   r4, [r0, r3]
    adds  r3, r3, #4

LoopCopyDataInit:
    adds  r4, r0, r3
    cmp   r4, r1
    bcc   CopyDataInit

    /* Zero fill the .bss segment. */
    ldr   r2, =_sbss
    ldr   r4, =_ebss
    movs  r3, #0
    b     LoopFillZerobss

FillZerobss:
    str   r3, [r2]
    adds  r2, r2, #4

LoopFillZerobss:
    cmp   r2, r4
    bcc   FillZerobss

    /* Call the clock system initialization function. */
    bl    SystemInit
    /* Call static constructors (no-op for C, kept for completeness). */
    bl    __libc_init_array
    /* Call the application's entry point. */
    bl    main
    bx    lr
    .size Reset_Handler, .-Reset_Handler

/**
 * @brief Default interrupt handler: an infinite loop used for every
 *        unimplemented vector.
 */
    .section .text.Default_Handler,"ax",%progbits
Default_Handler:
Infinite_Loop:
    b     Infinite_Loop
    .size Default_Handler, .-Default_Handler

/* The vector table. */
    .section .isr_vector,"a",%progbits
    .type g_pfnVectors, %object
    .size g_pfnVectors, .-g_pfnVectors

g_pfnVectors:
    .word _estack
    .word Reset_Handler
    .word NMI_Handler
    .word HardFault_Handler
    .word 0
    .word 0
    .word 0
    .word 0
    .word 0
    .word 0
    .word 0
    .word SVC_Handler
    .word 0
    .word 0
    .word PendSV_Handler
    .word SysTick_Handler

    /* External Interrupts */
    .word WWDG_IWDG_IRQHandler                 /*  0 */
    .word PVD_VDT_IRQHandler                    /*  1 */
    .word 0                                     /*  2 Reserved */
    .word FLASH_IRQHandler                      /*  3 */
    .word RCC_IRQHandler                        /*  4 */
    .word EXTI0_1_IRQHandler                    /*  5 */
    .word EXTI2_3_IRQHandler                    /*  6 */
    .word EXTI4_15_IRQHandler                   /*  7 */
    .word HWDIV_IRQHandler                      /*  8 */
    .word DMA1_Channel1_IRQHandler             /*  9 */
    .word DMA1_Channel2_3_IRQHandler           /* 10 */
    .word DMA1_Channel4_5_IRQHandler           /* 11 */
    .word ADC_COMP_IRQHandler                   /* 12 */
    .word TIM1_BRK_UP_TRG_COM_IRQHandler        /* 13 */
    .word TIM1_CC_IRQHandler                    /* 14 */
    .word TIM2_IRQHandler                       /* 15 */
    .word TIM3_IRQHandler                       /* 16 */
    .word 0                                     /* 17 Reserved */
    .word 0                                     /* 18 Reserved */
    .word TIM14_IRQHandler                      /* 19 */
    .word 0                                     /* 20 Reserved */
    .word TIM16_IRQHandler                      /* 21 */
    .word TIM17_IRQHandler                      /* 22 */
    .word I2C1_IRQHandler                       /* 23 */
    .word 0                                     /* 24 Reserved */
    .word SPI1_IRQHandler                       /* 25 */
    .word SPI2_IRQHandler                       /* 26 */
    .word UART1_IRQHandler                      /* 27 */
    .word UART2_IRQHandler                      /* 28 */
    .word UART3_IRQHandler                      /* 29 */
    .word FLEX_CAN_IRQHandler                   /* 30 */
    .word 0                                     /* 31 Reserved */

/*
 * Provide weak aliases for each interrupt handler to Default_Handler.
 * Any function with the same name will override these definitions.
 */
    .macro def_irq_handler handler_name
    .weak \handler_name
    .thumb_set \handler_name, Default_Handler
    .endm

    def_irq_handler NMI_Handler
    def_irq_handler HardFault_Handler
    def_irq_handler SVC_Handler
    def_irq_handler PendSV_Handler
    def_irq_handler SysTick_Handler
    def_irq_handler WWDG_IWDG_IRQHandler
    def_irq_handler PVD_VDT_IRQHandler
    def_irq_handler FLASH_IRQHandler
    def_irq_handler RCC_IRQHandler
    def_irq_handler EXTI0_1_IRQHandler
    def_irq_handler EXTI2_3_IRQHandler
    def_irq_handler EXTI4_15_IRQHandler
    def_irq_handler HWDIV_IRQHandler
    def_irq_handler DMA1_Channel1_IRQHandler
    def_irq_handler DMA1_Channel2_3_IRQHandler
    def_irq_handler DMA1_Channel4_5_IRQHandler
    def_irq_handler ADC_COMP_IRQHandler
    def_irq_handler TIM1_BRK_UP_TRG_COM_IRQHandler
    def_irq_handler TIM1_CC_IRQHandler
    def_irq_handler TIM2_IRQHandler
    def_irq_handler TIM3_IRQHandler
    def_irq_handler TIM14_IRQHandler
    def_irq_handler TIM16_IRQHandler
    def_irq_handler TIM17_IRQHandler
    def_irq_handler I2C1_IRQHandler
    def_irq_handler SPI1_IRQHandler
    def_irq_handler SPI2_IRQHandler
    def_irq_handler UART1_IRQHandler
    def_irq_handler UART2_IRQHandler
    def_irq_handler UART3_IRQHandler
    def_irq_handler FLEX_CAN_IRQHandler

    .end
