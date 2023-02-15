
// #define NONBMODULE

#define NUM_SETUPOPERATIONS_NBIOT 5
#define NUM_SETUPOPERATIONS_BLE 9
#define NUM_MASTEROPERATIONS_BLE 3
#define NUM_DISCONNOPERATIONS_BLE 3
#define CONNECTION_TIME 3000
#define RSRQ_THRESHOLD 250
const int buttonPin = 2;  
const int ledPin = 12;   

bool forceNBIOT = false;
bool oldForceNBIOT = false;

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
char mac[13];
char BLENAME[3];
char TRANScmd[50] = "";
char payload[15] = "";
char payloadHex[22];
char forwardPayload[22];
char MAC_TO_CONNECT[13];
uint8_t idDatagram = 0;
uint8_t totalTransmissions = 31;
char current_rsrq[4] = "180";
char connectTAG[7] = "AT+CON";
char connectCMD[19] = "";


const char*  setupIoTList[NUM_SETUPOPERATIONS_NBIOT] = { "AT+CMEE=1\r\n", "AT+CFUN=1\r\n", "AT+CGDCONT=1,\"IP\",\"nb.inetd.gdsp\"\r\n", "AT+CEREG=2\r\n", "AT+COPS=1,2,\"22210\"\r\n" };
char* setupBLEList[NUM_SETUPOPERATIONS_BLE] = {"AT+IMME0", "AT+ROLE0", "AT+NOTI1", "AT+NOTP1", "AT+ADTY0", "AT+ADVI1", "AT+NAMEmx-150-9", "AT+SHOW3", "AT+RESET"};
const char*  masterBLEList[NUM_MASTEROPERATIONS_BLE] = {"AT+IMME1", "AT+ROLE1", "AT+DISC?" };
const char*  disconnectionBLEList[NUM_DISCONNOPERATIONS_BLE] = {"AT+IMME0", "AT+ROLE0", "AT+RESET"};

// String cmdNBIOT;
// String cmd_ble;

