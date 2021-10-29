const int BUT_TIME = D6;
int but_time_state = 0;

const int BUT_STOCK = D8;
int but_stock_state = 0;

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  // initialize the pushbutton pin as an input:
  pinMode(BUT_TIME, INPUT_PULLUP);
  pinMode(BUT_STOCK, INPUT_PULLUP);

}

void loop() {
  // read the state of the pushbutton value:
  but_time_state = digitalRead(BUT_TIME);
  but_stock_state = digitalRead(BUT_STOCK);

  // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
  if (but_time_state == HIGH) {
    // turn LED on:
    digitalWrite(LED_BUILTIN, LOW);
    Serial.println("BUT_TIME");
  } else if(but_stock_state == HIGH) {
    // turn LED off:
    digitalWrite(LED_BUILTIN, HIGH);
    Serial.println("BUT_STOCK");
  }
}
