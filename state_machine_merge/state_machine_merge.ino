#include <SoftwareSerial.h>
#include <string.h>
#include <YA_FSM.h>  // https://github.com/cotestatnt/YA_FSM
#include <MemoryFree.h>


#define NBIOTSerial Serial1
#define powerPin 7
#define BLENAME "meter0"
#define NUM_SETUPOPERATIONS_NBIOT 5
#define NUM_SETUPOPERATIONS_BLE 4
#define NUM_MASTEROPERATIONS_BLE 3
#define NUM_DISCONNOPERATIONS_BLE 6
#define SLEEP_TIME 5000
#define MAC_TO_CONNECT "94A9A83B7B35"
#define CONNECTION_TIME 10000
#define RSRQ_THRESHOLD 19


const char *BLE_TAG = "BLE+";
const char *NBIOT_TAG = "NB+";
const char *CEREG_NBIOT_TAG = "CEREG: 5";
const char *SUCCESS_NBIOT_TAG = "OK";
const char *RESET_NBIOT_TAG = "UFOTAS";
/***** SERIAL READ VARS *****/
unsigned long BAUD_RATE = 9600;
const int BLUETOOTH_TX = 9;
const int BLUETOOTH_RX = 8;
const int READ_TIME = 500;  //ms

unsigned long prevMillis;

SoftwareSerial BLESerial(BLUETOOTH_RX, BLUETOOTH_TX);

String outputNBIOT;
String outputBLE;
char messageBLE[] = "00000000000000000000";
uint8_t okNBIOTList = 0;
uint8_t bleOperationIndex = 0;
uint8_t nbIoToldState = -1;
uint8_t oldStateBle = -1;

// int SLEEP_TIME = 5000;
uint8_t bleTransmissions = 0;

/***** NB-IOT VARS *****/
char DGRAMcmd[35] = "AT+NSOCR=\"DGRAM\",17,3365,1\r\n";
char STATScmd[15] = "AT+NUESTATS\r\n";
char CGATTcmd[13] = "AT+CGATT?\r\n";
char TRANScmd[50] = "";
char rsrq[5] = "";
char payload[20] = "";
char payloadHex[20];
uint8_t idDatagram = 0;
String stringIdDatagram = "";
char remainingPayload[4] = "AAAA";
char buffer[5] = "";
uint8_t payloadLen = 0;
int rsrqInt = 0;

char mac[13];
bool macFound = false;

String setupIoTList[NUM_SETUPOPERATIONS_NBIOT] = { "AT+CMEE=1", "AT+CFUN=1", "AT+CGDCONT=1,\"IP\",\"nb.inetd.gdsp\"", "AT+CEREG=2", "AT+COPS=1,2,\"22210\"" };
String setupBLEList[NUM_SETUPOPERATIONS_BLE] = { "AT+IMME0", "AT+ROLE0", "AT+NAMEmeter1", "AT+RESET"};
String masterBLEList[NUM_MASTEROPERATIONS_BLE] = {"AT+IMME1", "AT+ROLE1", "AT+DISC?" };
String disconnectionBLEList[NUM_DISCONNOPERATIONS_BLE] = { "AT", "AT+IMME0", "AT+ROLE0", "AT+NAMEmeter1", "AT+RESET" };

String cmdNBIOT;
String cmd_ble;

char *strremove(char *str, const char *sub);
void readSerial();
void readBLE();
void readNBIOT();

/***** FINITE STATE MACHINE VARS *****/
YA_FSM stateMachine;  // Create new FSM

bool resetState = false;
bool setupBLEState = false;
bool setupNBIOTState = false;
bool sleepState = false;
bool masterState = false;
bool readyToSendNBIOT = false;
bool connectedState = false;

enum State { INIT,
             RESET,
             SETUP_BLE,
             SETUP_NBIOT,
             SLEEP,
             WAKEUP,
             BLE_MASTER,
             BLE_CONNECTED,
             //BLE_DISCONNECTION
             };                                                                                                         // State Alias
const char *const stateName[] PROGMEM = { "INT",
                                          "RST",
                                          "S_BLE",
                                          "S_NB",
                                          "SLP",
                                          "WAKE",
                                          "BLE_M",
                                          "BLE_C",
                                          //"BLE_DISCONNECTION"
                                          };  // Helper for print labels instead integer when state change


void setupStateMachine() {
  stateMachine.AddState(stateName[INIT], nullptr, nullptr, onExit);
  stateMachine.AddState(stateName[RESET], resetInterfaces, nullptr, onExit);
  stateMachine.AddState(stateName[SETUP_BLE], onEnter, setupBlueToothConnection, onExit);
  stateMachine.AddState(stateName[SETUP_NBIOT], onEnter, setupNBIoTConnection, onExit);
  stateMachine.AddState(stateName[SLEEP], SLEEP_TIME, onSleep, nullptr, onExit);
  stateMachine.AddState(stateName[WAKEUP], onWakeUp, sendNBIOT, onExit);
  stateMachine.AddState(stateName[BLE_MASTER], onEnter, onMaster, onExit);
  stateMachine.AddState(stateName[BLE_CONNECTED], bleConnected, nullptr, onExit);


  // bool val at true activate the transition
  stateMachine.AddTransition(RESET, SETUP_BLE, setupBLEState);
  stateMachine.AddTransition(SETUP_BLE, SETUP_NBIOT, setupNBIOTState);
  stateMachine.AddTransition(SETUP_NBIOT, SLEEP, sleepState);
  stateMachine.AddTransition(WAKEUP, SLEEP, sleepState);
  stateMachine.AddTransition(WAKEUP, BLE_MASTER, masterState);
  stateMachine.AddTransition(BLE_MASTER, BLE_CONNECTED, connectedState);
  stateMachine.AddTransition(INIT, RESET, resetState);

  stateMachine.AddTimedTransition(SLEEP, WAKEUP);
}

void setup() {
  pinMode(powerPin, OUTPUT);
  digitalWrite(powerPin, HIGH);
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(BAUD_RATE);
  NBIOTSerial.begin(BAUD_RATE);
  BLESerial.begin(BAUD_RATE);
  delay(2000);

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