#include <SoftwareSerial.h>
#include <string.h>
#include <YA_FSM.h>  // https://github.com/cotestatnt/YA_FSM
#include <MemoryFree.h>
#include <ArduinoUniqueID.h>

#include "state_machine_disable.h"
#include "fsm.h"

void setup() {
  pinMode(powerPin, OUTPUT);
  digitalWrite(powerPin, HIGH);
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(BAUD_RATE);
  NBIOTSerial.begin(BAUD_RATE);
  BLESerial.begin(BAUD_RATE);
  delay(2000);

	UniqueIDdump(Serial);
  // for (i = 0; i < UniqueIDsize; i++)
	// {
	// 	if (UniqueID[i] < 0x10)
	// 		Serial.print("0");
  //   Serial.print(UniqueID[i]);
	// 	Serial.print(" ");
	// }

  if(UniqueID[8] == 25){
    Serial.println(F("Sensor B"));
    BLENAME = "m0";
    SLEEP_TIME = 25000;
  }
  if(UniqueID[8] == 22){
    Serial.println(F("Sensor N"));
    BLENAME = "m1";
    SLEEP_TIME = 10000;
    Serial.println(SLEEP_TIME);
  }
  
  Serial.println(F("Start FSM"));
  setupStateMachine();
  //switch from INIT to SETUP
  resetState = true;
}

void loop() {
  readSerial();
  readBLE();
  readNBIOT();


  if (stateMachine.Update()) {
    Serial.print(F("State: "));
    Serial.println(stateMachine.ActiveStateName());
  }
}