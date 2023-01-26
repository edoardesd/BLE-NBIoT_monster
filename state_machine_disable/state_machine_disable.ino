#include <SoftwareSerial.h>
#include <string.h>
#include <YA_FSM.h>  // https://github.com/cotestatnt/YA_FSM
#include <MemoryFree.h>
#include <ArduinoUniqueID.h>

#include "state_machine_disable.h"
#include "fsm.h"
#include "serial.h"
#include "const_cmd.h"

void setup() {
  pinMode(powerPin, OUTPUT);
  digitalWrite(powerPin, HIGH);
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(BAUD_RATE);
  NBIOTSerial.begin(BAUD_RATE);
  BLESerial.begin(BAUD_RATE);
  delay(2000);
  
  setupId();
  setupStateMachine();
  //switch from INIT to SETUP
  resetState = true;
}

void loop() {
  readSerial();
  readBLE();
  readNBIOT();

  if (stateMachine.Update()) {
    Serial.print(F("St: "));
    Serial.println(stateMachine.ActiveStateName());
  }
}