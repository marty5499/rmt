#include <remote.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"

void onMsg(String msg)
{
}

TaskHandle_t t_handle;
char buf[4096];

void taskList()
{
    UBaseType_t n = uxTaskGetNumberOfTasks();
    Serial.println(n);
}

void print_shit(void)
{
    char *str = (char *)malloc(sizeof(char) * 2000);
    memset(str, 0, 2000);
    char *pcWriteBuffer = str;

    TaskStatus_t *pxTaskStatusArray;

    volatile UBaseType_t uxArraySize, x;
    unsigned long ulStatsAsPercentage;
    uint32_t ulTotalRunTime;

    /* Make sure the write buffer does not contain a string. */
    *pcWriteBuffer = 0x00;

    /* Take a snapshot of the number of tasks in case it changes while this
    function is executing. */
    uxArraySize = uxTaskGetNumberOfTasks();

    /* Allocate a TaskStatus_t structure for each task.  An array could be
    allocated statically at compile time. */
    Serial.printf("sizeof TaskStatus_t: %d\n", sizeof(TaskStatus_t));
    pxTaskStatusArray = (TaskStatus_t *)pvPortMalloc(uxArraySize * sizeof(TaskStatus_t));
    memset(pxTaskStatusArray, 0, uxArraySize * sizeof(TaskStatus_t));

    if (pxTaskStatusArray != NULL)
    {
        /* Generate raw status information about each task. */
        Serial.printf("Array size before: %d\n", uxArraySize);
        uxArraySize = uxTaskGetSystemState(pxTaskStatusArray,
                                           uxArraySize,
                                           &ulTotalRunTime);
        Serial.printf("Array size after: %d\n", uxArraySize);
        Serial.printf("Total runtime: %d\n", ulTotalRunTime);

        /* For percentage calculations. */
        ulTotalRunTime /= 100UL;

        /* Avoid divide by zero errors. */
        if (ulTotalRunTime > 0)
        {
            /* For each populated position in the pxTaskStatusArray array,
            format the raw data as human readable ASCII data. */
            Serial.printf("  name        runtime      pct     core         prio\n");
            for (int x = 0; x < uxArraySize; x++)
            {
                TaskStatus_t *fuckit;
                void *tmp = &pxTaskStatusArray[x];
                void *hmm = tmp + (4 * x);
                fuckit = (TaskStatus_t *)hmm;

                // Serial.printf("Name: %.5s, ulRunTimeCounter: %d\n", fuckit->pcTaskName , fuckit->ulRunTimeCounter);
                /* What percentage of the total run time has the task used?
                This will always be rounded down to the nearest integer.
                ulTotalRunTimeDiv100 has already been divided by 100. */
                ulStatsAsPercentage =
                    fuckit->ulRunTimeCounter / ulTotalRunTime;

                if (ulStatsAsPercentage > 0UL)
                {
                    sprintf(pcWriteBuffer, "%30s %10lu %10lu%% %5d %5d\n",
                            fuckit->pcTaskName,
                            fuckit->ulRunTimeCounter,
                            ulStatsAsPercentage,
                            *((int *)(&fuckit->usStackHighWaterMark) + 1),
                            fuckit->uxBasePriority);
                }
                else
                {
                    /* If the percentage is zero here then the task has
                    consumed less than 1% of the total run time. */
                    sprintf(pcWriteBuffer, "%30s %10lu %10s  %5d %5d\n",
                            fuckit->pcTaskName,
                            fuckit->ulRunTimeCounter,
                            "<1%",
                            *((uint32_t *)(&fuckit->usStackHighWaterMark) + 1),
                            fuckit->uxBasePriority);
                }

                pcWriteBuffer += strlen((char *)pcWriteBuffer);
                // Serial.printf("len: %d, idx: %d\n", pcWriteBuffer - str, x);
            }
            Serial.print(str);
        }

        /* The array is no longer needed, free the memory it consumes. */
        //   vPortFree( pxTaskStatusArray );
        //   free(str);
    }
}

void init()
{
    remote();
    taskList();
    print_shit();
    // t_handle = xTaskGetCurrentTaskHandle();
    // uxTaskGetSystemState()
    // startTimer(100);
    // startTimer(1000 * 1000);
}