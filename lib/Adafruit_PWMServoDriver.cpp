/***************************************************
  This is a library for our Adafruit 16-channel PWM & Servo driver

  Pick one up today in the adafruit shop!
  ------> http://www.adafruit.com/products/815

  These displays use I2C to communicate, 2 pins are required to
  interface. For Arduino UNOs, thats SCL -> Analog 5, SDA -> Analog 4

  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  BSD license, all text above must be included in any redistribution
 ****************************************************/

#include "Adafruit_MotorShield/Adafruit_PWMServoDriver.h"

Adafruit_PWMServoDriver::Adafruit_PWMServoDriver() {}

Adafruit_PWMServoDriver::Adafruit_PWMServoDriver(uint8_t bus, uint8_t addr) {
    _i2c_bus = bus;
    _i2c_addr = addr;

    snprintf(filename, 127, "/dev/i2c-%u", (unsigned int) _i2c_bus);
    fd = open(filename, O_RDWR);
    if (fd < 0) {
        std::cout << "Adafruit_PWMServoDriver::Adafruit_PWMServoDriver Error: "
                  << strerror(errno) << std::endl;
        exit(1);
    }

}

void Adafruit_PWMServoDriver::begin(void) {

    if (ioctl(fd, I2C_SLAVE, _i2c_addr) < 0) {
        std::cout << "Adafruit_PWMServoDriver::begin Error: "
                  << strerror(errno) << std::endl;
        exit(1);
    }

    reset();
}


void Adafruit_PWMServoDriver::reset(void) {
    write8(PCA9685_MODE1, 0x00);
}

void Adafruit_PWMServoDriver::setPWMFreq(float freq) {

  freq *= 0.9;  // Correct for overshoot in the frequency setting (see issue #11).

  float prescaleval = 25000000;
  prescaleval /= 4096;
  prescaleval /= freq;
  prescaleval -= 1;
  uint8_t prescale = floor(prescaleval + 0.5);

  uint8_t oldmode = read8(PCA9685_MODE1);
  uint8_t newmode = (oldmode&0x7F) | 0x10; // sleep
  write8(PCA9685_MODE1, newmode); // go to sleep
  write8(PCA9685_PRESCALE, prescale); // set the prescaler
  write8(PCA9685_MODE1, oldmode);
  usleep(5000);  // Sleep for 5 ms
  write8(PCA9685_MODE1, oldmode | 0xa1);  //  This sets the MODE1 register to turn on auto increment.
                                          // This is why the beginTransmission below was not working.
}

void Adafruit_PWMServoDriver::setPWM(uint8_t num, uint16_t on, uint16_t off) {
    uint8_t addr = LED0_ON_L + 4 * num;

    write8(addr, on);
    write8(addr + 1, (on >> 8));
    write8(addr + 2, off);
    write8(addr + 3, (off >> 8));

}

uint8_t Adafruit_PWMServoDriver::read8(uint8_t addr) {

    unsigned char buf[1];
    *buf = (unsigned char) addr;

    if (read(fd, buf, 1) != 1) {
        std::cout << "Adafruit_PWMServoDriver::read8 Error: "
                  << strerror(errno) << std::endl;
        exit(1);
    } else {
        return (uint8_t) *buf;
    }
}

void Adafruit_PWMServoDriver::write8(uint8_t addr, uint8_t data) {
    uint8_t buf[2];
    buf[0] = addr;
    buf[1] = data;

    printf("writing %x to %x\n", data, addr);

    if (write(fd, buf, 2) != 2) {
        std::cout << "Adafruit_PWMServoDriver::write8 Error: "
                  << strerror(errno) << std::endl;
        exit(1);
    }
}
