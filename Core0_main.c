#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"
#include "xil_printf.h"
#include "xparameters.h"
#include "xil_cache.h"
#include <stdio.h>
#include "xil_types.h"
#include "xstatus.h"
#include "xil_exception.h"
#include "xil_assert.h"
#include <sleep.h>

static void prvSerialTask( void *pvParameters );
static void prvSerialTask2( void *pvParameters );
static TaskHandle_t xSerialTask;
static TaskHandle_t xSerialTask2;
long RxtaskCntr = 0;

u32 *flag = (u32 *) 0xFFFF0000;

int main( void )
{
	Xil_DCacheDisable();
	flag[0] = 1; // 0 = None of the Serial task will work
	xTaskCreate( 	prvSerialTask, ( const char * ) "Serial", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1,	&xSerialTask );
	xTaskCreate( 	prvSerialTask2, ( const char * ) "Serial2", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, &xSerialTask2 );
	/* Start the tasks and timer running. */
	vTaskStartScheduler();
	/* If all is well, the scheduler will now be running, and the following line will never be reached. */
	for( ;; );
}

/*-----------------------------------------------------------*/
static void prvSerialTask( void *pvParameters )
{
	for( ;; )
	{
		if (*flag == 1)
		{
			flag[0] = 0;
			xil_printf( "Printing from Task #1 Core #0 %d\r\n",*flag + 1);
			flag[0] = 2;
			taskYIELD();//This allows task1 to leave the CPU so task2 can run
		}
	}
}

static void prvSerialTask2( void *pvParameters )
{
	for( ;; )
	{
		if (*flag == 2)
		{
			flag[0] = 0;
			xil_printf( "Printing from Task #2 Core #0 %d\r\n",*flag + 2);
			//flag[0] = 1;
			flag[0] = 3;
			taskYIELD();//This allows task2 to leave the CPU so task3 and task4 on Core 2 can run
		}
	}
}




