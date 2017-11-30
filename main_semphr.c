#include <stdio.h>
#include <stdlib.h>

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h" //Header file for semaphore related functions


/* Declaring semaphore*/
xSemaphoreHandle gatekeeper=0;

void access_resource()
{
}

void mytask1(void *p) {
	while(1) {
		/*
		  xSemaphoreTake(semaphoreHandle, timeout)
		  acquires a semaphore
		*/
		if (xSemaphoreTake(gatekeeper, 1000)) {
			puts("mytask1 has got access\n");
			access_resource();
			/*
			  xSemaphoreGive(semaphoreHandle)
			  releases the semaphore
			*/
			xSemaphoreGive(gatekeeper);
		} else {
			puts("mytask1 has failed to get acess within 1 sec\n");
		}
		//puts("mytask1\n");
		vTaskDelay(500);
	}
}

void mytask2(void *param) {
	while (1) {

		if (xSemaphoreTake(gatekeeper, 1000)) {
			puts("mytask2 has got access\n");
			access_resource();
			xSemaphoreGive(gatekeeper);
		} else {
			puts("mytask2 has failed to get acess within 1 sec\n");
		}
		//printf("mytask2\n");
		vTaskDelay(500);
	}
}

int main() {

	/*
	  xSemaphoreCreate(void)
	  creats mutex type semaphore and returns handle for it to be accessed
	*/
	gatekeeper = xSemaphoreCreateMutex();	//creating mutex	

	/*Creating task*/
	xTaskCreate(mytask1, (signed char *)"Task1", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
	xTaskCreate(mytask2, (signed char *)"Task2", configMINIMAL_STACK_SIZE, NULL, 1, NULL);

	/*start scheduler*/
	vTaskStartScheduler();

return -1;
}
