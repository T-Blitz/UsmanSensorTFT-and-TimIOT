#include "UsmanBME.h"
#include "UsmanTFT.h"

// function library that handles WiFi and MQTT. Written by Tim Bli. using <WiFiS3.h> and <ArduinoMqttClient.h>
#include "TimMQTT.h"




// used for comparing current measured data with last send data
// 105 because it's and absured number, so that after each reboot a correct number is send to HA
float oldTemp = 105;
float oldHum = 105;
float oldPress = 105;
float oldGas = 105;



// Void Setup here
void setup() {
  // start Serial Monitor
  Serial.begin(9600);

  // Setting up the BME680 Sensor using "UsmanBME.h"
  usmanBME_setup();
  // Setting up the TFT Display using "UsmanTFT.h"
  usmanTFT_setup();     
  // Setting up and connecting to WiFi, using "TimIOT.h"
  timWiFi_setupWiFi();
  // Setting up and connecting to MQTT, using "TimIOT.h"
  timMQTT_setupMQTT();

  // sending the students matrikelnumber once after IoT setup complition.
  timMQTT_sendMatrikelnummer2Broker();
}

//    Void Loop here   //
void loop() {

  // using "TimIOT.h"
  // calling both, "Keep Alive", functions every loop.
  // To ensure a mentained Connection to both WiFi and MQTT Broker
  timWiFi_keepAlive();
  timMQTT_keepAlive();

  // reads the current Room Climate and passes them in to a struct
  // using the struct "SensorDataBME", declared in UsmanBME.h for simple usage of data
  UsmanBME_SensorDataBME resultBME = usmanBME_ReadingBME();


  // get The newly read Climate data and passes them in main loop
  // leaving them for now, even though they are not needed anymore, since we can just use the struct
  //float temperature = resultBME.temperature;
  //float humidity = resultBME.humidity;
  //float pressure = resultBME.pressure;
  //float altitude = resultBME.altitude;
  //float gasResistance = resultBME.gasResistance;

  usmanTFT_displayDataOnTFT(resultBME.temperature, resultBME.humidity, resultBME.pressure, resultBME.altitude, resultBME.gasResistance);

  // Compares if Sensor-data has changed before sending
  // in order to only send changes in data
  // using the resultBME struct 
  if (oldTemp != resultBME.temperature) {
    // save the last time a message was sent
    oldTemp = resultBME.temperature;
    timMQTT_sendData2Broker(resultBME.temperature, topicTemp);
  }
  if (oldHum != resultBME.humidity) {
    // save the last time a message was sent
    oldHum = resultBME.humidity;
    timMQTT_sendData2Broker(resultBME.humidity, topicHum);
  }
  if (oldPress != resultBME.pressure) {
    // save the last time a message was sent
    oldPress = resultBME.pressure;
    timMQTT_sendData2Broker(resultBME.pressure, topicPress);
  }
  if (oldGas != resultBME.gasResistance) {
    // save the last time a message was sent
    oldGas = resultBME.gasResistance;
    timMQTT_sendData2Broker(resultBME.gasResistance, topicGas);
  }

  
  // Print to Serial Monitor
  Serial.print("Temperature: ");
  Serial.print(resultBME.temperature);
  Serial.print(" °C, Humidity: ");
  Serial.print(resultBME.humidity);
  Serial.print(" %, Pressure: ");
  Serial.print(resultBME.pressure);
  Serial.println("Gas Resistance:");
  Serial.print(resultBME.gasResistance);
  Serial.print("kOhm");
  Serial.println("");

  // Adjust delay as needed. Currently: 5sec
  delay(5000);  
}