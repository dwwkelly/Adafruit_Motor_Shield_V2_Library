#include <cstdio>
#include <unistd.h>
#include <Adafruit_MotorShield/Adafruit_MotorShield.h>


int main(int argc, char *argv[])
{

    Adafruit_MotorShield AFMS =  Adafruit_MotorShield(10, 0x60);

    Adafruit_StepperMotor *myMotor = AFMS.getStepper(513, 1);
    AFMS.begin(1600);

    myMotor->setSpeed(5);
    int i = 0;

    while (true) {
        usleep(2e6);
        printf("Step\n");
        for (i=0; i<255; i++) {
            myMotor->step(1, FORWARD, INTERLEAVE);
            usleep(1000);
        }
    }

    return 0;
}

