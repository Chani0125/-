#include <Servo.h>

// Arduino pin assignment

// #define PIN_POTENTIOMETER 3 // Potentiometer at Pin A3
// Add IR Sensor Definition Here !!!
#define PIN_IRSENSOR 0
#define PIN_SERVO 10

#define DIST_MIN 1000
#define DIST_MAX 2500

#define _DUTY_MIN 553  // servo full clock-wise position (0 degree)
#define _DUTY_NEU 1476 // servo neutral position (90 degree)
#define _DUTY_MAX 2399 // servo full counter-clockwise position (180 degree)

#define LOOP_INTERVAL 20   // Loop Interval (unit: msec)
#define _EMA_ALPHA 0.5

float dist, dist_ema, dist_prev = _DIST_MAX;


Servo myservo;
unsigned long last_loop_time;   // unit: msec

void setup()
{
  myservo.attach(PIN_SERVO); 
  myservo.writeMicroseconds(_DUTY_NEU);
  
  Serial.begin(1000000);
}

void loop()
{
  unsigned long time_curr = millis();
  int a_value, duty;

  // wait until next event time
  if (time_curr < (last_loop_time + LOOP_INTERVAL))
    return;
  last_loop_time += LOOP_INTERVAL;

  // Remove Next line !!!
  // a_value = analogRead(PIN_POTENTIOMETER);
  a_value = analogRead(PIN_IRSENSOR);
  // Read IR Sensor value !!!
  
  // Convert IR sensor value into distance !!!
  // we need distance range filter here !!!
  // we need EMA filter here !!!

  // map distance into duty
  duty = map(a_value, 0, 1023, _DUTY_MIN, _DUTY_MAX);
  myservo.writeMicroseconds(duty);
  
  if (dist < _DIST_MIN) {
    dist = dist_prev;           // cut lower than minimum
    digitalWrite(PIN_LED, 1);       // LED OFF
  } else if (dist > _DIST_MAX) {
    dist = dist_prev;           // Cut higher than maximum
    digitalWrite(PIN_LED, 1);       // LED OFF
  } else {    // In desired Range
    digitalWrite(PIN_LED, 0);       // LED ON      
    dist = dist_raw;
  }

  if (dist_ema <= _TARGET_LOW)
    myservo.writeMicroseconds(_DUTY_MIN);
  else if (dist_ema >= _TARGET_HIGH)
    myservo.writeMicroseconds(_DUTY_MAX);
  else
    myservo.writeMicroseconds((_DUTY_MAX - _DUTY_MIN) * ((dist_ema - _TARGET_LOW) / (_TARGET_HIGH - _TARGET_LOW)) + _DUTY_MIN);
  
  // print IR sensor value, distnace, duty !!!
  Serial.print("MIN:");    Serial.print(DIST_MIN);
  Serial.print(",IR:");  Serial.print(a_value);
  Serial.print(",dist:");  Serial.print(dist);
  Serial.print(",ema:");  Serial.print(dist_ema);
  Serial.print(",servo:"); Serial.print(duty);
  Serial.print(",Max:");   Serial.print(DIST_MAX);
  Serial.println("");
}
