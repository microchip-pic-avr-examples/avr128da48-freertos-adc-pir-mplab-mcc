/**
 * TCB0 Generated Driver API Header File
 *
 * @file tcb0.h
 *
 * @defgroup tcb0 TCB0
 *
 * @brief This file contains the API prototypes and custom data types for the TCB0 driver.
 *
 * @version TCB0 Driver Version 1.1.5
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

#ifndef TCB0_H_INCLUDED
#define TCB0_H_INCLUDED

#include <stdint.h>
#include "../system/utils/compiler.h"
#include "rtos_timer_interface.h"

#ifdef __cplusplus
extern "C" {
#endif

extern const struct RTOS_TIMER_INTERFACE TCB0_Interface;


/**
 * @ingroup tcb0
 * @typedef void *TCB0_cb_t
 * @brief Function pointer to callback function called by the TCB. The default value is set to NULL which means that no callback function will be used.
 */
typedef void (*TCB0_cb_t)(void);

/**
 * @ingroup tcb0
 * @brief Registers a callback function to be called at capture event.
 * @param TCB0_cb_t cb - Callback function for capture event.
 * @return None.
 */
void TCB0_CaptureCallbackRegister(TCB0_cb_t cb);


/**
 * @ingroup tcb0
 * @brief Initializes the TCB module
 * @param None.
 * @return None.
 */
void TCB0_Initialize(void);
/**
 * @ingroup tcb0
 * @brief Starts the TCB counter.
 * @param None.
 * @return None.
 */
void TCB0_Start(void);
/**
 * @ingroup tcb0
 * @brief Stops the TCB counter.
 * @param None.
 * @return None.
 */
void TCB0_Stop(void);
/**
 * @ingroup tcb0
 * @brief Enables the capture interrupt for the TCB.
 * @param None.
 * @return None.
 */
void TCB0_EnableCaptInterrupt(void);
/**
 * @ingroup tcb0
 * @brief Disables the capture interrupt for the TCB.
 * @param None.
 * @return None.
 */
void TCB0_DisableCaptInterrupt(void);
/**
 * @ingroup tcb0
 * @brief Enables the overflow interrupt for the TCB.
 * @param None.
 * @return None.
 */
void TCB0_EnableOvfInterrupt(void);
/**
 * @ingroup tcb0
 * @brief Disables the overflow interrupt for the TCB.
 * @param None.
 * @return None.
 */
void TCB0_DisableOvfInterrupt(void);
/**
 * @ingroup tcb0
 * @brief Reads the 16-bit timer value of the TCB.
 * @param None.
 * @return uint16_t
 */
uint16_t TCB0_Read(void);
/**
 * @ingroup tcb0
 * @brief Writes the 16-bit timer value to the TCB. 
 * @param uint16_t timerVal - 16-bit Timer value to write for TCB interface.
 * @return None.
 */
void TCB0_Write(uint16_t timerVal);
/**
 * @ingroup tcb0
 * @brief Checks the Overflow Interrupt flag.
 * @param None.
 * @return bool.
 */
bool TCB0_IsOvfInterruptFlag(void);
/**
 * @ingroup tcb0
 * @brief Checks the Capture Interrupt flag.
 * @param None.
 * @return bool.
 */
bool TCB0_IsCaptInterruptFlag(void);

/**
 * @ingroup tcb0
 * @brief Clears the Capture Interrupt flag.
 * @param None.
 * @return None.
 */
void TCB0_ClearCaptInterruptFlag(void);
/**
 * @ingroup tcb0
 * @brief Checks if the capture interrupt is enabled.
 * @param None.
 * @return None.
 */
bool TCB0_IsCaptInterruptEnabled(void);
/**
 * @ingroup tcb0
 * @brief Clears the Overflow Interrupt flag.
 * @param None.
 * @return None.
 */
void TCB0_ClearOvfInterruptFlag(void);
/**
 * @ingroup tcb0
 * @brief Checks if the overflow interrupt is enabled.
 * @param None.
 * @return None.
 */
bool TCB0_IsOvfInterruptEnabled(void);

/**
 * @ingroup tcb0_normal
 * @brief Performs the tasks to be executed on timer events.
 * @param None.
 * @return None.
 */
void TCB0_Tasks(void);

/* Helper macros for portSAVE_CONTEXT/ portRESTORE_CONTEXT - common support for Mega-0 and AVR-Dx families */

#if defined(__AVR_HAVE_RAMPZ__)

#define portSAVE_RAMPZ()                              \
    asm volatile("in    r0, __RAMPZ__           \n\t" \
                 "push  r0                      \n\t");

#define portRESTORE_RAMPZ()                           \
    asm volatile("pop   r0                      \n\t" \
                 "out   __RAMPZ__, r0           \n\t");

#else

