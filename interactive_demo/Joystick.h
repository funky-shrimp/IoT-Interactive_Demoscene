#ifndef JOYSTICK_H
#define JOYSTICK_H

struct jskValues {
  int x;
  int y;
  int btn;
};

class Joystick {
private:
  //Pointer for RAM optimization
  const int* pins;

public:
  //Constructor
  Joystick(const int* pinsArray)
    : pins(pinsArray) {}  

  //Returning Joystick state
  jskValues getValue() {

    jskValues values;
    values.x = analogRead(pins[0]);
    values.y = analogRead(pins[1]);
    values.btn = digitalRead(pins[2]);

    return values;
  }

  //Loging the state of the Joystick
  void log() {
    Serial.print("Button :  ");
    Serial.print(digitalRead(pins[2]));
    Serial.print(" | ");
    Serial.print("X-axis : ");
    Serial.print(analogRead(pins[0]));
    Serial.print(" | ");
    Serial.print("Y-axis : ");
    Serial.print(analogRead(pins[1]));
    Serial.println(" | ");
    delay(200);
  }

  //Initialize joystick
  void init() {
    Serial.print("Joystick initiated on pins ");

    //X
    pinMode(pins[0], INPUT);
    Serial.print(pins[0]);
    Serial.print(",");

    //Y
    pinMode(pins[1], INPUT);
    Serial.print(pins[1]);
    Serial.print(",");

    //Last pin is assumed to be the button
    pinMode(pins[2], INPUT);
    digitalWrite(pins[2], HIGH);
    Serial.print(pins[2]);
    Serial.print(",");

    Serial.println("");
  }
};

#endif