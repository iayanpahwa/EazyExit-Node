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


/*
ESP8266 firmware acting as MQTT endpoint for EazyExit home automation solution.
# Originally created by: Ayan Pahwa for SDIoT/EazyExit
# Date : March 5th 2017
*/

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <configuration.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>

#define RELAY D0 //Define pin for load
#define SERIAL_DEBUG 1 // Enable this for optional serial debugging
#define CONFIGURATION_PROVIDED 0

//Clients instances
WiFiClient espClient;
WiFiManager wifiManager;
PubSubClient client(espClient);

//Declare functions
bool isConnected(); //To check connection if node is connected to Access Point
void setup_wps(); //To setup WPS connection with Access Point
void setup_wifi(); //Fallback, to use WiFi manager for connection
void callback(char* , byte* , unsigned int); // Function executes whenever MQTT message arrives
void getIP(); //Print MAC_Address,IP address and other info

//Program starts here:
void setup() {

  #if SERIAL_DEBUG
  Serial.begin(115200);
  #endif

  pinMode(RELAY,OUTPUT); //Configure Pin as Output

  #if CONFIGURATION_PROVIDED

  #if SERIAL_DEBUG
  Serial.println("Connecting to AP whose credentials are provided already");
  Serial.println(ssid);
  #endif

  WiFi.begin(ssid, password);  //Connect using credentials provided in credentials.h file
  delay(5000);

  #else
  #if SERIAL_DEBUG
  Serial.println("Trying connected to a known AP");
  #endif

  WiFi.begin();
  delay(5000); //Wait 5 seconds for connecting
  #endif

  if(!isConnected()){
      #if SERIAL_DEBUG
      Serial.println("Can't find a known AP, Activating WPS mode");
      delay(1000);
      setup_wps();
      #endif
  }

    #if SERIAL_DEBUG
    getIP();
    #endif

  client.setServer(mqtt_server, 1883); //Set MQTT server in 'credentials.h'
  client.setCallback(callback); //Set MQTT callback function, which executes whenever MQTT message arrives
}

void setup_wps(){
  #if SERIAL_DEBUG
  Serial.println("WPS MODE ACTIVATED!");
  #endif

  WiFi.mode(WIFI_STA); //Set up station mode for WPS to work
  WiFi.beginWPSConfig(); //Start WPS connection
  delay(10000); //10 seconds timeout

  if(!isConnected()){
      #if SERIAL_DEBUG
      Serial.println("WPS timeout! Starting WiFi Manager");
      #endif

      setup_wifi();
  }
}

bool isConnected(){

  if (WiFi.status()!= WL_CONNECTED){
    #if SERIAL_DEBUG
    Serial.println("NOT CONNECTED TO AP");
    #endif
    return false;
  }
  else{
    #if SERIAL_DEBUG
    Serial.println("CONNECTED TO AP");
    #endif
    return true;
  }
}

void setup_wifi() {

  delay(5); // Wait 5 seconds before connecting

  #if SERIAL_DEBUG
  Serial.println("Cannot find known router, Starting WiFiManager Access Point");
  #endif

  wifi_wps_disable(); //Disable WPS mode for fallback to work
  wifiManager.setTimeout(60); //Timeout and destroy AP after 1 minutes
  wifiManager.autoConnect("EazyExit"); //Connect using WiFi manager, if no known network in range or credentials not provided
  delay(500);

  if(!isConnected()){
        delay(2000); // 2 seconds timeout
        #if SERIAL_DEBUG
        Serial.println("TIMEOUT, All methods failed! Restarting Node");
        #endif
        ESP.reset(); //If all methods fails reset ESP
    }
}

void getIP(){
  #if SERIAL_DEBUG
  Serial.println("IP address: ");
  Serial.print(WiFi.localIP());
  #endif
}

void callback(char* topic, byte* payload, unsigned int length) {

  char p[length + 1];
  memcpy(p, payload, length);
  p[length] = 0;
  String message(p);

  #if SERIAL_DEBUG
  Serial.println("Message arrived");
  for(int i=0; i<=length+1; i++)
  Serial.println(message[i]);
  #endif

  if(message == "off")
  digitalWrite(RELAY,HIGH);

  if(message == "onn")
  digitalWrite(RELAY,LOW);
}

void reconnect() {

  // Loop until we're reconnected
  while (!client.connected()) {

    #if SERIAL_DEBUG
    Serial.println("\nAttempting MQTT connection...");
    #endif

    // Attempt to connect
    if (client.connect("ESP8266Client")) {
      #if SERIAL_DEBUG
      Serial.println("connected");
      #endif
      client.subscribe(topic);
}

    else {
      #if SERIAL_DEBUG
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      #endif
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}
