#ifndef STEPPER_CONTROLLER_H
#define STEPPER_CONTROLLER_H
#include <AccelStepper.h>
#include <SoftwareSerial.h>
#include <TMC2209.h>

const uint8_t RX_PIN = A0;
const uint8_t TX_PIN = 11; // z end

SoftwareSerial soft_serial(RX_PIN, TX_PIN);

#define STEPPER_ENABLE 8
#define STEP_X 2
#define DIR_X 5
#define ENDSTOP_X 12

#define STEP_Y 3
#define DIR_Y 6
#define ENDSTOP_Y 13

AccelStepper stepper_X(
    AccelStepper::DRIVER, STEP_X,
    DIR_X); // Defaults to AccelStepper::FULL4WIRE (4 pins) on 2, 3, 4, 5
AccelStepper stepper_Y(
    AccelStepper::DRIVER, STEP_Y,
    DIR_Y); // Defaults to AccelStepper::FULL4WIRE (4 pins) on 2, 3, 4, 5

class StepperController {
public:
  TMC2209 stepper_driver;
  StepperController() { stepper_driver.setup(soft_serial); }

  void initAccelStepper() {

    stepper_X.setMaxSpeed(10000);
    stepper_X.setAcceleration(10000);
    stepper_X.setSpeed(10000);

    stepper_Y.setMaxSpeed(10000);
    stepper_Y.setAcceleration(10000);
    stepper_Y.setSpeed(10000);
  }

  void initTMC() {
    stepper_driver.setRunCurrent(100);
    stepper_driver.setRMSCurrent(1500, 0.11, 1.0);
    stepper_driver.enableCoolStep();
    stepper_driver.enable();
  }

  void setHommingSettings() { stepper_driver.setMicrostepsPerStep(1); }

  void setRunSettings() { stepper_driver.setMicrostepsPerStep(256); }

  void home_Y() {
    Serial.println("Y homming");
    stepper_Y.setSpeed(-20);

    while (digitalRead(ENDSTOP_Y) == HIGH) {
      stepper_Y.runSpeed();
    }

    stepper_Y.setSpeed(10);
    while (digitalRead(ENDSTOP_Y) == LOW) {
      stepper_Y.runSpeed();
    }

    stepper_Y.setSpeed(-5);
    while (digitalRead(ENDSTOP_Y) == HIGH) {
      stepper_Y.runSpeed();
    }

    stepper_Y.setSpeed(5);
    while (digitalRead(ENDSTOP_Y) == LOW) {
      stepper_Y.runSpeed();
    }

    stepper_Y.setCurrentPosition(0);
  }

  void home_X() {
    Serial.println("X homming");
    stepper_X.setSpeed(20);

    while (digitalRead(ENDSTOP_X) == HIGH) {
      stepper_X.runSpeed();
    }

    stepper_X.setSpeed(-10);
    while (digitalRead(ENDSTOP_X) == LOW) {
      stepper_X.runSpeed();
    }

    stepper_X.setSpeed(5);
    while (digitalRead(ENDSTOP_X) == HIGH) {
      stepper_X.runSpeed();
    }

    stepper_X.setSpeed(-5);
    while (digitalRead(ENDSTOP_X) == LOW) {
      stepper_X.runSpeed();
    }

    stepper_X.setCurrentPosition(0);

    // stepper_X.moveTo(-39);
    // stepper_X.setSpeed(-20);
    // while (stepper_X.distanceToGo() != 0) {
    // stepper_X.runSpeedToPosition();
    // }

    stepper_X.setCurrentPosition(0);
  }

  void moveRelative(int32_t x, int32_t y, uint32_t time) {

    long speedX = 1000.0 * x / time;
    long speedY = 1000.0 * y / time;

    stepper_X.move(x);
    stepper_X.setSpeed(speedX);
    stepper_Y.move(y);
    stepper_Y.setSpeed(speedY);
    while (stepper_X.distanceToGo() != 0 || stepper_Y.distanceToGo() != 0) {
      stepper_X.runSpeedToPosition();
      stepper_Y.runSpeedToPosition();
    }
    Serial.print("Position:\t");
    Serial.print(stepper_X.currentPosition());
    Serial.print("\t");
    Serial.println(stepper_Y.currentPosition());
  }

  void moveTo(int32_t x, int32_t y, uint32_t time) {
    long currentX = stepper_X.currentPosition();
    long currentY = stepper_Y.currentPosition();

    long distanceToGoX = x - currentX;
    long distanceToGoY = y - currentY;

    long speedX = 1000.0 * distanceToGoX / time;
    long speedY = 1000.0 * distanceToGoY / time;

    stepper_X.moveTo(x);
    stepper_X.setSpeed(speedX);
    stepper_Y.moveTo(y);
    stepper_Y.setSpeed(speedY);
    while (stepper_X.distanceToGo() != 0 || stepper_Y.distanceToGo() != 0) {
      stepper_X.runSpeedToPosition();
      stepper_Y.runSpeedToPosition();
    }
    Serial.print("Position:\t");
    Serial.print(stepper_X.currentPosition());
    Serial.print("\t");
    Serial.println(stepper_Y.currentPosition());
  }

  void testEndstop() {
    while (true) {

      stepper_X.setSpeed(50);
      while (digitalRead(ENDSTOP_X) == HIGH) {
        stepper_X.runSpeed();
      }
      Serial.print("X: ");
      Serial.print(stepper_X.currentPosition());

      stepper_X.setSpeed(-50);
      while (digitalRead(ENDSTOP_X) == LOW) {
        stepper_X.runSpeed();
      }
      Serial.print(" ");
      Serial.print(stepper_X.currentPosition());

      stepper_Y.setSpeed(-50);
      while (digitalRead(ENDSTOP_Y) == HIGH) {
        stepper_Y.runSpeed();
      }
      Serial.print(" Y: ");
      Serial.print(stepper_Y.currentPosition());

      stepper_Y.setSpeed(50);
      while (digitalRead(ENDSTOP_Y) == LOW) {
        stepper_Y.runSpeed();
      }
      Serial.print(" ");
      Serial.print(stepper_Y.currentPosition());
      Serial.println();
    }
  }
};
#endif