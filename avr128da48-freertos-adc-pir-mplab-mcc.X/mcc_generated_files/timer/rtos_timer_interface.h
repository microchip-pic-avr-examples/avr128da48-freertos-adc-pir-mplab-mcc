/**
 * RTOS Timer Interface Header File
 * 
 * @file rtos_timer_interface.h
 * 
 * @defgroup rtos_timer_interface RTOS_TIMER_INTERFACE
 * 
 * @brief This file contains APIs for timer module.
 *
 * @version RTOS Timer Version 1.1.0
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

#ifndef RTOS_TIMER_INTERFACE_H
#define	RTOS_TIMER_INTERFACE_H
/**
  Section: Included Files
*/
#include <stdbool.h>
#include <stdint.h>
       
/**
 @ingroup RTOS Timer Interface
 @struct RTOS_TIMER_INTERFACE
 @brief Structure containing the function pointers of RTOS Timer interface.
*/
struct RTOS_TIMER_INTERFACE 
{
    void (*Initialize)(void);
    void (*Deinitialize)(void);
    void (*Start)(void);
    void (*Stop)(void);
    void (*PeriodSet)(size_t count);
    size_t (*CounterGet)(void); 
    void (*TimerInterruptCallbackRegister)(void (* CallbackHandler)(void));
    void (*EnableTimerInterrupt)(void);
    void (*DisableTimerInterrupt)(void);
};

#endif	/* RTOS_TIMER_INTERFACE_H */
