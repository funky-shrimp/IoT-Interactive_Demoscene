#ifndef POTENTIOMETER_H
#define POTENTIOMETER_H

/*
Simple class for Potentiometer
*/

class Potentiometer {
private:
  int pin;
  float value = 0;

public:
  Potentiometer(int pinIn)
    : pin(pinIn) {}

  float getValue() {
    value = analogRead(pin);
    return value;
  }

  static long mapPotValue(long value, long from, long to) {
    return map(value, 0, 1023, from, to);
  }

  void init() {
    pinMode(pin, INPUT);

    Serial.print("Potentiometer on analog pin ");
    Serial.print("A");
    Serial.print(pin);
    Serial.println(" initiated");
  }
};


#endif