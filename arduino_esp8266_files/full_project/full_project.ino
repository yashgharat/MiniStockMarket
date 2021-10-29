#define TIMER_INTERRUPT_DEBUG         0
#define _TIMERINTERRUPT_LOGLEVEL_     0

#include "ESP8266TimerInterrupt.h"
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

#include <Adafruit_GFX.h>
#include <Max72xxPanel.h>

#include <ESP8266WiFi.h>  
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <ArduinoJson.h>
#include <ArduinoWebsockets.h>

volatile bool statusLed = false;
volatile uint32_t lastMillis = 0;

#define TIMER_INTERVAL_MS       1000

LiquidCrystal_I2C lcd(0x27, 16, 2);
Max72xxPanel matrix = Max72xxPanel(D4, 4, 1);

const int BUT_TIME = D6;
int but_time_state = 0;

const int BUT_STOCK = D8;
int but_stock_State = 0;

int rand_vals[32];

const char* ssid     = "Orion";         // The SSID (name) of the Wi-Fi network you want to connect to
const char* password = "Resident";     // The password of the Wi-Fi network

const char* websockets_server = "wss://ws.finnhub.io/?token=c5q4draad3iaqkuehsqg"; //server adress and port
using namespace websockets;

void onMessageCallback(WebsocketsMessage message) {
  DynamicJsonDocument doc(2048);
  deserializeJson(doc, message.data());

//  Serial.print("Got Message: ");

  JsonArray array = doc["data"].as<JsonArray>();
  JsonObject obj = array[0];

//  Serial.print(obj["s"].as<char *>());
//  Serial.print("\t");
//  Serial.println(obj["p"].as<float>());

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

void display_trend_graph(int vals[32]){
  int i, j;
  
  matrix.fillScreen(LOW);
  for(i=0; i<32; i++){
    for(j=0;j<vals[i]; j++){
      matrix.drawPixel(i, j, HIGH);
    }
  }
  matrix.write();
  delay(1000);
}

ESP8266Timer ITimer;
void IRAM_ATTR TimerHandler()
{
  
}

void IRAM_ATTR timeISR(){
  int i;
  while(digitalRead(BUT_TIME)==HIGH){
    yield(); //This will prevent wdt reset
  }
  for(i = 0; i<32; i++){
    rand_vals[i] = rand() % 8+1;
  }

  display_trend_graph(rand_vals);
  delay(1000);  //Simple 1 Sec Delay
}

void IRAM_ATTR stockISR(){ 
  int i;
  while(digitalRead(BUT_STOCK)==HIGH){
    yield(); //This will prevent wdt reset
  }

  for(i = 0; i<32; i++){
    rand_vals[i] = rand() % 8+1;
  }

  display_trend_graph(rand_vals);
  
  Serial.println("Stock interrupt");
  delay(1000);  //Simple 1 Sec Delay
}

const int capacity = JSON_OBJECT_SIZE(2);
StaticJsonDocument<capacity> doc;

WebsocketsClient client;

void setup() {
  pinMode(BUT_TIME, INPUT_PULLUP);
  pinMode(BUT_STOCK, INPUT_PULLUP);

  Serial.begin(115200);
  WiFi.begin(ssid, password);             // Connect to the network
  int i=0;
  while (WiFi.status() != WL_CONNECTED) { // Wait for the Wi-Fi to connect
  delay(1000);
  Serial.print(++i); Serial.print(' ');
  }

  matrix.setIntensity(1);    // Use a value between 0 and 15 for brightness
  for(i = 0; i<4; i++){
    matrix.setRotation(i, 3);
  }

  lcd.init();
  // Turn on the backlight.
  lcd.backlight();
  attachInterrupt(digitalPinToInterrupt(BUT_TIME), timeISR, FALLING); // trigger when button pressed, but not when released.
  attachInterrupt(digitalPinToInterrupt(BUT_STOCK), stockISR, FALLING); // trigger when button pressed, but not when released.

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

//  // Interval in microsecs
//  if (ITimer.attachInterruptInterval(TIMER_INTERVAL_MS * 1000 * 30, TimerHandler))
//  {
//    lastMillis = millis();
//  }
//  else
//    Serial.println(F("Can't set ITimer correctly. Select another freq. or interval"));
}
 
void loop() {
  client.poll();
}
