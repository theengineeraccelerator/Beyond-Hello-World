// Bounce.pde
// -*- mode: C++ -*-
//
// Make a single stepper bounce from one limit to another
//
// Copyright (C) 2012 Mike McCauley
// $Id: Random.pde,v 1.1 2011/01/05 01:51:01 mikem Exp mikem $

#include <AccelStepper.h>
#define MS1 4
#define MS2 5
#define EN  6

long startTime = 0;
int numBounces = 0;
long pos = 800;

// Define a stepper and the pins it will use
AccelStepper stepper(AccelStepper::DRIVER, 2, 3) ; // Driver interface with Step on pin 2 and Direction on pin 3
void setup()
{
  Serial.begin (115200);
  pinMode(MS1, OUTPUT);
  pinMode(MS2, OUTPUT);
  pinMode(EN, OUTPUT);
  //digitalWrite(EN, LOW);
  digitalWrite(MS1, LOW);
  digitalWrite(MS2, LOW);
  // Change these to suit your stepper if you want
  stepper.setMaxSpeed(500);
  stepper.setAcceleration(200);
  // from -pos to +pos is 1600steps at 1/8th micro
  stepper.setCurrentPosition(pos); 
  stepper.moveTo(-pos);
}

void loop()
{
  // If at the end of travel go to the other end
  if (stepper.distanceToGo() == 0) {
    if (numBounces < 3) {
      stepper.moveTo(-stepper.currentPosition());
      numBounces++;
    }
  }
  startTime = micros();
  stepper.run();
  //Serial.print ("Processing time "); Serial.println(micros() - startTime);
}
