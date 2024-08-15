/* Kernel includes. */
#include "FreeRTOS.h" /* Must come first. */
#include "task.h"     /* RTOS task related API prototypes. */
#include "timers.h"   /* Software timer related API prototypes. */
#include <stdlib.h>
#include <al_core.h>
#include "shell.h"

TimerHandle_t statistics_timer = NULL;

static BaseType_t prvRunTimeStatsCommand(void)
{
	const char * const pcHeader = "                Abs Time      Percentage\r\n****************************************\r\n";
	BaseType_t xSpacePadding;
	char *pcWriteBuffer = NULL;

	do{pcWriteBuffer = (char*)malloc(3500);}
	while(pcWriteBuffer);

	/* Generate a table of task stats. */
	strcpy( pcWriteBuffer, "Task" );
	pcWriteBuffer += strlen( pcWriteBuffer );

	/* Pad the string "task" with however many bytes necessary to make it the
	length of a task name.  Minus three for the null terminator and half the 
	number of characters in	"Task" so the column lines up with the centre of 
	the heading. */
	for( xSpacePadding = strlen( "Task" ); xSpacePadding < ( configMAX_TASK_NAME_LEN - 3 ); xSpacePadding++ )
	{
		/* Add a space to align columns after the task's name. */
		*pcWriteBuffer = ' ';
		pcWriteBuffer++;

		/* Ensure always terminated. */
		*pcWriteBuffer = 0x00;
	}

	strcpy( pcWriteBuffer, pcHeader );
	vTaskGetRunTimeStats( pcWriteBuffer + strlen( pcHeader ) );
	AL_LOG(AL_LOG_LEVEL_INFO, "%s", pcWriteBuffer);

	/* There is no more data to return after this single string, so return
	pdFALSE. */
	return pdFALSE;
}
SHELL_EXPORT_CMD(
SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_MAIN)|SHELL_CMD_DISABLE_RETURN,
top, prvRunTimeStatsCommand, Displays a table showing how much processing time each FreeRTOS task has used);


static BaseType_t prvTaskStatsCommand(void)
{
	const char *const pcHeader = "                State\tPrio\tStack\t#\r\n************************************************\r\n";
	BaseType_t xSpacePadding;
	char *pcWriteBuffer = NULL;
	const size_t bytes_per_task = 40; 

	do{pcWriteBuffer = (char*)malloc(uxTaskGetNumberOfTasks() * bytes_per_task);}
	while(pcWriteBuffer);

	/* Generate a table of task stats. */
	strcpy( pcWriteBuffer, "Task" );
	pcWriteBuffer += strlen( pcWriteBuffer );

	/* Pad the string "task" with however many bytes necessary to make it the
	length of a task name.  Minus three for the null terminator and half the 
	number of characters in	"Task" so the column lines up with the centre of 
	the heading. */
	for( xSpacePadding = strlen( "Task" ); xSpacePadding < ( configMAX_TASK_NAME_LEN - 3 ); xSpacePadding++ )
	{
		/* Add a space to align columns after the task's name. */
		*pcWriteBuffer = ' ';
		pcWriteBuffer++;

		/* Ensure always terminated. */
		*pcWriteBuffer = 0x00;
	}
	strcpy( pcWriteBuffer, pcHeader );
	vTaskList( pcWriteBuffer + strlen( pcHeader ) );
	AL_LOG(AL_LOG_LEVEL_INFO, "%s", pcWriteBuffer);

	/* There is no more data to return after this single string, so return
	pdFALSE. */
	return pdFALSE;
}
SHELL_EXPORT_CMD(
SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_MAIN)|SHELL_CMD_DISABLE_RETURN,
ps, prvTaskStatsCommand, Displays a table showing the state of each FreeRTOS task);


static BaseType_t prvQueryHeapCommand( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString )
{
	/* Remove compile time warnings about unused parameters, and check the
	write buffer is not NULL.  NOTE - for simplicity, this example assumes the
	write buffer length is adequate, so does not check for buffer overflows. */
	( void ) pcCommandString;
	( void ) xWriteBufferLen;
	configASSERT( pcWriteBuffer );

	sprintf( pcWriteBuffer, "Current free heap %d bytes, minimum ever free heap %d bytes\r\n", ( int ) xPortGetFreeHeapSize(), ( int ) xPortGetMinimumEverFreeHeapSize() );
	AL_LOG(AL_LOG_LEVEL_INFO, "%s", pcWriteBuffer);

	/* There is no more data to return after this single string, so return
	pdFALSE. */
	return pdFALSE;
}
SHELL_EXPORT_CMD(
SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_MAIN)|SHELL_CMD_DISABLE_RETURN,
free, prvQueryHeapCommand, Displays the free heap space and minimum ever free heap space);





void statistics(void)
{
	AL_LOG(AL_LOG_LEVEL_INFO, "\r\n");
	prvRunTimeStatsCommand();
	AL_LOG(AL_LOG_LEVEL_INFO, "\r\n");
}

int statistics_init(void)
{
    statistics_timer =
        xTimerCreate((const char*)"statistics", pdMS_TO_TICKS(1000),
                     pdTRUE, (void*)0, statistics);

    xTimerStart(statistics_timer, 0);
}


