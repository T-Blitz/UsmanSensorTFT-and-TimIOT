#include <WiFiS3.h>
#include <ArduinoMqttClient.h>

// list of Login data and adresses
#include "Secrets.h"


// Function prototypes, they make things way easier
void TimIOT_WiFi_setupWifi();
void TimIOT_WiFi_connect2WiFi();
void TimIOT_WiFi_keepAlive();

void TimIOT_MQTT_setupMQTT();
void TimIOT_MQTT_connect2Brocker();
void TimIOT_MQTT_keepAlive();
void TimIOT_MQTT_sendDataToHASS(float sensorData, const char topicName[]);
void TimIOT_MQTT_sendMatrikelnummerToHASS(const char* nameData, const char topicName[]);


// defining Wifi login data
int status = WL_IDLE_STATUS;
const char ssid[] = SECRET_SSID;  // your network-WiFi SSID (name)
const char pass[] = SECRET_PASS;  // your network-WiFI password
int keyIndex = 0;

// defining MQTT User and login data
const char broker[] = SECRET_MQTT_BROKER_ADDRESS;   // Name of address of the MQTT Broker
const unsigned int port = SECRET_MQTT_BROKER_PORT;  // Name of port of the MQTT Broker
const char username[] = SECRET_MQTT_USER;           // Name of user name of user using the MQTT Broker
const char password_mqtt[] = SECRET_MQTT_PASS;      // Name of password of user using the MQTT Broker
char MQTT_ClientID[17] = "1234567890";
// Setting Topic to send to
// formated for the use with Home Assistant - LaborTKE
// https://www.home-assistant.io/integrations/sensor.mqtt/
const char topicTemp[] = "stat/ua-sensor0X/temperature";   // Temperature
const char topicHum[] = "stat/ua-sensor0X/humidity";       // Humidity
const char topicPress[] = "stat/ua-sensor0X/pressure";     // Air Pressure
const char topicGas[] = "stat/ua-sensor0X/gasResistance";  //
const char topicAltitude[] = "stat/ua-sensor0X/altitude"; 

const char topicMatrikelnummer[] = "stat/ua-sensor0X/matrikelnummer";  // Matrikelnummer 


WiFiClient wifiClientMQTT;
MqttClient mqttClient(wifiClientMQTT);


// Set up WiFi
void TimIOT_WiFi_setupWifi() {

  // check for the WiFi module
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    // don't continue, "Bricks" the board. Try to find out why there is no WiFi module detected
    while (true)
      ;
  }
  // check for Wifi firmware Version, notifies through Serial monitor, when a new version is available
  String fv = WiFi.firmwareVersion();
  if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
    Serial.println("Please upgrade the firmware");
  }
  TimIOT_WiFi_connect2WiFi();
}

// Connect to WiFi, also used for reconnecting
void TimIOT_WiFi_connect2WiFi() {

  Serial.print("Attempting to connect to SSID: ");
  Serial.println(ssid);
  // tries to connect to Wifi, passing Wifi Name and password
  while (WiFi.begin(ssid, pass) != WL_CONNECTED) {
    Serial.print(".");
    // wait 10 seconds for connection:
    delay(10000);
  }
  Serial.println("Connected to WiFi");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

// Checks if the connection to WiFi has been lost
// reconnects using TimIOT_WiFi_connect2WiFi()
void TimIOT_WiFi_keepAlive() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi connection lost. Reconnecting...");
    TimIOT_WiFi_connect2WiFi();
  }
}

// Set up MQTT
void TimIOT_MQTT_setupMQTT() {

  // passing MQTT login data and confirms it in serial monitor
  // converts local ip to char Array, and passes it as MQTT clientID
  itoa(WiFi.localIP(), MQTT_ClientID, 17);
  mqttClient.setId(MQTT_ClientID);  // setting MQTT user, Id and password
  mqttClient.setUsernamePassword(username, password_mqtt);
  Serial.print("Attempting to connect to the MQTT broker: ");
  Serial.println(broker);
  // tries to connect to MQTT broker, shows error message if failed and tries again
  TimIOT_MQTT_connect2Brocker();

  // Searial message if connection succesfull
  Serial.println("You're connected to the MQTT broker!");
  Serial.print("Your MQTT-Client ID:");
  Serial.print(MQTT_ClientID);
  Serial.print("MQTT_ClientID");
  Serial.println();
}

// connects to MQTT Broker 
// calls TimIOT_WiFi_keepAlive() to ensure WiFi connection
void TimIOT_MQTT_connect2Brocker() {

  while (!mqttClient.connect(broker, port)) {
    Serial.print("MQTT connection failed! Error code = ");
    Serial.println(mqttClient.connectError());
    TimIOT_WiFi_keepAlive();
    // wait 10 seconds for connection:
    delay(10000);
  }
}

// Checks if the connection to MQTT Broker has been lost
// reconnects using TimIOT_MQTT_connect2Brocker()
void TimIOT_MQTT_keepAlive() {

  // checks first to see if the connections has been lost.
  if (!mqttClient.connected()) {
    Serial.println("MQTT connection lost. Reconnecting...");
    TimIOT_MQTT_connect2Brocker();
  } else {
    // then call poll() regularly to allow the library to send MQTT keep alives which
    // avoids being disconnected by the broker.
    mqttClient.poll();  // you made this?... I made this.
  }
}

// use this to send the sensor data to Home Assistant using MQTT
void TimIOT_MQTT_sendDataToHASS(float sensorData, const char topicName[]) {

  Serial.print("Sending message to topic: ");
  Serial.println(topicName);
  Serial.print(sensorData);

  // send message, the Print interface can be used to set the message contents
  mqttClient.beginMessage(topicName, true);
  mqttClient.print(sensorData);
  mqttClient.endMessage();

  Serial.println();
}


void TimIOT_MQTT_sendMatrikelnummerToHASS(const char* nameData, const char topicName[]) {

  Serial.print("Sending message to topic: ");
  Serial.println(topicName);
  Serial.print(nameData);

    // send message, the Print interface can be used to set the message contents
  mqttClient.beginMessage(topicName, true);
  mqttClient.print(nameData);
  mqttClient.endMessage();

  Serial.println();
}








