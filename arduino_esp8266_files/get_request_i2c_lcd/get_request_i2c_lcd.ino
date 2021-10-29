#include <LiquidCrystal_I2C.h>
#include <Wire.h> 

#include <ESP8266WiFi.h>  
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <ArduinoJson.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

const char* ssid     = "SpectrumSetup-B0";         // The SSID (name) of the Wi-Fi network you want to connect to
const char* password = "woodenstreet622";     // The password of the Wi-Fi network

void setup() {
  WiFi.begin(ssid, password);             // Connect to the network
  while (WiFi.status() != WL_CONNECTED) { // Wait for the Wi-Fi to connect
  delay(1000);
  int i=0;
  Serial.print(++i); Serial.print(' ');
  }
  
  lcd.init();
  
  // Turn on the backlight.
  lcd.backlight();

  HTTPClient http;
  WiFiClient client;

  http.useHTTP10(true);
  http.begin(client, "http://jsonplaceholder.typicode.com/users/3");
  int httpCode = http.GET(); 

  Stream& response = http.getStream();

  DynamicJsonDocument doc(2048);

  deserializeJson(doc, response);
  
  // Move the cursor 5 characters to the right and
  // zero characters down (line 1).
  lcd.setCursor(0, 0);
  
  // Print HELLO to the screen, starting at 5,0.
  lcd.print(doc["name"].as<char *>());
  
  // Move the cursor to the next line and print
  // WORLD.
  lcd.setCursor(0, 1);      
  lcd.print(doc["website"].as<char *>());
}
 
void loop() {
}
