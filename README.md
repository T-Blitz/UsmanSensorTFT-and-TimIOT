# UsmanSensorTFT-and-TimIOT
Using a UNO R4 WiFi, BME680 enviroment sensor together with a TFT-Display to read out a Rooms climate and publish said climate data using MQTT, for example to homeassistant. 

- Usmam A. - Everything to do with the BME680 enviroment sensor and the TFT-display. 
- Tim B.   - Everything to do with Wifi connection and data publishing to a MQTT-Broker.




Presetup: 
	
	Clone the repository and upload the "mytt_config.yaml" file to your Home
	Assistant, use the File Editor Addon to do so.
	
	Add "mqtt: !include mqtt_config.yaml" to your main 'config.yaml' in 
	Home Assistant, fully restart your HA.
	Your HA only needs to be rebooted ONCE after that you can just reload 
	the yaml configs in developer settings.
	
	Before uploading the code to your Arduino, make sure that the 
	"Secrets.h" has your wifi information and your mqtt user + pw.
