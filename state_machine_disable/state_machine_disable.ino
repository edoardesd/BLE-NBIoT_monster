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
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT);

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

  forceNBIOT = digitalRead(buttonPin);
  if(forceNBIOT) {
    digitalWrite(ledPin, LOW);
  } else {
    digitalWrite(ledPin, HIGH);
  }
  if(oldForceNBIOT != forceNBIOT){
    forceNBIOT ? Serial.println(F("WARN: no NBIoT")) : Serial.println(F("WARN: actv NBIoT"));
    oldForceNBIOT = forceNBIOT;
  }

  if (stateMachine.Update()) {
    Serial.print(F("St: "));
    Serial.println(stateMachine.ActiveStateName());
  }
}