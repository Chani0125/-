#include <Servo.h>
#define PIN_SERVO 10

Servo myservo;

void setup() {
  myservo.attach(PIN_SERVO); 
  myservo.writeMicroseconds(553);
  delay(1000);

  Serial.begin(57600);
}

void loop() {
}
