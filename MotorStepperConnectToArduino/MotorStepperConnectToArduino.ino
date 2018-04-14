/*
 * MotorKnob
 *
 * A stepper motor follows the turns of a potentiometer
 * (or other sensor) on analog input 0.
 *
 * http://www.arduino.cc/en/Reference/Stepper
 * This example code is in the public domain.
 */
#define STEPS 64
#include <Stepper.h>

Stepper stepper(STEPS, 8, 9, 10, 11);

void setup() {
  Serial.begin(9600);
  stepper.setSpeed(350);
  pinMode(3, INPUT);
}

void loop() {
//
  if(digitalRead(3) == HIGH)
  {
    stepper.step(230);
    delay(3520);
    stepper.step(800);
    delay(1000);
  }
 }
