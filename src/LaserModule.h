#ifndef LASERMODULE_H
#define LASERMODULE_H
#include <Arduino.h>

class LaserModule {
public:
  LaserModule() {
    analogWrite(LASER_EN, 0);
    pinMode(LASER_EN, OUTPUT);
  }

  void power(uint8_t power) {
    if (power > 200) {
      digitalWrite(LASER_EN, HIGH);
      Serial.println("Laser FULL POWER");
    } else {
      analogWrite(LASER_EN, power);
      Serial.print("Laser power: ");
      Serial.println(power);
    }
  }

  void off() { analogWrite(LASER_EN, 0); }

private:
  static constexpr const uint8_t LASER_EN = 9;
};
#endif