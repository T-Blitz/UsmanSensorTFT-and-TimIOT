#include <WiFiS3.h>

// list of Login data and adresses
#include "Config_Secrets.h"

// Function prototypes, they make things way easier
void timWiFi_setupWiFi();
void timWiFi_connect2WiFi();
void timWiFi_keepAlive();


// defining Wifi login data
int status = WL_IDLE_STATUS;
const char ssid[] = SECRET_SSID;  // your network-WiFi SSID (name)
const char pass[] = SECRET_PASS;  // your network-WiFI password
int keyIndex = 0;



// Set up WiFi
void timWiFi_setupWiFi() {

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
  timWiFi_connect2WiFi();
}

// Connect to WiFi, also used for reconnecting
void timWiFi_connect2WiFi() {

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
void timWiFi_keepAlive() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi connection lost. Reconnecting...");
    timWiFi_connect2WiFi();
  }
}