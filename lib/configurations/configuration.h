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

// Hardware configs
#define RELAY D0 // Define pin for load
#define SERIAL_DEBUG 1 // Enable this for optional serial debugging
#define BAUD_RATE 115200
// Add your WiFi and MQTT topic credentials below
const char* mqtt_server = "***.***.***.***"; //Address or IP
const char* topic = "myHome"; // MQTT topic string
