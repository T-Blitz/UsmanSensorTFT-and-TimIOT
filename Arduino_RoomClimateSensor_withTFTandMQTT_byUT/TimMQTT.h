
#include <ArduinoMqttClient.h>
#include "Config_Secrets.h"
#include "TimWiFi.h"

void timMQTT_setupMQTT();
void timMQTT_connect2Brocker();
void timMQTT_keepAlive();
void timMQTT_sendData2Broker(float sensorData, const char topicName[]);
void timMQTT_sendMatrikelnummer2Broker();




// defining MQTT User and login data
const char broker[] = SECRET_MQTT_BROKER_ADDRESS;   // Name of address of the MQTT Broker
const unsigned int port = SECRET_MQTT_BROKER_PORT;  // Name of port of the MQTT Broker
const char username[] = SECRET_MQTT_USER;           // Name of user name of user using the MQTT Broker
const char password_mqtt[] = SECRET_MQTT_PASS;      // Name of password of user using the MQTT Broker
char MQTT_ClientID[17] = "1234567890";

// Setting Topic to send to
// formated for the use with Home Assistant - LaborTKE
// https://www.home-assistant.io/integrations/sensor.mqtt/
// Topic names are fully defined in Secrets_Config.h
const char topicTemp[] = TOPIC_TEMPERATURE;   // Temperature
const char topicHum[] = TOPIC_HUMIDITY;       // Humidity
const char topicPress[] = TOPIC_PRESSURE;     // Air Pressure
const char topicGas[] = TOPIC_GAS_RESISTENCE;  //

const char topicMatrikelnummer[] = TOPIC_MATRIKELNUMMER;


WiFiClient wifiClientMQTT;
MqttClient mqttClient(wifiClientMQTT);

// Set up MQTT
void timMQTT_setupMQTT() {

  // passing MQTT login data and confirms it in serial monitor
  // converts local ip to char Array, and passes it as MQTT clientID
  itoa(WiFi.localIP(), MQTT_ClientID, 17);
  mqttClient.setId(MQTT_ClientID);  // setting MQTT user, Id and password
  mqttClient.setUsernamePassword(username, password_mqtt);
  Serial.print("Attempting to connect to the MQTT broker: ");
  Serial.println(broker);
  // tries to connect to MQTT broker, shows error message if failed and tries again
  timMQTT_connect2Brocker();

  // Searial message if connection succesfull
  Serial.println("You're connected to the MQTT broker!");
  Serial.print("Your MQTT-Client ID:");
  Serial.print(MQTT_ClientID);
  Serial.println();
}

// connects to MQTT Broker 
// calls TimIOT_WiFi_keepAlive() to ensure WiFi connection
void timMQTT_connect2Brocker() {

  while (!mqttClient.connect(broker, port)) {
    Serial.print("MQTT connection failed! Error code = ");
    Serial.println(mqttClient.connectError());
    timWiFi_keepAlive();
    // wait 10 seconds for connection:
    delay(10000);
  }
}

// Checks if the connection to MQTT Broker has been lost
// reconnects using TimIOT_MQTT_connect2Brocker()
void timMQTT_keepAlive() {

  // checks first to see if the connections has been lost.
  if (!mqttClient.connected()) {
    Serial.println("MQTT connection lost. Reconnecting...");
    timMQTT_connect2Brocker();
  } else {
    // then call poll() regularly to allow the library to send MQTT keep alives which
    // avoids being disconnected by the broker.
    mqttClient.poll();  // you made this?... I made this.
  }
}

// use this to send the sensor data to Home Assistant using MQTT
void timMQTT_sendData2Broker(float sensorData, const char topicName[]) {

  Serial.print("Sending message to topic: ");
  Serial.println(topicName);
  Serial.print(sensorData);

  // send message, the Print interface can be used to set the message contents
  mqttClient.beginMessage(topicName, true);
  mqttClient.print(sensorData);
  mqttClient.endMessage();

  Serial.println();
}


void timMQTT_sendMatrikelnummer2Broker() {

  Serial.print("Sending message to topic: ");
  Serial.println(TOPIC_MATRIKELNUMMER);
  Serial.print(MATRIKELNUMMER);

    // send message, the Print interface can be used to set the message contents
  mqttClient.beginMessage(TOPIC_MATRIKELNUMMER, true);
  mqttClient.print(MATRIKELNUMMER);
  mqttClient.endMessage();

  Serial.println();
}








