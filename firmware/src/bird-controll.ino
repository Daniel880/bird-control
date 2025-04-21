#include "LaserModule.h"
#include "SerialParser.h"
#include "StepperController.h"

// Define a stepper and the pins it will use

StepperController stepperController;
LaserModule laserModule;
SerialParser serialParser;

void setup() {
  laserModule.off();

  Serial.begin(115200);
  Serial.println("Start");

  pinMode(ENDSTOP_X, INPUT_PULLUP);
  pinMode(ENDSTOP_Y, INPUT_PULLUP);

  pinMode(STEPPER_ENABLE, OUTPUT);
  digitalWrite(STEPPER_ENABLE, LOW);

  stepperController.initTMC();
  stepperController.initAccelStepper();

  stepperController.setHommingSettings();
  stepperController.home_Y();
  stepperController.home_X();

  stepperController.setRunSettings();
  Serial.println("START");
}

long speed = 1000;
uint16_t power = 0;
uint8_t loopCnt = 0;
long x = 1000;
long y = 1000;

void loop() {
  if (loopCnt == 0) {
    speed = 1000;
    power = 10;
    loopCnt++;
  } else if (loopCnt == 1) {
    speed = 5000;
    power = 128;
    loopCnt++;
  } else {
    speed = 10000;
    power = 255;
  }

  laserModule.power(power);
  stepperController.moveTo(500, 6700, speed);
  stepperController.moveTo(640, 6260, speed);

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
