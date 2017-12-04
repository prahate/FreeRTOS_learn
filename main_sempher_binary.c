#include <stdio.h>
#include <stdlib.h>

#include "FreeRTOS.h"
#include "task.h"	//header file for task related APIs
#include "semphr.h"	//header file for semaphore related APIs

xSemaphoreHandle employee_signal = 0;

void employee_task() {
}

void boss (void *p) {
	while (1) {
		printf("Boss giving signal to employee\n");
		xSemaphoreGive(employee_signal);
		printf("Boss gave task/signal to employee\n");
		vTaskDelay(2000);
	}
}

void employee (void *p) {
	while (1) {
		if (xSemaphoreTake(employee_signal, 1000)) {
			employee_task();
			printf("Employee finished task\n");
		}
	}
}

int main () {

	//enableFlushAfterPrintf();

	vSemaphoreCreateBinary(employee_signal);

	xTaskCreate(boss, (signed char *)"t1", 1024, NULL, 1, NULL);
	xTaskCreate(employee, (signed char *)"t2", 1024, NULL, 1, NULL);

	vTaskStartScheduler();
	
	return 0;
}
