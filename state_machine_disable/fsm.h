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
bool disconnectedState = false;
bool isLostConn = false; 

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
             };                                                                                                     
const char *const stateName[] PROGMEM = { "INT",
                                          "RST",
                                          "BLE_S",
                                          "NB_S",
                                          "SLP",
                                          "WAKE",
                                          "BLE_M",
                                          "BLE_C",
                                          "BLE_DISC",
                                          "NB_FWD"
                                          };  // Helper for print labels instead integer when state change
