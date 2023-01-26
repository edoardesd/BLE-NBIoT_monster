// #define MAIN
#define NONBMODULE

#define NUM_SETUPOPERATIONS_NBIOT 5
#define NUM_SETUPOPERATIONS_BLE 9
#define NUM_MASTEROPERATIONS_BLE 3
#define NUM_DISCONNOPERATIONS_BLE 3
#define CONNECTION_TIME 10000
#define RSRQ_THRESHOLD 250

bool forceBLE = false;

int SLEEP_TIME = 25000;

String outputNBIOT;
String outputBLE;
uint8_t okNBIOTList = 0;
uint8_t bleOperationIndex = 0;
uint8_t nbIoToldState = -1;
uint8_t oldStateBle = -1;


/***** Helpers *****/
uint8_t i = 0;
uint8_t j = 0;

/*****  *****/ 
char BLENAME[3] = "mx";
char TRANScmd[50] = "";
char payload[15] = "";
char payloadHex[22];
char forwardPayload[22];
char MAC_TO_CONNECT[13];
uint8_t idDatagram = 0;
bool mac_found = false;

const char*  setupIoTList[NUM_SETUPOPERATIONS_NBIOT] = { "AT+CMEE=1", "AT+CFUN=1", "AT+CGDCONT=1,\"IP\",\"nb.inetd.gdsp\"", "AT+CEREG=2", "AT+COPS=1,2,\"22210\"" };
const char* setupBLEList[NUM_SETUPOPERATIONS_BLE] = { "AT+IMME0", "AT+ROLE0", "AT+NAME", "AT+NOTI1", "AT+NOTP1", "AT+ADTY0", "AT+ADVI1", "AT+SHOW3", "AT+RESET"};
const char*  masterBLEList[NUM_MASTEROPERATIONS_BLE] = {"AT+IMME1", "AT+ROLE1", "AT+DISC?" };
const char*  disconnectionBLEList[NUM_DISCONNOPERATIONS_BLE] = {"AT+IMME0", "AT+ROLE0", "AT+RESET"};

String cmdNBIOT;
String cmd_ble;

// char *strremove(char *str, const char *sub);
// void readSerial();
// void readBLE();
// void readNBIOT();

