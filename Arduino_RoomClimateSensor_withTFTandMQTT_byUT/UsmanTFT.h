#include <Adafruit_ST7735.h>

// Function prototypes, they make things way easier
void usmanTFT_setup();
void usmanTFT_displayDataOnTFT(float temp, float hum, float pressure, float altitude, float gasResistance);

#define TFT_CS 10
#define TFT_DC 9
#define TFT_RST 8

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

void usmanTFT_setup(){

    // Initialize the display with a black tab
  tft.initR(INITR_BLACKTAB); 
  // Adjust the rotation as needed 
  tft.setRotation(4);  
}



void usmanTFT_displayDataOnTFT(float temp, float hum, float pressure, float altitude, float gasResistance) {
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



