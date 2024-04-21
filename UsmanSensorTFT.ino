#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME680.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>

// function library that handles WiFi and MQTT. Written by Tim Bli. using <WiFiS3.h> and <ArduinoMqttClient.h>
#include "TimIOT.h"

#define TFT_CS 10
#define TFT_DC 9
#define TFT_RST 8

// used for comparing current measured data with last send data
// 105 because it's and absured number, so that after each reboot a correct number is send to HA
float oldTemp = 105;
float oldHum = 105;
float oldPress = 105;
float oldGas = 105;
float oldAltitude = 105;

const char* matrikelnummerData = "1776";

Adafruit_BME680 bme;
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

// Void Setup here
void setup() {
  // start Serial Monitor
  Serial.begin(9600);

  if (!bme.begin()) {
    Serial.println("Could not find a valid BME680 sensor, check wiring!");
    while (1)
      ;
  }

  // Initialize the display with a black tab
  tft.initR(INITR_BLACKTAB); 
  // Adjust the rotation as needed 
  tft.setRotation(4);         

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

    // Activate the heater
  bme.performReading();
  bme.setGasHeater(320, 150);

  // Deactivate the heater after reading
  bme.setGasHeater(0, 0);

  float temperature = bme.readTemperature();
  float humidity = bme.readHumidity();
  float pressure = bme.readPressure() / 100.0;  // Convert to hPa
  float altitude = bme.readAltitude(SENSORS_PRESSURE_SEALEVELHPA);
  // Convert to KOhms, later known as "voc" 
  float gasResistance = bme.readGas() / 100.0;  

  displayDataOnTFT(temperature, humidity, pressure, altitude, gasResistance);

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
  if (oldAltitude != altitude) {
    // save the last time a message was sent
    oldAltitude = altitude;
    TimIOT_MQTT_sendDataToHASS(altitude, topicAltitude);
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


void displayDataOnTFT(float temp, float hum, float pressure, float altitude, float gasResistance) {
  // Display data on TFT

  tft.fillScreen(ST7735_WHITE);
  tft.setTextSize(2);
  tft.setCursor(2, 2);
  tft.setTextColor(ST7735_BLUE);

  tft.println("Temper: "); tft.print(temp); tft.println(" *C");
  tft.print("Humidity: "); tft.print(hum); tft.println(" %");
  tft.print("Pressure: "); tft.println(pressure);
  tft.print("Altitude: "); tft.println(altitude);
  tft.print("VOC: "); tft.println(gasResistance);
}
