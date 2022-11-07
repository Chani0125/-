#include <Servo.h>

// Arduino pin assignment

// #define PIN_POTENTIOMETER 3 // Potentiometer at Pin A3
// Add IR Sensor Definition Here !!!
#define PIN_IRSENSOR 0
#define PIN_SERVO 10
#define PIN_LED 9

#define DIST_MIN 100
#define DIST_MAX 250

#define _DUTY_MIN 553  // servo full clock-wise position (0 degree)
#define _DUTY_NEU 1476 // servo neutral position (90 degree)
#define _DUTY_MAX 2399 // servo full counter-clockwise position (180 degree)

#define LOOP_INTERVAL 20   // Loop Interval (unit: msec)
#define _EMA_ALPHA 0.5

float dist, dist_ema, dist_prev = DIST_MAX;


Servo myservo;
unsigned long last_loop_time;   // unit: msec

void setup()
{
  pinMode(PIN_LED, OUTPUT);
  
  myservo.attach(PIN_SERVO); 
  myservo.writeMicroseconds(_DUTY_NEU);
  
  Serial.begin(1000000);
}

void loop()
{
  unsigned long time_curr = millis();
  int duty;

  // wait until next event time
  if (time_curr < (last_loop_time + LOOP_INTERVAL))
    return;
  last_loop_time += LOOP_INTERVAL;

  // Read IR Sensor value !!!
  int a_value = analogRead(PIN_IRSENSOR);
  
  // Convert IR sensor value into distance !!!
  dist = (6762.0/(a_value-9)-4.0) * 10.0 - 60.0;
  
  // we need distance range filter here !!!
  if (dist < DIST_MIN) {
    dist = dist_prev;           // cut lower than minimum
    digitalWrite(PIN_LED, 1);       // LED OFF
  } else if (dist > DIST_MAX) {
    dist = dist_prev;           // Cut higher than maximum
    digitalWrite(PIN_LED, 1);       // LED OFF
  } else {    // In desired Range
    digitalWrite(PIN_LED, 0);       // LED ON      
    dist_prev = dist;
  }
  
  // we need EMA filter here !!!
  dist_ema = dist * _EMA_ALPHA + dist_ema * (1 - _EMA_ALPHA);

  if (dist_ema <= DIST_MIN)
    duty = _DUTY_MIN;
  else if (dist_ema >= DIST_MAX)
    duty = _DUTY_MAX;
  else
    duty = (_DUTY_MAX - _DUTY_MIN) * ((dist_ema - DIST_MIN) / (DIST_MAX - DIST_MIN)) + _DUTY_MIN;
  myservo.writeMicroseconds(duty);
  
  // print IR sensor value, distnace, duty !!!
  Serial.print("MIN:");    Serial.print(DIST_MIN);
  Serial.print(",IR:");    Serial.print(a_value);
  Serial.print(",dist:");  Serial.print(dist);
  Serial.print(",ema:");   Serial.print(dist_ema);
  Serial.print(",servo:"); Serial.print(duty);
  Serial.print(",Max:");   Serial.print(DIST_MAX);
  Serial.println("");
}
