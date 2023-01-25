// 58 37 33 31 30 32 01 0C 16 
// 58 37 33 31 30 32 13 17 19 
// A06C65CF7F9COK
// #define MAIN
#define NONBMODULE

#define NBIOTSerial Serial1
#define powerPin 7
#define NUM_SETUPOPERATIONS_NBIOT 5
#define NUM_SETUPOPERATIONS_BLE 9
#define NUM_MASTEROPERATIONS_BLE 3
#define NUM_DISCONNOPERATIONS_BLE 4


/***** SERIAL READ VARS *****/
#define BAUD_RATE 9600
#define BLUETOOTH_TX 9
#define BLUETOOTH_RX 8
#define READ_TIME 500 //ms

#define CONNECTION_TIME 10000
#define RSRQ_THRESHOLD 250

// #if defined(NONBMODULE)
//   #define SKIP_NBIOT true
// #else
//   #define SKIP_NBIOT false
// #endif

// #if defined(MAIN)
//   // #define BLENAME "m0"
//   // #define SLEEP_TIME 20000
//   #define MAC_TO_CONNECT "A06C65CF7F9C" // N

// #else 
//   // #define BLENAME "m1"
//   // #define SLEEP_TIME 20000
//   #define MAC_TO_CONNECT "94A9A83B7B35" // B
// #endif  
  // char BLENAME[2];
  // int SLEEP_TIME;
bool forceBLE = false;

char BLENAME[3] = "mx";
char MAC_TO_CONNECT[13];
int SLEEP_TIME = 25000;

const PROGMEM char *const FIXED_DATAGRAM = "AT+NSOST=0,\"131.175.120.22\",8883,";
const PROGMEM char *const BLE_TAG = "BLE+";
const PROGMEM char *const NBIOT_TAG = "NB+";
const PROGMEM char *const FORCE_TAG = "NONB";
const PROGMEM char *const CEREG_NBIOT_TAG = "CEREG: 5";
const PROGMEM char *const SUCCESS_NBIOT_TAG = "OK";
const PROGMEM char *const RESET_NBIOT_TAG = "UFOTAS";

const PROGMEM char *const DGRAMcmd = "AT+NSOCR=\"DGRAM\",17,3365,1\r\n"; 
const PROGMEM char *const STATScmd = "AT+NUESTATS\r\n";
const PROGMEM char *const CGATTcmd = "AT+CGATT?\r\n";
unsigned long prevMillis;

SoftwareSerial BLESerial(BLUETOOTH_RX, BLUETOOTH_TX);

String outputNBIOT;
String outputBLE;
uint8_t okNBIOTList = 0;
uint8_t bleOperationIndex = 0;
uint8_t nbIoToldState = -1;
uint8_t oldStateBle = -1;

// int SLEEP_TIME = 5000;

/***** Helpers *****/
uint8_t i = 0;
uint8_t j = 0;

/***** NB-IOT VARS *****/
// char connectCMD[13] = ""; 
char TRANScmd[50] = "";
// char rsrq[5] = "";
char payload[15] = "";
char payloadHex[22];
char forwardPayload[22];
uint8_t idDatagram = 0;
// String stringIdDatagram = "";
// char remainingPayload[5] = "AAAA";
// char buffer[5] = "";
// uint8_t payloadLen = 0;
// uint8_t rsrqInt = 0;

// char mac[13];
bool macFound = false;

const char*  setupIoTList[NUM_SETUPOPERATIONS_NBIOT] = { "AT+CMEE=1", "AT+CFUN=1", "AT+CGDCONT=1,\"IP\",\"nb.inetd.gdsp\"", "AT+CEREG=2", "AT+COPS=1,2,\"22210\"" };
const char* setupBLEList[NUM_SETUPOPERATIONS_BLE] = { "AT+IMME0", "AT+ROLE0", "AT+NAME", "AT+NOTI1", "AT+NOTP1", "AT+ADTY0", "AT+ADVI1", "AT+SHOW3", "AT+RESET"};
const char*  masterBLEList[NUM_MASTEROPERATIONS_BLE] = {"AT+IMME1", "AT+ROLE1", "AT+DISC?" };
const char*  disconnectionBLEList[NUM_DISCONNOPERATIONS_BLE] = { "AT", "AT+IMME0", "AT+ROLE0", "AT+RESET" };

String cmdNBIOT;
String cmd_ble;

char *strremove(char *str, const char *sub);
void readSerial();
void readBLE();
void readNBIOT();

/***** FINITE STATE MACHINE VARS *****/ //TODO class?
YA_FSM stateMachine;  // Create new FSM

bool resetState = false;
bool setupBLEState = false;
bool setupNBIOTState = false;
bool sleepState = false;
bool masterState = false;
bool readyToSendNBIOT = false;
bool connectedState = false;
bool forwardState = false;
bool disconnectedState = false;