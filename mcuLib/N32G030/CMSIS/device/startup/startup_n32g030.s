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
; Nations' name may not be used to endorse or promote products derived from
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

Stack_Size      EQU     0x00000400

                AREA    STACK, NOINIT, READWRITE, ALIGN=3
Stack_Mem       SPACE   Stack_Size
__initial_sp
                                                  
; <h> Heap Configuration
;   <o>  Heap Size (in Bytes) <0x0-0xFFFFFFFF:8>
; </h>

Heap_Size       EQU     0x00000200

                AREA    HEAP, NOINIT, READWRITE, ALIGN=3
__heap_base
Heap_Mem        SPACE   Heap_Size
__heap_limit

                PRESERVE8
                THUMB


; Vector Table Mapped to Address 0 at Reset
                AREA    RESET, DATA, READONLY
                EXPORT  __Vectors
                EXPORT  __Vectors_End
                EXPORT  __Vectors_Size

__Vectors       DCD     __initial_sp               ; Top of Stack
                DCD     Reset_Handler              ; Reset Handler
                DCD     NMI_Handler                ; NMI Handler
                DCD     HardFault_Handler          ; Hard Fault Handler
                DCD     0                          ; Reserved
                DCD     0                          ; Reserved
                DCD     0                          ; Reserved
                DCD     0                          ; Reserved
                DCD     0                          ; Reserved
                DCD     0                          ; Reserved
                DCD     0                          ; Reserved
                DCD     SVC_Handler                ; SVCall Handler
                DCD     0                          ; Reserved
                DCD     0                          ; Reserved
                DCD     PendSV_Handler             ; PendSV Handler
                DCD     SysTick_Handler            ; SysTick Handler

                ; External Interrupts
                DCD     WWDG_IRQHandler            ; Window Watchdog
                DCD     PVD_IRQHandler             ; PVD through EXTI Line 16 detect
                DCD     RTC_IRQHandler             ; RTC interrupt(through EXTI line 17/19/20 interrupt)
                DCD     MMU_IRQHandler             ; MMU
                DCD     FLASH_IRQHandler           ; Flash
                DCD     RCC_IRQHandler             ; RCC
                DCD     EXTI0_1_IRQHandler         ; EXTI Line 0.1
                DCD     EXTI2_3_IRQHandler         ; EXTI Line 2.3
                DCD     EXTI4_15_IRQHandler        ; EXTI Line 4..15
                DCD     RESERVE9_IRQHandler        ; RESERVE9
                DCD     DMA_Channel1_2_IRQHandler  ; DMA Channel 1,2
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
__Vectors_End

__Vectors_Size  EQU  __Vectors_End - __Vectors

                AREA    |.text|, CODE, READONLY
                
; Reset handler
Reset_Handler   PROC
                EXPORT  Reset_Handler             [WEAK]
                IMPORT  __main
                IMPORT  SystemInit
                LDR     R0, =SystemInit
                BLX     R0               
                LDR     R0, =__main
                BX      R0
                ENDP
                
; Dummy Exception Handlers (infinite loops which can be modified)

NMI_Handler     PROC
                EXPORT  NMI_Handler                [WEAK]
                B       .
                ENDP
HardFault_Handler\
                PROC
                EXPORT  HardFault_Handler          [WEAK]
                B       .
                ENDP
SVC_Handler     PROC
                EXPORT  SVC_Handler                [WEAK]
                B       .
                ENDP
PendSV_Handler  PROC
                EXPORT  PendSV_Handler             [WEAK]
                B       .
                ENDP
SysTick_Handler PROC
                EXPORT  SysTick_Handler            [WEAK]
                B       .
                ENDP

