
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
