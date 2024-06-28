#include <Adafruit_BME680.h>


// creating struct with for easy passing of Climate data to the main loop
struct UsmanBME_SensorDataBME { 
  float temperature;
  float humidity;
  float pressure;
  float altitude; 
  float gasResistance;
};

// Function prototypes, they make things way easier
//declaration after the struct declaration, since the compiler does not know what "SensorDataBME" means otherwise 
void usmanBME_setup();

// using name of the struct "SensorData" instead of "void" in function declaration since void can't return structs 
UsmanBME_SensorDataBME usmanBME_ReadingBME();


Adafruit_BME680 bme;


void usmanBME_setup(){

  if (!bme.begin()) {

    // checks if the BME is installed correctly
    // if not, sends an error message in serial and "Bricks" the system 
    // using an infinte while loop, till reset
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

// reads the current Room Climate and saves them in a struct
// then returns them for futher usage
UsmanBME_SensorDataBME usmanBME_ReadingBME(){

  // setting up the struct for in function usage
  UsmanBME_SensorDataBME dataBME;

  bme.performReading();
  
  dataBME.temperature = bme.readTemperature();
  dataBME.humidity = bme.readHumidity();
  dataBME.pressure = bme.readPressure() / 100.0;  // Convert to hPa
  dataBME.altitude = bme.readAltitude(SENSORS_PRESSURE_SEALEVELHPA);
  // Convert to KOhms, later known as "voc" 
  dataBME.gasResistance = bme.readGas() / 100.0;


  return dataBME;
}





