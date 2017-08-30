# EazyExit is an open-source home automation and security solution which is focused to provide you with seamless connected home experience, while ensuring security in your absence.

## Changelog:

10//06/2017
- Added DB support and Node discovery feature. 

04/04/2017
- Added JAVA EazyExit server code

20/03/2017
- Added WPS support, push WPS button on router to connect to node during power on.

17/03/2017:
- Added WiFiManager(https://github.com/tzapu/WiFiManager.git) support to dynamically manage WiFi connections.

13/03/2017:
- Added PlatformIO(http://platformio.org) development and Build support

30/04/2017:
- Added Node UUID based on Hash generated from MAC Address

24/05/2017:
- Added DNS support for NODE to avoid hardcoding MQTT server IP in firmware(IP ---> eazyexit.lan)


## Setting up Raspberry pi to act as MQTT Server/Broker (Raspberry Pi/Debian Equivalent)
	Change hostname from raspberrypi to eazy exit using below command and then download required packages:
	- ``` sudo raspi-config > Advanced Options > hostnames to eazyexit```
	- ``` sudo apt-get update ```
	- ``` sudo apt-get install mosquito mosquito-clients ```

  - Mosquito broker will automatically start running on <your_pi_ip_address> port 1883

## Setting up Development and Build system

- Hardware Supported (ESP8266 Nodemcu modules).
- Platform independent, only dependency is Python 2.7, pip installed.
- Make sure SiLabs CP21xx USB to Serial drivers are installed.

- Install and setup PlatformIO environment and command line tools
	- ``` sudo pip install platformio ```

- Clone the repo into your local machine
	- ``` git clone https://github.com/Team-SDIoT/EazyExit.git ```
	- ``` cd EazyExit ```

- Fetch latest versions of submodules/dependent libraries using:
	- ``` git submodule init ```
	- ``` git submodule update ```

- In case above step doesn’t work clone libraries manually
	- ``` cd EazyExit/EazyExit-Node/lib ```
	- ``` git clone https://github.com/knolleary/pubsubclient.git PubSubClient ```
	- ``` git clone https://github.com/ekstrand/ESP8266wifi ```
	- ``` git clone https://github.com/tzapu/WiFiManager.git ```

- Edit "EazyExit/EazyExit-Node/lib/credentials/credentials.h" and add your WiFi SSID, WiFi password and IP address of server(Raspberry Pi) running MQTT broker

- Edit GPIO pin number EazyExit/EazyExit-Node/src/ESP8266.cpp if required default is pin D0 on which on-board LED is connected

- Build firmware using following command:
	- ``` cd EazyExit-Node ```
	- ``` pio run ``` // Note: This will build and generate binaries for all supportive hardware
	- ``` pio run -r nodemcu ``` // Note: This will build and generate binary only for nodemcu hardware

- Build Server code using following command:
	- ``` cd EazyExit/EazyExit-Server ```
	- ``` change JAVA_HOME in setenv.sh ```
	- ``` run build.sh as bash build.sh or make it executable to run as ./build.sh ```
	- ``` run deploy.sh in the folder where Deployable.tar resides ```

- To manually flash the binary you will need to install esptool:
	- ``` sudo pip install esptool ```

- Identify COMM port of board attached to your machine using Device Manager on windows and following command on Linux or MAC OSX
	- ```ls /dev/tty*, on mac and linux it is /dev/tty.SLAB_USBtoUART```

- cd to directory holding binary:
	- ``` EazyExit/.pioenvs/<Targer_Board> binary —> firmware.bin```

- Flash using esp_tool:
	- ``` esptool.py -p <COMM_PORT> —baud 460800 write_flash —flash_size detect 0 firmware.bin ```

## TEST

- ESP node listens to MQTT messages<onn/off> on topic myHome
- On raspberry pi run ``` mosquitto_pub -t myHome -m off ``` LED should be off and ``` mosquitto_pub -t myHome -m one ``` LED should turn on.

## Development Environment setup:

- Download PlatformIO official IDE "http://platformio.org/platformio-ide" or PlatformIO Plugin for ATOM IDE (http://www.atom.io)
