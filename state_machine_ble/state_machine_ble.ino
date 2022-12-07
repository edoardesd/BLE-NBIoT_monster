#include <SoftwareSerial.h>
#include <string.h>
#include <YA_FSM.h>  // https://github.com/cotestatnt/YA_FSM

#define NBIOTSerial Serial1
#define powerPin 7 
#define BLENAME "meter0"
#define NUM_SETUPOPERATIONS_NBIoT 5
#define NUM_SETUPOPERATIONS_BLE 11
#define NUM_MASTEROPERATIONS_BLE 3

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
int nbIoToldState = -1;
int oldStateBle = -1;

int ADV_TIME = 60000; 

String setupIoTList[NUM_SETUPOPERATIONS_NBIoT] = {"AT+CMEE=1", "AT+CFUN=1", "AT+CGDCONT=1,\"IP\",\"nb.inetd.gdsp\"", "AT+CEREG=2", "AT+COPS=1,2,\"22210\""};
String setupBLEList[NUM_SETUPOPERATIONS_BLE] = {"AT+IMME0", "AT+ROLE0", "AT+NAMEmeter1", "AT+NOTI1", "AT+NOTP1", "AT+ADTY0", "AT+BAUD0", "AT+ADVI7", "AT+SHOW3", "AT+PWRM1", "AT+RESET"};
String masterBLEList[NUM_MASTEROPERATIONS_BLE] = {"AT+IMME1", "AT+ROLE1", "AT+DISC?"};
String cmd;
String cmd_ble;

char *strremove(char *str, const char *sub);
void readSerial();
void readBLE();
void readNBIOT();

/***** FINITE STATE MACHINE VARS *****/
YA_FSM stateMachine; // Create new FSM

enum State {INIT, RESET, SETUP_BLE, BLE_ADV, BLE_ADV_9, BLE_ADV_D, BLE_MASTER}; // State Alias
const char * const stateName[] PROGMEM = {"INIT", "RESET", "SETUP_BLE", "BLE_ADV", "BLE_ADV_9", "BLE_ADV_D", "BLE_MASTER"}; // Helper for print labels instead integer when state change

bool resetState = false;
bool setupBLEState = false;
bool setupNBIOTState = false;
bool transmissionState = false;
bool bleMasterState = false;
bool bleAdvState = false;
bool bleAdv_9State = false;
bool bleAdv_DState = false;

void setupStateMachine() {
  stateMachine.AddState(stateName[INIT], nullptr, nullptr, onExit);
  stateMachine.AddState(stateName[RESET], resetInterfaces, nullptr, onExit);
  stateMachine.AddState(stateName[SETUP_BLE], onEnter, setupBlueToothConnection, onExit);
  // stateMachine.AddState(stateName[NBIOT_TRANSMISSION], transmissionSM, nullptr, onExit);
  stateMachine.AddState(stateName[BLE_ADV], ADV_TIME, onEnter, nullptr, onExit);
  stateMachine.AddState(stateName[BLE_ADV_9], ADV_TIME, bleAdvertisement_9, nullptr, onExit);
  stateMachine.AddState(stateName[BLE_ADV_D], ADV_TIME, bleAdvertisement_D, nullptr, onExit);
  // stateMachine.AddState(stateName[BLE_ADV], onEnter, bleAdvertisement, onExit);
  stateMachine.AddState(stateName[BLE_MASTER], onEnter, onMaster, onExit);

  // bool val at true activate the transition
  stateMachine.AddTransition(RESET, SETUP_BLE, setupBLEState);
  // stateMachine.AddTransition(SETUP_BLE, SETUP_NBIOT, setupNBIOTState);   
  stateMachine.AddTransition(SETUP_BLE, BLE_ADV, bleAdvState);    
  stateMachine.AddTransition(BLE_ADV, BLE_ADV_9);    
  stateMachine.AddTransition(BLE_ADV_9, BLE_ADV_D);    
  // stateMachine.AddTransition(SETUP_NBIOT, NBIOT_TRANSMISSION, transmissionState);    
  // stateMachine.AddTransition(NBIOT_TRANSMISSION, MASTER, masterState);
  stateMachine.AddTransition(BLE_ADV, BLE_MASTER, bleMasterState);
  // stateMachine.AddTransition(MASTER, NBIOT_TRANSMISSION, transmissionState);
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
  // readNBIOT();
  

  if(stateMachine.Update()){
    Serial.print(F("Active state: "));
    Serial.println(stateMachine.ActiveStateName());
  }
}
