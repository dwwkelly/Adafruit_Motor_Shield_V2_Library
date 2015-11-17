
#include <cstdio>
#include "Adafruit_MotorShield/Adafruit_MotorShield.h"

// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield(10, 0x60);
// Or, create it with a different I2C address (say for stacking)
// Adafruit_MotorShield AFMS = Adafruit_MotorShield(0x61);

// Connect a stepper motor with 200 steps per revolution (1.8 degree)
// to motor port #2 (M3 and M4)
Adafruit_StepperMotor *myMotor = AFMS.getStepper(200, 2);


void setup() {
  printf("Stepper Test\n");

  AFMS.begin();  // create with the default frequency 1.6KHz
  //AFMS.begin(1000);  // OR with a different frequency, say 1KHz

  myMotor->setSpeed(10);  // 10 rpm
}

void loop() {
  printf("Single coil steps\n");
  myMotor->step(100, FORWARD, SINGLE);
  myMotor->step(100, BACKWARD, SINGLE);

  printf("Double coil steps\n");
  myMotor->step(100, FORWARD, DOUBLE);
  myMotor->step(100, BACKWARD, DOUBLE);

  printf("Interleave coil steps\n");
  myMotor->step(100, FORWARD, INTERLEAVE);
  myMotor->step(100, BACKWARD, INTERLEAVE);

  printf("Microstep steps\n");
  myMotor->step(50, FORWARD, MICROSTEP);
  myMotor->step(50, BACKWARD, MICROSTEP);
}

int main(int argc, char *argv[])
{
    setup();
    loop();
    return 0;
}
