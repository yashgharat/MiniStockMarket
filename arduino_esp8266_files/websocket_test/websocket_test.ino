#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

#include <ESP8266WiFi.h>  
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <ArduinoJson.h>
#include <ArduinoWebsockets.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

const char* ssid     = "SpectrumSetup-B0";         // The SSID (name) of the Wi-Fi network you want to connect to
const char* password = "woodenstreet622";     // The password of the Wi-Fi network

const char* websockets_server = "wss://ws.finnhub.io/?token=c5q4draad3iaqkuehsqg"; //server adress and port

using namespace websockets;

void onMessageCallback(WebsocketsMessage message) {
  DynamicJsonDocument doc(2048);
  deserializeJson(doc, message.data());

  Serial.print("Got Message: ");

  JsonArray array = doc["data"].as<JsonArray>();
  JsonObject obj = array[0];

  Serial.print(obj["s"].as<char *>());
  Serial.print("\t");
  Serial.println(obj["p"].as<float>());

  lcd.setCursor(0, 0);
  
  lcd.print(obj["s"].as<char *>());

  lcd.setCursor(0, 1);      
  lcd.print(obj["p"].as<float>());
}

void onEventsCallback(WebsocketsEvent event, String data) {
    if(event == WebsocketsEvent::ConnectionOpened) {
        Serial.println("Connnection Opened");
    } else if(event == WebsocketsEvent::ConnectionClosed) {
        Serial.println("Connnection Closed");
    } else if(event == WebsocketsEvent::GotPing) {
        Serial.println("Got a Ping!");
    } else if(event == WebsocketsEvent::GotPong) {
        Serial.println("Got a Pong!");
    }
}

const int capacity = JSON_OBJECT_SIZE(2);
StaticJsonDocument<capacity> doc;

WebsocketsClient client;

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);             // Connect to the network
  int i=0;
  while (WiFi.status() != WL_CONNECTED) { // Wait for the Wi-Fi to connect
  delay(1000);
  Serial.print(++i); Serial.print(' ');
  }

  lcd.init();
  
  // Turn on the backlight.
  lcd.backlight();

  client.onMessage(onMessageCallback);
  client.onEvent(onEventsCallback);

  // Connect to server
  client.connect(websockets_server);

  doc["type"].set("subscribe");
  doc["symbol"].set("BINANCE:BTCUSDT");

  // Serialize JSON document
  String json;
  serializeJson(doc, json);

  client.send(json);
}
 
void loop() {
  client.poll();
}
