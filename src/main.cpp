#include <local.h>
//#include <Arduino.h>
#include <stdio.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"


void taskList()
{
    UBaseType_t n = uxTaskGetNumberOfTasks();
    Serial.println(n);
}


void hello_task(void *pvParameter)
{
    Serial.println("Hello world!\n");
    int i=0;
    while (true) {
        Serial.println(i++);
        vTaskDelay(1000 / portTICK_RATE_MS);
    }
}

TaskHandle_t t_handle;
char *buf = (char*)calloc(1,2048);

void setting()
{
    //Serial.begin(115200);
    Serial.println("create Task.");
    //remote();
    xTaskCreate(&hello_task, "hello_task", 2048, NULL, 5, NULL);
    delay(1000);
    //taskList();
    vTaskList(buf);
    //vTaskGetRunTimeStats(buf);
    // t_handle = xTaskGetCurrentTaskHandle();
    // uxTaskGetSystemState()
    // startTimer(100);
    // startTimer(1000 * 1000);
}
