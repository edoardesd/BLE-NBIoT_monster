#include <SoftwareSerial.h>
#include <string.h>
#include <YA_FSM.h>  // https://github.com/cotestatnt/YA_FSM
#include <MemoryFree.h>

// #define MAIN

#define NBIOTSerial Serial1
#define powerPin 7
#define NUM_SETUPOPERATIONS_NBIOT 5
#define NUM_SETUPOPERATIONS_BLE 4
#define NUM_MASTEROPERATIONS_BLE 3
#define NUM_DISCONNOPERATIONS_BLE 6
#define MAC_TO_CONNECT "94A9A83B7B35"
#define CONNECTION_TIME 10000

#if defined(MAIN)
  #define BLENAME "m0"
  #define RSRQ_THRESHOLD 19
  #define SLEEP_TIME 5000
#else 
  #define BLENAME "m1"
  #define RSRQ_THRESHOLD 250
  #define SLEEP_TIME 20000
#endif  



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
uint8_t okNBIOTList = 0;
uint8_t bleOperationIndex = 0;
uint8_t nbIoToldState = -1;
uint8_t oldStateBle = -1;

// int SLEEP_TIME = 5000;
uint8_t bleTransmissions = 0;

/***** Helpers *****/
uint8_t i = 0;
uint8_t j = 0;

/***** NB-IOT VARS *****/
char DGRAMcmd[35] = "AT+NSOCR=\"DGRAM\",17,3365,1\r\n";
char STATScmd[15] = "AT+NUESTATS\r\n";
char CGATTcmd[13] = "AT+CGATT?\r\n";
char connectCMD[13] = ""; 
char TRANScmd[50] = "";
char rsrq[5] = "";
char payload[20] = "";
char payloadHex[25];
char forwardPayload[25];
uint8_t idDatagram = 0;
String stringIdDatagram = "";
char remainingPayload[4] = "AAAA";
char buffer[5] = "";
uint8_t payloadLen = 0;
int rsrqInt = 0;

char mac[13];
bool macFound = false;

String setupIoTList[NUM_SETUPOPERATIONS_NBIOT] = { "AT+CMEE=1", "AT+CFUN=1", "AT+CGDCONT=1,\"IP\",\"nb.inetd.gdsp\"", "AT+CEREG=2", "AT+COPS=1,2,\"22210\"" };
String setupBLEList[NUM_SETUPOPERATIONS_BLE] = { "AT+IMME0", "AT+ROLE0", "AT+NAME", "AT+RESET"};
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
bool forwardState = false;

enum State { INIT,
             RESET,
             SETUP_BLE,
             SETUP_NBIOT,
             SLEEP,
             WAKEUP,
             BLE_MASTER,
             BLE_CONNECTED,
             BLE_DISCONNECT,
             NBIOT_FORWARD
             };                                                                                                         // State Alias
const char *const stateName[] PROGMEM = { "INT",
                                          "RST",
                                          "S_BLE",
                                          "S_NB",
                                          "SLP",
                                          "WAKE",
                                          "BLE_M",
                                          "BLE_C",
                                          "BLE_DISC",
                                          "NB_FWD"
                                          };  // Helper for print labels instead integer when state change


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