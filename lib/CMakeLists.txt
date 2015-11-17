
INCLUDE_DIRECTORIES(${INCLUDE_DIR})

SET(SRCS
    Adafruit_MotorShield.cpp 
    Adafruit_PWMServoDriver.cpp)

ADD_LIBRARY(Adafruit_Motor_Shield SHARED ${SRCS})
