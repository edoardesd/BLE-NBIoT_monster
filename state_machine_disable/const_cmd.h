const PROGMEM char *const FIXED_DATAGRAM = "AT+NSOST=0,\"131.175.120.22\",8883,";
const PROGMEM char *const BLE_TAG = "BLE+";
const PROGMEM char *const NBIOT_TAG = "NB+";
const PROGMEM char *const FORCE_TAG = "NONB";
const PROGMEM char *const RELEASE_TAG = "YESNB";
const PROGMEM char *const CEREG_NBIOT_TAG = "CEREG: 5";
const PROGMEM char *const SUCCESS_NBIOT_TAG = "OK";
const PROGMEM char *const RESET_NBIOT_TAG = "UFOTAS";

const PROGMEM char *const DGRAMcmd = "AT+NSOCR=\"DGRAM\",17,3365,1\r\n"; 
const PROGMEM char *const STATScmd = "AT+NUESTATS\r\n";
const PROGMEM char *const CGATTcmd = "AT+CGATT?\r\n";

const PROGMEM char *const separator = "--------";