; ****************************************************************************
; Copyright (c) 2019, Nations Technologies Inc.
;
; All rights reserved.
; ****************************************************************************
;
; Redistribution and use in source and binary forms, with or without
; modification, are permitted provided that the following conditions are met:
;
; - Redistributions of source code must retain the above copyright notice,
; this list of conditions and the disclaimer below.
;
; Nations name may not be used to endorse or promote products derived from
; this software without specific prior written permission.
;
; DISCLAIMER: THIS SOFTWARE IS PROVIDED BY NATIONS "AS IS" AND ANY EXPRESS OR
; IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
; MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
; DISCLAIMED. IN NO EVENT SHALL NATIONS BE LIABLE FOR ANY DIRECT, INDIRECT,
; INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
; LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
; OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
; LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
; NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
; EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
; ****************************************************************************

; Amount of memory (in bytes) allocated for Stack
; Tailor this value to your application needs
; <h> Stack Configuration
;   <o> Stack Size (in Bytes) <0x0-0xFFFFFFFF:8>
; </h>
  
    MODULE  ?cstartup
        
        ;; Forward declaration of sections.
        SECTION CSTACK:DATA:NOROOT(3)

        SECTION .intvec:CODE:NOROOT(2)

        EXTERN  __iar_program_start
        EXTERN  SystemInit        
        PUBLIC  __vector_table

        DATA
       
__vector_table
        DCD     sfe(CSTACK)
        DCD     Reset_Handler             ; Reset Handler
        DCD     NMI_Handler               ; NMI Handler
        DCD     HardFault_Handler         ; Hard Fault Handler
        DCD     0                         ; Reserved
        DCD     0                         ; Reserved
        DCD     0                         ; Reserved
        DCD     0                         ; Reserved
        DCD     0                         ; Reserved
        DCD     0                         ; Reserved
        DCD     0                         ; Reserved
        DCD     SVC_Handler               ; SVCall Handler
        DCD     0                         ; Reserved
        DCD     0                         ; Reserved
        DCD     PendSV_Handler            ; PendSV Handler
        DCD     SysTick_Handler           ; SysTick Handler

         ; External Interrupts
        DCD     WWDG_IRQHandler           ; Window Watchdog
        DCD     PVD_IRQHandler            ; PVD through EXTI Line detect
        DCD     RTC_IRQHandler            ; RTC interrupt(through EXTI line 17/19/20 interrupt)
        DCD     MMU_IRQHandler            ; MMU
        DCD     FLASH_IRQHandler          ; Flash
        DCD     RCC_IRQHandler            ; RCC
        DCD     EXTI0_1_IRQHandler        ; EXTI Line 0.1
        DCD     EXTI2_3_IRQHandler        ; EXTI Line 2.3
        DCD     EXTI4_15_IRQHandler       ; EXTI Line 4..15
        DCD     0                         ; Reserved
        DCD     DMA_Channel1_2_IRQHandler ; DMA Channel 1,2
        DCD     DMA_Channel1_2_3_4_IRQHandler ; DMA Channel 1,2,3,4
	    DCD     DMA_Channel5_IRQHandler    ; DMA Channel 5
        DCD     TIM1_BRK_UP_TRG_COM_IRQHandler ; TIM1 Break/Update/Trigger and Commutation
        DCD     TIM1_CC_IRQHandler         ; TIM1 Capture Compare
        DCD     DMA_Channel3_4_IRQHandler  ; DMA Channel 3,4
        DCD     TIM3_IRQHandler            ; TIM3
        DCD     USART2_IRQHandler          ; USART2
        DCD     TIM8_BRK_UP_TRG_COM_IRQHandler ; TIM8 Break/Update/Trigger and Commutation
        DCD     TIM8_CC_IRQHandler         ; TIM8 Capture Compare
        DCD     LPTIM_TIM6_IRQHandler      ; LPTIM/TIM6
        DCD     ADC_IRQHandler             ; ADC
        DCD     SPI2_IRQHandler            ; SPI2
        DCD     I2C1_IRQHandler            ; I2C1
        DCD     I2C2_IRQHandler            ; I2C2
        DCD     SPI1_IRQHandler            ; SPI1
        DCD     HDIV_SQRT_IRQHandler       ; HDIV/SQRT
        DCD     RAMC_PERR_IRQHandler       ; RAMC PARITY ERR
        DCD     USART1_2_IRQHandler        ; USART1/USART2
        DCD     LPUART_IRQHandler          ; LPUART
        DCD     USART1_IRQHandler          ; USART1
        DCD     COMP_IRQHandler            ; COMP
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; Default interrupt handlers.
;;
        THUMB

        PUBWEAK Reset_Handler
        SECTION .text:CODE:REORDER:NOROOT(2)