#define portSAVE_RAMPZ()
#define portRESTORE_RAMPZ()

#endif

/* Macro to save all the general purpose registers, the save the stack pointer
 * into the TCB.

 * The first thing we do is save the flags then disable interrupts.  This is to
 * guard our stack against having a context switch interrupt after we have already
 * pushed the registers onto the stack - causing the 32 registers to be on the
 * stack twice.

 * r1 is set to zero as the compiler expects it to be thus, however some
 * of the math routines make use of R1.
 *
 * The interrupts will have been disabled during the call to portSAVE_CONTEXT()
 * so we need not worry about reading/writing to the stack pointer.
 */

#define portSAVE_CONTEXT()                              \
    asm volatile ( "push  r0                      \n\t" \
                   "in    r0, __SREG__            \n\t" \
                   "cli                           \n\t" \
                   "push  r0                      \n\t"); \
                    portSAVE_RAMPZ();\
    asm volatile ( "push  r1                      \n\t" \
                   "clr   r1                      \n\t" \
                   "push  r2                      \n\t" \
                   "push  r3                      \n\t" \
                   "push  r4                      \n\t" \
                   "push  r5                      \n\t" \
                   "push  r6                      \n\t" \
                   "push  r7                      \n\t" \
                   "push  r8                      \n\t" \
                   "push  r9                      \n\t" \
                   "push  r10                     \n\t" \
                   "push  r11                     \n\t" \
                   "push  r12                     \n\t" \
                   "push  r13                     \n\t" \
                   "push  r14                     \n\t" \
                   "push  r15                     \n\t" \
                   "push  r16                     \n\t" \
                   "push  r17                     \n\t" \
                   "push  r18                     \n\t" \
                   "push  r19                     \n\t" \
                   "push  r20                     \n\t" \
                   "push  r21                     \n\t" \
                   "push  r22                     \n\t" \
                   "push  r23                     \n\t" \
                   "push  r24                     \n\t" \
                   "push  r25                     \n\t" \
                   "push  r26                     \n\t" \
                   "push  r27                     \n\t" \
                   "push  r28                     \n\t" \
                   "push  r29                     \n\t" \
                   "push  r30                     \n\t" \
                   "push  r31                     \n\t" \
                   "lds   r26, pxCurrentTCB       \n\t" \
                   "lds   r27, pxCurrentTCB + 1   \n\t" \
                   "in    r0, __SP_L__            \n\t" \
                   "st    x+, r0                  \n\t" \
                   "in    r0, __SP_H__            \n\t" \
                   "st    x+, r0                  \n\t" );

/*
 * Opposite to portSAVE_CONTEXT().  Interrupts will have been disabled during
 * the context save so we can write to the stack pointer.
 */

#define portRESTORE_CONTEXT()                           \
    asm volatile ( "lds   r26, pxCurrentTCB       \n\t" \
                   "lds   r27, pxCurrentTCB + 1   \n\t" \
                   "ld    r28, x+                 \n\t" \
                   "out   __SP_L__, r28           \n\t" \
                   "ld    r29, x+                 \n\t" \
                   "out   __SP_H__, r29           \n\t" \
                   "pop   r31                     \n\t" \
                   "pop   r30                     \n\t" \
                   "pop   r29                     \n\t" \
                   "pop   r28                     \n\t" \
                   "pop   r27                     \n\t" \
                   "pop   r26                     \n\t" \
                   "pop   r25                     \n\t" \
                   "pop   r24                     \n\t" \
                   "pop   r23                     \n\t" \
                   "pop   r22                     \n\t" \
                   "pop   r21                     \n\t" \
                   "pop   r20                     \n\t" \
                   "pop   r19                     \n\t" \
                   "pop   r18                     \n\t" \
                   "pop   r17                     \n\t" \
                   "pop   r16                     \n\t" \
                   "pop   r15                     \n\t" \
                   "pop   r14                     \n\t" \
                   "pop   r13                     \n\t" \
                   "pop   r12                     \n\t" \
                   "pop   r11                     \n\t" \
                   "pop   r10                     \n\t" \
                   "pop   r9                      \n\t" \
                   "pop   r8                      \n\t" \
                   "pop   r7                      \n\t" \
                   "pop   r6                      \n\t" \
                   "pop   r5                      \n\t" \
                   "pop   r4                      \n\t" \
                   "pop   r3                      \n\t" \
                   "pop   r2                      \n\t" \
                   "pop   r1                      \n\t" );\
                    portRESTORE_RAMPZ();                 \
    asm volatile ( "pop   r0                      \n\t" \
                   "out   __SREG__, r0            \n\t" \
                   "pop   r0                      \n\t" );

/*-----------------------------------------------------------*/


#ifdef __cplusplus
}
#endif

#endif /* TCB0_H_INCLUDED */