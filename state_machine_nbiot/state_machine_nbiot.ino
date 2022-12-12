#include <SoftwareSerial.h>
#include <string.h>
#include <YA_FSM.h>  // https://github.com/cotestatnt/YA_FSM

#define NBIOTSerial Serial1
#define powerPin 7 
#define BLENAME "meter0"
#define NUM_SETUPOPERATIONS 5

const char *BLE_TAG = "BLE+";
const char *NBIOT_TAG = "NB+";
const char *RESET_NBIOT_TAG = "UFOTAS";
/***** SERIAL READ VARS *****/
unsigned long BAUD_RATE = 9600;
const int BLUETOOTH_TX = 9;
const int BLUETOOTH_RX = 8;
const int READ_TIME = 500; //ms

unsigned long prevMillis;

SoftwareSerial BLESerial(BLUETOOTH_RX, BLUETOOTH_TX);

String outputNBIOT;
int okNBIOTList = 0;
int okBLEList = 0;
int nbIoToldState = -1;
int BLEoldState = -1;
String setupIoTList[NUM_SETUPOPERATIONS] = {"AT+CMEE=1", "AT+CFUN=1", "AT+CGDCONT=1,\"IP\",\"nb.inetd.gdsp\"", "AT+CEREG=2", "AT+COPS=1,2,\"22210\""};
String cmd;

char *strremove(char *str, const char *sub);
void readSerial();
void readBLE();
void readNBIOT();

/***** FINITE STATE MACHINE VARS *****/
YA_FSM stateMachine; // Create new FSM

enum State {INIT, RESET, SETUP_NBIOT, NBIOT_TRANSMISSION}; // State Alias
const char * const stateName[] PROGMEM = {"INIT", "RESET", "SETUP_NB-IOT", "NB-IOT TRANSMISSION"}; // Helper for print labels instead integer when state change

bool resetState = false;
bool setupNBIOTState = false;
bool transmissionState = false;

void setupStateMachine() {
  stateMachine.AddState(stateName[INIT], nullptr, nullptr, onExit);
  stateMachine.AddState(stateName[RESET], resetInterfaces, nullptr, onExit);
  stateMachine.AddState(stateName[SETUP_NBIOT], onEnter, setupNBIoTConnection, onExit);
  stateMachine.AddState(stateName[NBIOT_TRANSMISSION], transmissionSM, nullptr, onExit);

  // bool val at true activate the transition
  stateMachine.AddTransition(RESET, SETUP_NBIOT, setupNBIOTState);    
  stateMachine.AddTransition(SETUP_NBIOT, NBIOT_TRANSMISSION, transmissionState);    
  // stateMachine.AddTransition(NBIOT_TRANSMISSION, MASTER, masterState);
  stateMachine.AddTransition(INIT, RESET, resetState);
}

void setup() {
  pinMode(powerPin, OUTPUT);
  digitalWrite(powerPin, HIGH);

  Serial.begin(BAUD_RATE);
  NBIOTSerial.begin(BAUD_RATE);
  BLESerial.begin(BAUD_RATE);
  delay(2000);

  Serial.println("==== BLE Module started ====");
  Serial.println();
  Serial.println("Starting the Finite State Machine...");
  setupStateMachine();
  //switch from INIT to SETUP
  resetState = true;
}

void loop() {
  readSerial();
  readNBIOT();
  

  if(stateMachine.Update()){
    Serial.print(F("Active state: "));
    Serial.println(stateMachine.ActiveStateName());
  }
}
