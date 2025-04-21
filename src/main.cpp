#include "LaserModule.h"
#include "SerialParser.h"
#include "StepperController.h"

// Define a stepper and the pins it will use

StepperController stepperController;
LaserModule laserModule;
SerialParser serialParser;

struct LaserPoint {
  long x;
  long y;
  long time;
};

constexpr const size_t MAX_LASER_POINTS = 20;

const LaserPoint laserPoints[MAX_LASER_POINTS] = {};

void setup() {
  laserModule.off();

  Serial.begin(115200);
  Serial.println("Start");

  pinMode(ENDSTOP_X, INPUT_PULLUP);
  pinMode(ENDSTOP_Y, INPUT_PULLUP);

  stepperController.initTMC();
  stepperController.initAccelStepper();

  stepperController.setHommingSettings();
  stepperController.home_Y();
  stepperController.home_X();

  stepperController.setRunSettings();
  Serial.println("START");

  stepperController.testEndstop();
}

uint8_t laserPointIndex = 0;

void loop() {

  Serial.print("Moving to laser point: idx: ");
  Serial.print(laserPointIndex);
  Serial.print("\t");
  Serial.print(laserPoints[laserPointIndex].x);
  Serial.print("\t");
  Serial.print(laserPoints[laserPointIndex].y);
  Serial.print("\t");
  Serial.println(laserPoints[laserPointIndex].time);

  stepperController.moveTo(laserPoints[laserPointIndex].x,
                           laserPoints[laserPointIndex].y,
                           laserPoints[laserPointIndex].time);

  do {
    laserPointIndex++;
    if (laserPointIndex >= MAX_LASER_POINTS) {
      laserPointIndex = 0;
    }
  } while (laserPoints[laserPointIndex].time == 0);

  // 0 relative
  // 1 absolute
  // 2,0 laser OFF
  // 2,255 laser ON

  if (serialParser.read()) {
    Serial.print("Odebrano dane:");
    for (int i = 0; i < serialParser.numCount; i++) {
      Serial.print('\t');
      Serial.print(serialParser.numbers[i]);
    }
    Serial.println();
  }

  if (serialParser.numbers[0] == 0) {
    // relative
    stepperController.moveRelative(serialParser.numbers[1],
                                   serialParser.numbers[2],
                                   serialParser.numbers[3]);
  }

  if (serialParser.numbers[0] == 1) {
    // absolute
    stepperController.moveTo(serialParser.numbers[1], serialParser.numbers[2],
                             serialParser.numbers[3]);
  }

  if (serialParser.numbers[0] == 2) {
    if (serialParser.numbers[1] > 0 && serialParser.numbers[1] < 256) {
      laserModule.power(serialParser.numbers[1]);
    } else {
      laserModule.off();
    }
  }

  serialParser.numbers[0] = -1;
}