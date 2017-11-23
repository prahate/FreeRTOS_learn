# FreeRTOS_learn
FreeRTOS Tutorials on Linux

Following are the steps to get FreeRTOS working on Ubuntu system
There is already a port available for linux(x86 systems) using that port to work on tutorials to learn FreeRTOS

1. Download FreeRTOS port for x86 from below link
http://www.freertos.org/FreeRTOS-simulator-for-Linux.html
untar it and go to release folder and run make all

2. open FreeRTOSConfig.h file
and comment following line
/*extern void vMainQueueSendPassed( void );
#define traceQUEUE_SEND( pxQueue ) vMainQueueSendPassed()*/

3. Set the 
#define configUSE_IDLE_HOOK                             1
to 
#define configUSE_IDLE_HOOK                             0

4. To create new project move origianl main.c file to different name
delete everything apart from #includes
and write your code 

to compile run make clean and then make all from Release folder.