Default_Handler PROC

                EXPORT  WWDG_IRQHandler            [WEAK]
                EXPORT  PVD_IRQHandler             [WEAK]
                EXPORT  RTC_IRQHandler             [WEAK]
                EXPORT  MMU_IRQHandler             [WEAK]
                EXPORT  FLASH_IRQHandler           [WEAK]
                EXPORT  RCC_IRQHandler             [WEAK]
                EXPORT  EXTI0_1_IRQHandler         [WEAK]
                EXPORT  EXTI2_3_IRQHandler         [WEAK]
                EXPORT  EXTI4_15_IRQHandler        [WEAK]
                EXPORT  RESERVE9_IRQHandler        [WEAK]
                EXPORT  DMA_Channel1_2_IRQHandler  [WEAK]
                EXPORT  DMA_Channel1_2_3_4_IRQHandler [WEAK]
                EXPORT  DMA_Channel5_IRQHandler    [WEAK]
                EXPORT  TIM1_BRK_UP_TRG_COM_IRQHandler [WEAK]
                EXPORT  TIM1_CC_IRQHandler         [WEAK]
                EXPORT  DMA_Channel3_4_IRQHandler  [WEAK]
                EXPORT  TIM3_IRQHandler            [WEAK]
                EXPORT  USART2_IRQHandler          [WEAK]
                EXPORT  TIM8_BRK_UP_TRG_COM_IRQHandler [WEAK]
                EXPORT  TIM8_CC_IRQHandler         [WEAK]
                EXPORT  LPTIM_TIM6_IRQHandler      [WEAK]
                EXPORT  ADC_IRQHandler             [WEAK]
                EXPORT  SPI2_IRQHandler            [WEAK]
                EXPORT  I2C1_IRQHandler            [WEAK]
                EXPORT  I2C2_IRQHandler            [WEAK]
                EXPORT  SPI1_IRQHandler            [WEAK]
                EXPORT  HDIV_SQRT_IRQHandler       [WEAK]
                EXPORT  RAMC_PERR_IRQHandler       [WEAK]
                EXPORT  USART1_2_IRQHandler        [WEAK]
                EXPORT  LPUART_IRQHandler          [WEAK]
                EXPORT  USART1_IRQHandler          [WEAK]
                EXPORT  COMP_IRQHandler            [WEAK]

WWDG_IRQHandler                
PVD_IRQHandler                 
RTC_IRQHandler                 
MMU_IRQHandler                 
FLASH_IRQHandler               
RCC_IRQHandler                 
EXTI0_1_IRQHandler             
EXTI2_3_IRQHandler             
EXTI4_15_IRQHandler            
RESERVE9_IRQHandler                  
DMA_Channel1_2_IRQHandler                 
DMA_Channel1_2_3_4_IRQHandler 
DMA_Channel5_IRQHandler 
TIM1_BRK_UP_TRG_COM_IRQHandler 
TIM1_CC_IRQHandler             
DMA_Channel3_4_IRQHandler                 
TIM3_IRQHandler                
USART2_IRQHandler                
TIM8_BRK_UP_TRG_COM_IRQHandler 
TIM8_CC_IRQHandler             
LPTIM_TIM6_IRQHandler              
ADC_IRQHandler                 
SPI2_IRQHandler              
I2C1_IRQHandler                
I2C2_IRQHandler                
SPI1_IRQHandler                
HDIV_SQRT_IRQHandler                
RAMC_PERR_IRQHandler           
USART1_2_IRQHandler            
LPUART_IRQHandler             
USART1_IRQHandler             
COMP_IRQHandler  
                B       .

                ENDP

                ALIGN

;*******************************************************************************
; User Stack and Heap initialization
;*******************************************************************************
                 IF      :DEF:__MICROLIB
                
                 EXPORT  __initial_sp
                 EXPORT  __heap_base
                 EXPORT  __heap_limit
                
                 ELSE
                
                 IMPORT  __use_two_region_memory
                 EXPORT  __user_initial_stackheap
                 
__user_initial_stackheap

                 LDR     R0, =  Heap_Mem
                 LDR     R1, =(Stack_Mem + Stack_Size)
                 LDR     R2, = (Heap_Mem +  Heap_Size)
                 LDR     R3, = Stack_Mem
                 BX      LR

                 ALIGN

                 ENDIF

                 END
