#include <Adafruit_GFX.h>
#include <Max72xxPanel.h>
#include <ESP8266WiFi.h>  
#include <SPI.h>

//################# DISPLAY CONNECTIONS ################
// LED Matrix Pin -> ESP8266 Pin
// Vcc            -> 3v  (3V on NodeMCU 3V3 on WEMOS)
// Gnd            -> Gnd (G on NodeMCU)
// DIN            -> D7  (Same Pin for WEMOS)
// CS             -> D4  (Same Pin for WEMOS)
// CLK            -> D5  (Same Pin for WEMOS)

#define HARDWARE_TYPE MD_MAX72XX::FC16_HW

int pinCS = D4; // Attach CS to this pin, DIN to MOSI and CLK to SCK (cf http://arduino.cc/en/Reference/SPI )
int numberOfHorizontalDisplays = 4;
int numberOfVerticalDisplays   = 1;

char time_value[20];
String message;


Max72xxPanel matrix = Max72xxPanel(pinCS, numberOfHorizontalDisplays, numberOfVerticalDisplays);

int wait   = 75; // In milliseconds between scroll movements
int spacer = 1;
int width  = 5 + spacer; // The font width is 5 pixels

void setup() {
  Serial.begin(115200); // initialize serial communications
  message= "PINEAPPLE ON PIZZA";
  matrix.setIntensity(1);    // Use a value between 0 and 15 for brightness
  int i;
  for(i = 0; i<4; i++){
    matrix.setRotation(i, 1);
  }
}

void loop() {
//  display_message(message); // Display the message
  int rand_vals[32];
  int i;
  for(i = 0; i<32; i++){
    rand_vals[i] = rand() % 8+1;
    Serial.print(i);
  }

  display_trend_graph(rand_vals);
}

void display_message(String message) {
  for ( int i = 0 ; i < width * message.length() + matrix.width() - spacer; i++ ) {
    int letter = i / width;
    int x = (matrix.width() - 1) - i % width;
    int y = (matrix.height() - 8) / 2; // center the text vertically
    while ( x + width - spacer >= 0 && letter >= 0 ) {
      if ( letter < message.length() ) {
        matrix.drawChar(x, y, message[letter], HIGH, LOW, 1); // HIGH LOW means foreground ON, background OFF, reverse these to invert the display!
      }
      letter--;
      x -= width;
    }
    matrix.write(); // Send bitmap to display
    delay(wait / 2);
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