Reset_Handler
        LDR     R0, =SystemInit
        BLX     R0
        LDR     R0, =__iar_program_start
        BX      R0
               
        PUBWEAK NMI_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
NMI_Handler
        B NMI_Handler

        PUBWEAK HardFault_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
HardFault_Handler
        B HardFault_Handler

        PUBWEAK SVC_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
SVC_Handler
        B SVC_Handler

        PUBWEAK PendSV_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
PendSV_Handler
        B PendSV_Handler

        PUBWEAK SysTick_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
SysTick_Handler
        B SysTick_Handler

        PUBWEAK WWDG_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
WWDG_IRQHandler
        B WWDG_IRQHandler

        PUBWEAK PVD_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
PVD_IRQHandler
        B PVD_IRQHandler

        PUBWEAK RTC_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
RTC_IRQHandler
        B RTC_IRQHandler

        PUBWEAK MMU_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
MMU_IRQHandler
        B MMU_IRQHandler

        PUBWEAK FLASH_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
FLASH_IRQHandler
        B FLASH_IRQHandler

        PUBWEAK RCC_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
RCC_IRQHandler
        B RCC_IRQHandler

        PUBWEAK EXTI0_1_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
EXTI0_1_IRQHandler
        B EXTI0_1_IRQHandler

        PUBWEAK EXTI2_3_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
EXTI2_3_IRQHandler
        B EXTI2_3_IRQHandler

        PUBWEAK EXTI4_15_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
EXTI4_15_IRQHandler
        B EXTI4_15_IRQHandler

        PUBWEAK DMA_Channel1_2_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
DMA_Channel1_2_IRQHandler
        B DMA_Channel1_2_IRQHandler

        PUBWEAK DMA_Channel1_2_3_4_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
DMA_Channel1_2_3_4_IRQHandler
        B DMA_Channel1_2_3_4_IRQHandler

        PUBWEAK DMA_Channel5_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
DMA_Channel5_IRQHandler
        B DMA_Channel5_IRQHandler
        
        PUBWEAK TIM1_BRK_UP_TRG_COM_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
TIM1_BRK_UP_TRG_COM_IRQHandler
        B TIM1_BRK_UP_TRG_COM_IRQHandler

        PUBWEAK TIM1_CC_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
TIM1_CC_IRQHandler
        B TIM1_CC_IRQHandler

        PUBWEAK DMA_Channel3_4_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
DMA_Channel3_4_IRQHandler
        B DMA_Channel3_4_IRQHandler

        PUBWEAK TIM3_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
TIM3_IRQHandler
        B TIM3_IRQHandler

        PUBWEAK USART2_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
USART2_IRQHandler
        B USART2_IRQHandler

        PUBWEAK TIM8_BRK_UP_TRG_COM_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
TIM8_BRK_UP_TRG_COM_IRQHandler
        B TIM8_BRK_UP_TRG_COM_IRQHandler

        PUBWEAK TIM8_CC_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
TIM8_CC_IRQHandler
        B TIM8_CC_IRQHandler

        PUBWEAK LPTIM_TIM6_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
LPTIM_TIM6_IRQHandler
        B LPTIM_TIM6_IRQHandler

        PUBWEAK ADC_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
ADC_IRQHandler
        B ADC_IRQHandler

        PUBWEAK SPI2_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
SPI2_IRQHandler
        B SPI2_IRQHandler

        PUBWEAK I2C1_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
I2C1_IRQHandler
        B I2C1_IRQHandler

        PUBWEAK I2C2_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
I2C2_IRQHandler
        B I2C2_IRQHandler

        PUBWEAK SPI1_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
SPI1_IRQHandler
        B SPI1_IRQHandler

        PUBWEAK HDIV_SQRT_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
HDIV_SQRT_IRQHandler
        B HDIV_SQRT_IRQHandler

        PUBWEAK RAMC_PERR_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
RAMC_PERR_IRQHandler
        B RAMC_PERR_IRQHandler

        PUBWEAK USART1_2_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
USART1_2_IRQHandler
        B USART1_2_IRQHandler

        PUBWEAK LPUART_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
LPUART_IRQHandler
        B LPUART_IRQHandler

        PUBWEAK USART1_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
USART1_IRQHandler
        B USART1_IRQHandler

        PUBWEAK COMP_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
COMP_IRQHandler
        B COMP_IRQHandler
        
		
        END

