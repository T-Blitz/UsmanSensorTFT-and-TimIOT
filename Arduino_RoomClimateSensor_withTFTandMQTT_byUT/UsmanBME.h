//#include <Wire.h>
//#include <Adafruit_Sensor.h>
#include <Adafruit_BME680.h>

//#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>

void UsmanBME_setup();
void UsmanBME_readAndClean();

float UsmanBME_temperature = 0;
float UsmanBME_humidity = 0;
float UsmanBME_pressure = 0;
float UsmanBME_altitude = 0;
// Convert to KOhms, later known as "voc" 
float UsmanBME_gasResistance = 0;

Adafruit_BME680 bme;


void UsmanBME_setup(){

  if (!bme.begin()) {
    Serial.println("Could not find a valid BME680 sensor, check wiring!");
    while (1)
      ;
  }

    //Set Oversampling and filter settings
  bme.setTemperatureOversampling(BME680_OS_8X);
  bme.setHumidityOversampling(BME680_OS_2X);
  bme.setPressureOversampling(BME680_OS_4X);
  bme.setIIRFilterSize(BME680_FILTER_SIZE_3);
  bme.setGasHeater(320, 150);

}

// reads the current Room Climate and cleans the sensor using the heater
void UsmanBME_readAndClean(){
  bme.performReading();
  bme.setGasHeater(320, 150);

  float UsmanBME_temperature = bme.readTemperature();
  float UsmanBME_humidity = bme.readHumidity();
  float UsmanBME_pressure = bme.readPressure() / 100.0;  // Convert to hPa
  float UsmanBME_altitude = bme.readAltitude(SENSORS_PRESSURE_SEALEVELHPA);
  // Convert to KOhms, later known as "voc" 
  float UsmanBME_gasResistance = bme.readGas() / 100.0;  
}




