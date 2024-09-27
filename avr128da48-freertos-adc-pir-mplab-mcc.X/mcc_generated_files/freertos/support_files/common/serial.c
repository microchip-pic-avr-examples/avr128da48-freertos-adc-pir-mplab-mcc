/*
 * FreeRTOS Kernel V10.4.1
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
 * http://www.FreeRTOS.org
 * http://aws.amazon.com/freertos
 *
 * 1 tab == 4 spaces!
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


/* BASIC INTERRUPT DRIVEN SERIAL PORT DRIVER FOR AVR PORT. */

#include <stdlib.h>
#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"
#include "serial.h"
#include <avr/interrupt.h>
#include "mcc_generated_files/system/system.h"

#define com_WAIT					( ( TickType_t ) 1 )
#define comNO_BLOCK					( ( TickType_t ) 0 )

xComPortHandle xSerialPortInitMinimal( unsigned long ulWantedBaud, unsigned portBASE_TYPE uxQueueLength )
{
  
    /*Not implemented, the SerialPort is configured in hardware drivers */
    return NULL;
}

signed portBASE_TYPE xSerialGetChar( xComPortHandle pxPort, signed char *pcRxedChar, TickType_t xBlockTime )
{
    TickType_t currentTick;
    currentTick = xTaskGetTickCount();
    do {
        if (RTOS_UART_IsRxReady())
        {
            *pcRxedChar =  RTOS_UART_Read();
            return pdTRUE;
        }
    }while((xTaskGetTickCount() - currentTick) < xBlockTime);
   
    return pdFALSE;
}

signed portBASE_TYPE xSerialPutChar( xComPortHandle pxPort, signed char cOutChar, TickType_t xBlockTime )
{
    /* Return false if after the block time there is no room on the Tx buffer. */
    TickType_t currentTick;
    currentTick = xTaskGetTickCount();
    
    do
    {
        if (RTOS_UART_IsTxReady())
        {
        /* Send the next character queued for Tx. */
            RTOS_UART_Write(cOutChar);
            return pdPASS;
            
        }
    }while((xTaskGetTickCount() - currentTick) < xBlockTime);
   
    return pdFALSE; 
}

void vSerialClose( xComPortHandle xPort )
{
    /* Not implemented */
}

void vSerialPutString( xComPortHandle pxPort, const signed char * const pcString, unsigned short usStringLength )
{
    size_t cByteToSend;
    
    for( cByteToSend = 0; cByteToSend < usStringLength;)
    {
        if( xSerialPutChar( pxPort, pcString[cByteToSend], com_WAIT ) == pdPASS )
        {
            cByteToSend++;
        }
    }
}
