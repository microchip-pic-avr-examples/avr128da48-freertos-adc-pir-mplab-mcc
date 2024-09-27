/*
? [2022] Microchip Technology Inc. and its subsidiaries.

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

#include "mcc_generated_files/system/system.h"

#include "FreeRTOS.h"
#include "task.h"

#include "semphr.h"
#include "serial.h"

#include <string.h>

static TaskHandle_t PrintMessageHandler = NULL;
static TaskHandle_t ReadADCHandler = NULL;

static void vADC_Callback(void)
{
    /* Stop the ADC conversion when the result is outside the specified
    ADC window values */
    ADC0.COMMAND = ADC_SPCONV_bm;
    
    /* Resume the Read ADC task when the result is outside the window */
    vTaskResume(ReadADCHandler);
}

/* Priorities at which the tasks are created. */
#define mainQUEUE_READ_TEMP_TASK_PRIORITY       ( tskIDLE_PRIORITY )
#define mainQUEUE_SEND_TEMP_TASK_PRIORITY       ( tskIDLE_PRIORITY + 1 )

/* The minimum delay between ADC reading of motion detections. The 2000ms value 
is converted to ticks using the portTICK_PERIOD_MS constant. */
#define mainQUEUE_ADC_MIN_READINGS_MS         ( 2000 / portTICK_PERIOD_MS )

/* LED that is toggled periodically  */
#define mainCHECK_TASK_LED          ( 0 )

/* The number of items the queue can hold.  This is 1 as the receive task
will remove items as they are added, meaning the send task should always find
the queue empty. */
#define mainQUEUE_LENGTH            ( 1 )

/*
 * The tasks as described in the comments at the top of this file.
 */
static void prvQueueReadADCTask ( void *pvParameters );
static void prvQueuePrintMessageTask ( void *pvParameters );

/*-----------------------------------------------------------*/

/* The queue used by both tasks. */
static QueueHandle_t xQueue = NULL;

void FreeRTOS_Initialize(void)
{
    /* Callback Register for the user-defined callback function */
    ADC0_RegisterWindowCallback(vADC_Callback);
    
    /* Start the FreeRTOS application */
    FreeRTOS_AppStart();

    /* If all is well, the FreeRTOS user application will now be running, and the following
    line will never be reached */
    for( ;; );
}

void  FreeRTOS_AppStart(void)
{
   /* Create the queue. */
     xQueue = xQueueCreate( mainQUEUE_LENGTH, sizeof( double ) );
 
    if( xQueue != NULL )
    {
        /* Start the two tasks as described in the comments at the top of this
        file. */
        xTaskCreate( prvQueueReadADCTask,                   /* The function that implements the task. */
                    "ReadADC",                              /* The text name assigned to the task - for debug only as it is not used by the kernel. */
                    2* configMINIMAL_STACK_SIZE,            /* The size of the stack to allocate to the task. */
                    NULL,                                   /* The parameter passed to the task - not used in this case. */
                    mainQUEUE_READ_TEMP_TASK_PRIORITY,      /* The priority assigned to the task. */
                    &ReadADCHandler );                      /* The task handle is required, so ReadADCHandler is used. */

        xTaskCreate( prvQueuePrintMessageTask, 
                "PrintMessage", 
                2* configMINIMAL_STACK_SIZE, 
                NULL, 
                mainQUEUE_SEND_TEMP_TASK_PRIORITY, 
                &PrintMessageHandler );

        /* Start the tasks and timer running. */
        vTaskStartScheduler();
    }

    /* If all is well, the scheduler will now be running, and the following
    line will never be reached.  If the following line does execute, then
    there was either insufficient FreeRTOS heap memory available for the idle
    and/or timer tasks to be created, or vTaskStartScheduler() was called from
    User mode.  See the memory management section on the FreeRTOS web site for
    more details on the FreeRTOS heap http://www.freertos.org/a00111.html.  The
    mode from which main() is called is set in the C start up code and must be
    a privileged mode (not user mode). */
    for( ;; );
}

static void prvQueueReadADCTask( void *pvParameters )
{
uint8_t ulReadADC = 0;

    /* Remove compiler warning about unused parameter. */
    ( void ) pvParameters;

    for( ;; )
    {
        /* Clear the flag */
        ulReadADC = 0;
        
        /* Place this task in the blocked state until it is time to run again. */
        vTaskDelay(mainQUEUE_ADC_MIN_READINGS_MS);
        
        /* Start the ADC Conversion and put the task in the suspend state
         until the result of the ADC is outside the Window Thresholds*/
        ADC0_StartConversion(ADC_MUXPOS_AIN4_gc);  
        
        /* Suspend the ReadADC Task until the ADC Outside Window Interrupt*/
        vTaskSuspend(NULL); 
        
        /* Resume the Print message Task. It happens the ADC Outside Window 
        Interrupt occurs*/
        vTaskResume(PrintMessageHandler);
        
        /* Set the flag */
        ulReadADC = 1;
        
        /* Send to the queue - causing the queue receive task to unblock and
        print a message in the serial terminal. */
        xQueueSend( xQueue, &ulReadADC, portMAX_DELAY );
    }
}
/*-----------------------------------------------------------*/

static void prvQueuePrintMessageTask( void *pvParameters )
{
uint16_t ulSendADC=0;
char temp_string[50];

    /* Remove compiler warning about unused parameter. */
    ( void ) pvParameters;

    for( ;; )
    {
        /* Wait until something arrives in the queue - this task will block
        indefinitely provided INCLUDE_vTaskSuspend is set to 1 in
        FreeRTOSConfig.h. */
        xQueueReceive( xQueue, &ulSendADC, portMAX_DELAY );

        /*  To get here something must have been received from the queue, but
        is it the expected value?  If it is, print the message. */
        if(ulSendADC == 1)
        {
            sprintf(temp_string, "Motion detected!\n\r");
            vSerialPutString( NULL, (signed char*) temp_string, strlen(temp_string) );
        }
        
        /* Suspend the Print Message Task until the next motion is detected */
        vTaskSuspend(NULL);
    }
}





