#include <Servo.h>
#define PIN_SERVO 10

Servo myservo;

void setup() {
  myservo.attach(PIN_SERVO); 
  myservo.write(0);
  delay(1000);

  Serial.begin(57600);
}

void loop() {
  myservo.write(0);
  delay(1000);
  Serial.println(myservo.read());
  myservo.write(180);
  delay(1000);
  Serial.println(myservo.read());
}
