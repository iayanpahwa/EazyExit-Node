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
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <Hash.h>
#include <WiFiManager.h>
#include "configuration.h"
#include "EazyExit.h"

// Make a Unique Node ID using MAC Address SHA1 hash
String response; // MQTT response to discover command
String seperator = ":"; // Seperator character for parsing, treated as string for eaze
String x = sha1(WiFi.macAddress()); // Unique hash calculated on basis of MAC address
String command_on = x + seperator + "ON"; // MQTT on command frame
String command_off = x + seperator + "OFF"; // MQTT off command frame

// Convert hash string to constant char* to be used later
const char* UUID = x.c_str();

// Clients instances
WiFiClient espClient;
PubSubClient client(espClient);

// MQTT callback function prototype
void callback(char* , byte* , unsigned int); // Function executes whenever MQTT message arrives

//Program starts here:
void setup() {

  #if SERIAL_DEBUG
  Serial.begin(BAUD_RATE);
  #endif

  #if SERIAL_DEBUG
  Serial.println("Node UUID:");
  Serial.println(UUID);
  #endif

  pinMode(RELAY,OUTPUT); // Configure Pin as Output

  #if SERIAL_DEBUG
  Serial.println("Trying to connect to a known AP");
  #endif

  WiFi.begin(); // Try to connect to a known AP in range
  delay(CONNECT_WAIT); // Timeout of 5 seconds for connecting

  if(!isConnected()) {

      #if SERIAL_DEBUG
      Serial.println("Can't find a known AP, Activating WPS mode");
      #endif

      delay(1000); // Wait 1 second before entring WPS mode
      setup_wps(WPS_TIMEOUT); // Start WPS if no known AP is in range

        if(!isConnected()) {
            #if SERIAL_DEBUG
            Serial.println("WPS timeout! Starting WiFi Manager");
            #endif
            setup_wifi(); // Start WiFiManager in AP mode when WPS fails to connect
          }
}

  getIP(); // Print IP Address on console
  
  response = UUID + seperator + WiFi.localIP().toString(); // Response header to MQTT IDENTIFY call
  client.setServer(MQTT_SERVER, MQTT_PORT); //Set MQTT server in 'credentials.h'
  client.setCallback(callback); //Set MQTT callback function, which executes whenever MQTT message arrives
}

void callback(char* TOPIC, byte* payload, unsigned int length) {

  char p[length + 1];
  memcpy(p, payload, length);
  p[length] = 0;
  String message(p);

  #if SERIAL_DEBUG
  Serial.println("Message arrived");
  for(int i=0; i<=length+1; i++)
  Serial.println(message[i]);
  #endif

// Send MAC ID whenever APP requests node for discovery
  if(message == "IDENTIFY") {
    client.publish(TOPIC_DISCOVERY, response.c_str());
    client.subscribe(TOPIC);
  }

  if(message == command_on) {
    digitalWrite(RELAY,LOW);
    client.publish(TOPIC_ACK , command_on.c_str());
    client.subscribe(TOPIC);
  }

  if(message == command_off) {
    digitalWrite(RELAY,HIGH);
    client.publish(TOPIC_ACK , command_off.c_str());
    client.subscribe(TOPIC);
  }

  if(message == "ON") {
    digitalWrite(RELAY,HIGH);
    client.publish(TOPIC_ACK , command_on.c_str());
    client.subscribe(TOPIC);
  }

  if(message == "OFF") {
    digitalWrite(RELAY,LOW);
    client.publish(TOPIC_ACK , command_off.c_str());
    client.subscribe(TOPIC);
  }
}

// Function to reconnect to MQTT broker in case connection breaks
void reconnect() {

  // Loop until we're reconnected
  while (!client.connected()) {

    #if SERIAL_DEBUG
    Serial.println("\nAttempting MQTT connection...");
    #endif
    // Attempt to connect
    if (client.connect(UUID)) {
      #if SERIAL_DEBUG
      Serial.println("connected");
      #endif
      client.subscribe(TOPIC);
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
