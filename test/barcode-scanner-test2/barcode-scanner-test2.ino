#include "PS2Keyboard.h"

#define DATA_PIN A0
PS2Keyboard keyboard;

void setup() {
  keyboard.begin(A0, A1);

  Serial.begin(9600);
  Serial.println("hi");
  delay(1000);
}

void loop() {
  if(keyboard.available()) {
    byte dat = keyboard.read();
    byte val = dat - '0';
    Serial.println("rilevata tastiera");
    if(val >= 0 && val <= 9) {
      Serial.print(val, DEC);
    } else if(dat == PS2_ENTER) {
      Serial.println();
    } else if(dat == PS2_ESC) {
      Serial.println("[ESC]");
    } 
  }
}

