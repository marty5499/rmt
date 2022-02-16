#include <main.h>

void timerTrigger()
{
    Serial.println("timer trigger..");
}

void onMsg(String msg){
    Serial.println(msg);
    sw = !sw;
    refresh = true;
}


void init(){
    Serial.println("init...");
    WiFi.setSleep(false);
    startTimer(1000*1000);
    
}
