/***** SERIAL READ VARS *****/
#define NBIOTSerial Serial1
#define powerPin 7

#define BAUD_RATE 9600
#define BLUETOOTH_TX 9
#define BLUETOOTH_RX 8
#define READ_TIME 500 //ms

SoftwareSerial BLESerial(BLUETOOTH_RX, BLUETOOTH_TX);

unsigned long prevMillis;
