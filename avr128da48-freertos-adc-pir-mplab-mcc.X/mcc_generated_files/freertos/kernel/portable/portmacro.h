/*
 * FreeRTOS Kernel V10.4.3
 * Copyright (C) 2020 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * https://www.FreeRTOS.org
 * https://github.com/FreeRTOS
 *
 */
/*
© [2024] Microchip Technology Inc. and its subsidiaries.

    Subject to your compliance with these terms, you may use Microchip 
    software and any derivatives exclusively with Microchip products. 
    You are responsible for complying with 3rd party license terms  
    applicable to your use of 3rd party software (including open source  
    software) that may accompany Microchip software. SOFTWARE IS ?AS IS.? 
    NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS 
    SOFTWARE, INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT,  
    MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT 
    WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY 
    KIND WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF 
    MICROCHIP HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE 
    FORESEEABLE. TO THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP?S 
    TOTAL LIABILITY ON ALL CLAIMS RELATED TO THE SOFTWARE WILL NOT 
    EXCEED AMOUNT OF FEES, IF ANY, YOU PAID DIRECTLY TO MICROCHIP FOR 
    THIS SOFTWARE.
*/ 

#ifndef PORTMACRO_H
#define PORTMACRO_H

#ifdef __cplusplus
extern "C"
{
#endif


/*-----------------------------------------------------------
 * Port specific definitions.
 *
 * The settings in this file configure FreeRTOS correctly for the
 * given hardware and compiler.
 *
 * These settings should not be altered.
 *-----------------------------------------------------------
 */

/* Type definitions */
#define portCHAR                char
#define portFLOAT               float
#define portDOUBLE              double
#define portLONG                long
#define portSHORT               int
#define portSTACK_TYPE          uint8_t
#define portBASE_TYPE           char

#define portPOINTER_SIZE_TYPE   uint16_t

typedef portSTACK_TYPE StackType_t;
typedef signed char BaseType_t;
typedef unsigned char UBaseType_t;

typedef uint16_t TickType_t;
#define portMAX_DELAY           (TickType_t) 0xffff

/* Critical section management */
#define portENTER_CRITICAL()                       \
    {                                              \
        asm volatile("in __tmp_reg__, __SREG__");  \
        asm volatile("cli");                       \
        asm volatile("push __tmp_reg__");          \
    }

#define portEXIT_CRITICAL()                        \
    {                                              \
        asm volatile("pop __tmp_reg__");           \
        asm volatile("out __SREG__, __tmp_reg__"); \
    }

#define portDISABLE_INTERRUPTS()    asm volatile("cli" ::);
#define portENABLE_INTERRUPTS()     asm volatile("sei" ::);

/* Architecture specifics */
#define portSTACK_GROWTH        (-1)
#define portTICK_PERIOD_MS      ((float)1000 / configTICK_RATE_HZ)
#define portBYTE_ALIGNMENT      1
#define portNOP()               asm volatile("nop");

/* Kernel utilities */
extern void vPortYield(void) __attribute__((naked));
#define portYIELD() vPortYield()

extern void vPortYieldFromISR( void ) __attribute__( ( naked ) );
#define portYIELD_FROM_ISR()    vPortYieldFromISR()

/*
  Context switch function used by the tick.  This must be identical to vPortYield() from the call to vTaskSwitchContext() onwards. The only difference from vPortYield() is the tick count is incremented as the call comes from the tick ISR.
 */

void vPortYieldFromTick( void ) __attribute__( ( naked ) );

#define portTASK_FUNCTION_PROTO( vFunction, pvParameters )    void vFunction( void * pvParameters )
#define portTASK_FUNCTION( vFunction, pvParameters )          void vFunction( void * pvParameters )



#ifdef __cplusplus
}
#endif

#endif /* PORTMACRO_H */
