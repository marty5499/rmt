#include <remote.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"

void timerTrigger()
{
    Serial.println("timer trigger..");
}

void onMsg(String msg)
{
 
}

void init()
{
    Serial.println("init...");
    //startTimer(1000*1000);
}