/*
	FreeRTOS.org V6.0.4 - Copyright (C) 2003-2009 Richard Barry.

	This file is part of the FreeRTOS.org distribution.

	FreeRTOS.org is free software; you can redistribute it and/or modify it
	under the terms of the GNU General Public License (version 2) as published
	by the Free Software Foundation and modified by the FreeRTOS exception.

	FreeRTOS.org is distributed in the hope that it will be useful,	but WITHOUT
	ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
	FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
	more details.

	You should have received a copy of the GNU General Public License along
	with FreeRTOS.org; if not, write to the Free Software Foundation, Inc., 59
	Temple Place, Suite 330, Boston, MA  02111-1307  USA.

	A special exception to the GPL is included to allow you to distribute a
	combined work that includes FreeRTOS.org without being obliged to provide
	the source code for any proprietary components.  See the licensing section
	of http://www.FreeRTOS.org for full details.


	***************************************************************************
	*                                                                         *
	* Get the FreeRTOS eBook!  See http://www.FreeRTOS.org/Documentation      *
	*                                                                         *
	* This is a concise, step by step, 'hands on' guide that describes both   *
	* general multitasking concepts and FreeRTOS specifics. It presents and   *
	* explains numerous examples that are written using the FreeRTOS API.     *
	* Full source code for all the examples is provided in an accompanying    *
	* .zip file.                                                              *
	*                                                                         *
	***************************************************************************

	1 tab == 4 spaces!

	Please ensure to read the configuration and relevant port sections of the
	online documentation.

	http://www.FreeRTOS.org - Documentation, latest information, license and
	contact details.

	http://www.SafeRTOS.com - A version that is certified for use in safety
	critical systems.

	http://www.OpenRTOS.com - Commercial support, development, porting,
	licensing and training services.
*/

/**
 * Creates all the demo application tasks and co-routines, then starts the
 * scheduler.
 *
 * Main. c also creates a task called "Print".  This only executes every
 * five seconds but has the highest priority so is guaranteed to get
 * processor time.  Its main function is to check that all the other tasks
 * are still operational.  Nearly all the tasks in the demo application
 * maintain a unique count that is incremented each time the task successfully
 * completes its function.  Should any error occur within the task the count is
 * permanently halted.  The print task checks the count of each task to ensure
 * it has changed since the last time the print task executed.  If any count is
 * found not to have changed the print task displays an appropriate message.
 * If all the tasks are still incrementing their unique counts the print task
 * displays an "OK" message.
 *
 * The LED flash tasks do not maintain a count as they already provide visual
 * feedback of their status.
 *
 * The print task blocks on the queue into which messages that require
 * displaying are posted.  It will therefore only block for the full 5 seconds
 * if no messages are posted onto the queue.
 *
 * Main. c also provides a demonstration of how the trace visualisation utility
 * can be used, and how the scheduler can be stopped.
 *
 * \page MainC main.c
 * \ingroup DemoFiles
 * <HR>
 */

/* System headers. */
#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <stdlib.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <errno.h>
#include <unistd.h>

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "croutine.h"
#include "partest.h"

/*-----------------------------------------------------------*/

xQueueHandle GlobalQueueHandle;	//defining global variable as refrence to queue


void sender_task(void* p)
{
    int i=0;
    while(1) {
	/*
	  xQueueSend(QueueHandle, *dataToSend, timeout)
	  will send data into the queue and wait for timeout if queue is full before sending data again to queue
	*/
        if ( ! xQueueSend(GlobalQueueHandle, &i, 1000)) {
		printf("Failed to send to queue\n");
	}
	printf("Sending  %d to queue\n", i);
	++i;
        vTaskDelay(3000);
    }
}

void receiver_task(void* p)
{
    int rx_int=0;
    while(1) {
	/* 
	  xQueueReceive(QueueHandle, *buffer, timetowait) 
	  will wait for data to be put in queue for the time specified, if it didn't get any data then will go to sleep
	*/
	if (xQueueReceive(GlobalQueueHandle, &rx_int, 1000)) {
		printf("Received %d\n", rx_int);
	}
	else {
		printf("Receive Failed\n");
	}
       // vTaskDelay(1000);
    }
}
int main()
{


    /*
      Queues are used to pass message between tasks or tasks and ISRs
      xQueueCreate(LenghtOfQueue, sizeofQueueItem);
     */
    GlobalQueueHandle = xQueueCreate(3, sizeof(int));
    if (GlobalQueueHandle == NULL) {
        printf("Failed to create queue\n");
    }

    /*
    Create task
    xtaskCreate(TaskName, NameOfTask, StackSizeOfTask, ParametersToTask, TaskPriority, TaskHandle)    
    */
    xTaskCreate(sender_task, (signed char*)"tx1", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
    xTaskCreate(receiver_task, (signed char*)"tx2", configMINIMAL_STACK_SIZE, NULL, 1, NULL);

    /*Start schedular*/
    vTaskStartScheduler();

    return -1;
}

