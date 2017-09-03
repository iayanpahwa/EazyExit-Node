#include <ESP8266WiFi.h>
#include "EazyExit.h"

bool isConnected() {

  if (WiFi.status()!= WL_CONNECTED){
    #if SERIAL_DEBUG
    Serial.println("NOT CONNECTED TO AP");
    #endif
    return false;
  }

  else {
    #if SERIAL_DEBUG
    Serial.println("CONNECTED TO AP");
    #endif
    return true;
  }
}
