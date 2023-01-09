#include <SoftwareSerial.h>

#define BLENAME "meter0"
#define NUM_SETUPOPERATIONS_BLE 10
#define NUM_MASTEROPERATIONS_BLE 2

const char *BLE_TAG = "BLE+";
unsigned long BAUD_RATE = 9600;

const int BLUETOOTH_TX = 9;
const int BLUETOOTH_RX = 8;

const int READ_TIME = 500; //ms

unsigned long prevMillis;

String outputBLE;
int okNBIOTList = 0;
int bleOperationIndex = 0;

SoftwareSerial BLESerial(BLUETOOTH_RX, BLUETOOTH_TX);
String setupBLEList[NUM_SETUPOPERATIONS_BLE] = {"AT+IMME0", "AT+ROLE0", "AT+NAMEmeter1", "AT+NOTI1", "AT+NOTP1", "AT+ADTY0", "AT+BAUD0", "AT+ADVID", "AT+SHOW3", "AT+RESET"};

void checkOkBLE(){
    if(strstr(outputBLE.c_str(), "OK+Set")){
      Serial.println("Operation ble done, go with the next one");
      bleOperationIndex++;
    }
}

void checkResetBLE(){
  if(strstr(outputBLE.c_str(), "OK+RESET")) {
    Serial.println("BLE setup done.");
    BLESerial.write("AT+NAME?");

  }
}

void readBLE(){
  if (BLESerial.available()) {
    Serial.print("HM10: ");
    
    outputBLE = BLESerial.readStringUntil('\n');
    Serial.println(outputBLE);
    checkOkBLE();
    checkResetBLE();
  }  
}
void setup() {
  // Start serial communications.
  // The baud rate must be the same for both the serial and the bluetooth.
  Serial.begin(BAUD_RATE);
  BLESerial.begin(BAUD_RATE);

  Serial.println("==== BLE Module started ====");
  Serial.println();
  setupBlueToothConnection();
  Serial.println("Setup Complete");
}

void loop() {
    readBLE();
}

void setupBlueToothConnection(){
  BLESerial.write("AT");
  delay(200);
  BLESerial.write("AT+RENEW");
  delay(400);
}
