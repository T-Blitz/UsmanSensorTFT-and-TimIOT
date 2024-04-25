//#include <Wire.h>
//#include <Adafruit_Sensor.h>
//#include <Adafruit_BME680.h>
//#include <Adafruit_GFX.h>
//#include <Adafruit_ST7735.h>


void UsmanTFT_setup();
void UsmanTFT_displayDataOnTFT(float temp, float hum, float pressure, float altitude, float gasResistance);

#define TFT_CS 10
#define TFT_DC 9
#define TFT_RST 8

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

void UsmanTFT_setup(){

    // Initialize the display with a black tab
  tft.initR(INITR_BLACKTAB); 
  // Adjust the rotation as needed 
  tft.setRotation(4);  
}



void UsmanTFT_displayDataOnTFT(float temp, float hum, float pressure, float altitude, float gasResistance) {
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


