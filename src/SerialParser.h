#ifndef SERIALPARSER_H
#define SERIALPARSER_H
#include <Arduino.h>

class SerialParser {
public:
  SerialParser() = default;
  ~SerialParser() = default;

  bool read() {
    while (Serial.available()) {
      char c = Serial.read();
      if (c == '\n') {
        parseNumbers(input);
        input = "";
        return true;
      } else {
        input += c;
      }
    }
    return false;
  }

  void parseNumbers(String data) {
    numCount = 0;
    int startIdx = 0;
    for (unsigned int i = 0; i <= data.length(); i++) {
      if (data[i] == ',' || i == data.length()) {
        String numStr = data.substring(startIdx, i);
        numbers[numCount++] = numStr.toInt();
        startIdx = i + 1;
      }
    }
  }

  int numbers[10] = {0};
  int numCount = 0;
  String input = "";
};
#endif