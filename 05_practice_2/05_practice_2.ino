#define PIN_LED 7
unsigned int count, toggle;

void setup() {
  pinMode(PIN_LED, OUTPUT);
  count = toggle = 0;
  digitalWrite(PIN_LED, toggle);
}

void loop() {
  digitalWrite(PIN_LED, toggle);
  delay(100);
  count++;
  if((count >= 10) and count <= 20) {
    toggle = 1 - toggle;
  }
}
