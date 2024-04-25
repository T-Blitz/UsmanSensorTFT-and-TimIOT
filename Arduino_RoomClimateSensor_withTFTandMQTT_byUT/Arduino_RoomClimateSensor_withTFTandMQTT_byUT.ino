#include "UsmanBME.h"
#include "UsmanTFT.h"

// function library that handles WiFi and MQTT. Written by Tim Bli. using <WiFiS3.h> and <ArduinoMqttClient.h>
#include "TimIOT.h"


// used for comparing current measured data with last send data
// 105 because it's and absured number, so that after each reboot a correct number is send to HA
float oldTemp = 105;
float oldHum = 105;
float oldPress = 105;
float oldGas = 105;

const char* matrikelnummerData = "";  //Pleas add your Matrikelnumber, char somehow needs to be number not a string


// Void Setup here
void setup() {
  // start Serial Monitor
  Serial.begin(9600);


  UsmanBME_setup();
  UsmanTFT_setup();     

  // Setting up and connecting to WiFi, using "TimIOT.h"
  TimIOT_WiFi_setupWifi();

  // Setting up and connecting to MQTT, using "TimIOT.h"
  TimIOT_MQTT_setupMQTT();

  TimIOT_MQTT_sendMatrikelnummerToHASS(matrikelnummerData, topicMatrikelnummer);
}

//    Void Loop here   //
void loop() {

  // using "TimIOT.h"
  // calling both, "Keep Alive", functions every loop.
  // To ensure a mentained Connection to both WiFi and MQTT Broker
  TimIOT_WiFi_keepAlive();
  TimIOT_MQTT_keepAlive();

  // reads the current Room Climate and Activate the heater
  UsmanBME_readAndClean();


  // get The newly read Climate data and passes them in main loop
  float temperature = UsmanBME_temperature;
  float humidity = UsmanBME_humidity;
  float pressure = UsmanBME_pressure;
  float altitude = UsmanBME_altitude;
  float gasResistance = UsmanBME_gasResistance;

  UsmanTFT_displayDataOnTFT(temperature, humidity, pressure, altitude, gasResistance);

  // Compares if Sensor-data has changed before sending
  // in order to only send changes in data
  //TimIOT_MQTT_sendMatrikelnummerToHASS(matrikelnummerData, topicMatrikelnummer);

  if (oldTemp != temperature) {
    // save the last time a message was sent
    oldTemp = temperature;
    TimIOT_MQTT_sendDataToHASS(temperature, topicTemp);
  }
  if (oldHum != humidity) {
    // save the last time a message was sent
    oldHum = humidity;
    TimIOT_MQTT_sendDataToHASS(humidity, topicHum);
  }
  if (oldPress != pressure) {
    // save the last time a message was sent
    oldPress = pressure;
    TimIOT_MQTT_sendDataToHASS(pressure, topicPress);
  }
  if (oldGas != gasResistance) {
    // save the last time a message was sent
    oldGas = gasResistance;
    TimIOT_MQTT_sendDataToHASS(gasResistance, topicGas);
  }

  
  // Print to Serial Monitor
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print(" °C, Humidity: ");
  Serial.print(humidity);
  Serial.print(" %, Pressure: ");
  Serial.print(pressure);
  Serial.println("Gas Resistance:");
  Serial.print(gasResistance);
  Serial.print("kOhm");
  Serial.println("");



  // Adjust delay as needed
  delay(500);  
}