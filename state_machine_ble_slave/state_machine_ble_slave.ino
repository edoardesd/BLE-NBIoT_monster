#include <SoftwareSerial.h>
#include <string.h>
#include <YA_FSM.h>  // https://github.com/cotestatnt/YA_FSM

#define NBIOTSerial Serial1
#define powerPin 7 
#define BLENAME "meter0"
#define NUM_SETUPOPERATIONS_NBIoT 5
#define NUM_SETUPOPERATIONS_BLE 4
#define NUM_MASTEROPERATIONS_BLE 2

const int ledPin = 11;   
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
String outputBLE;
int okNBIOTList = 0;
int bleOperationIndex = 0;
int oldStateBle = -1;
String setupBLEList[NUM_SETUPOPERATIONS_BLE] = {"AT+IMME0", "AT+ROLE0", "AT+NAMEm3-138-1", "AT+RESET"};
String masterBLEList[NUM_MASTEROPERATIONS_BLE] = {"AT+IMME1", "AT+ROLE1"};
String cmd_ble;

char *strremove(char *str, const char *sub);
void readSerial();
void readBLE();
void readNBIOT();

/***** FINITE STATE MACHINE VARS *****/
YA_FSM stateMachine; // Create new FSM

enum State {INIT, RESET, SETUP_BLE, BLE_ADV}; // State Alias
const char * const stateName[] PROGMEM = {"INIT", "RESET", "SETUP_BLE", "BLE_ADV"}; // Helper for print labels instead integer when state change

bool resetState = false;
bool setupBLEState = false;
bool setupNBIOTState = false;
bool transmissionState = false;
bool bleMasterState = false;
bool bleAdvState = false;

bool isReceived = true;

void setupStateMachine() {
  stateMachine.AddState(stateName[INIT], nullptr, nullptr, onExit);
  stateMachine.AddState(stateName[RESET], resetInterfaces, nullptr, onExit);
  stateMachine.AddState(stateName[SETUP_BLE], onEnter, setupBlueToothConnection, onExit);
  stateMachine.AddState(stateName[BLE_ADV], onEnter, bleAdvertisement, onExit);

  // bool val at true activate the transition
  stateMachine.AddTransition(RESET, SETUP_BLE, setupBLEState); 
  stateMachine.AddTransition(SETUP_BLE, BLE_ADV, bleAdvState);    
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
  readBLE();
  
  if(isReceived) {
    digitalWrite(ledPin, HIGH);
  } else {
    digitalWrite(ledPin, LOW);
  }

  if(stateMachine.Update()){
    Serial.print(F("Active state: "));
    Serial.println(stateMachine.ActiveStateName());
  }
}
