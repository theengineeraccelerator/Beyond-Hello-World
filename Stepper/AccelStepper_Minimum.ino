//AccelStepper Minimum
// Malcolm Knapp
//
// This is the minimum code you need to run a stepper from it start position on revolution

#include <AccelStepper.h>
long pos = 1600*9;

AccelStepper stepper(AccelStepper::DRIVER, 2, 3) ; // Driver interface with Step on pin 2 and Direction on pin 3

void setup() {
  // put your setup code here, to run once:
  Serial.begin (115200);
  Serial.print("Move on revolution");
  stepper.setMaxSpeed(500);
  stepper.setAcceleration(200);
  stepper.moveTo(pos);
}

void loop() {
  // put your main code here, to run repeatedly:
  stepper.run();

}
