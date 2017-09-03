/*
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
    */

    
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>
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

void setup_wps(int timeout) {
  #if SERIAL_DEBUG
  Serial.println("WPS MODE ACTIVATED!");
  #endif

  WiFi.mode(WIFI_STA); //Set up station mode for WPS to work
  WiFi.beginWPSConfig(); //Start WPS connection
  delay(timeout); //10 seconds timeout
}


void setup_wifi() {

  WiFiManager wifiManager;
  delay(5); // Wait 5 seconds before connecting

  #if SERIAL_DEBUG
  Serial.println("Cannot find known router, Starting WiFiManager Access Point");
  #endif

  wifi_wps_disable(); //Disable WPS mode for fallback to work
  wifiManager.setTimeout(300); //Timeout and destroy AP after 5 minutes

  // Setup node as Access point for configuration with a unique SSID and pasword=eazyexit
  String ssid = "EazyExit_"+ String(ESP.getChipId());
  const char* password = "eazyexit";
  wifiManager.autoConnect(ssid.c_str(), password); //Connect using WiFi manager, if no known network in range or credentials not provided
  delay(500);

  if(!isConnected()) {
        delay(2000); // 2 seconds timeout
        #if SERIAL_DEBUG
        Serial.println("TIMEOUT, All methods failed! Restarting Node");
        #endif
        ESP.reset(); //If all methods fails reset ESP
    }
}

void getIP() {
  #if SERIAL_DEBUG
  Serial.println("IP address: ");
  Serial.print(WiFi.localIP());
  #endif
}
