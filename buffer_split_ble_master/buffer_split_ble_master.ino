#include <SoftwareSerial.h>

#define BLENAME "meterMASTER"

unsigned long BAUD_RATE = 9600;

const int BLUETOOTH_TX = 9;
const int BLUETOOTH_RX = 8;

const int READ_TIME = 500; //ms

unsigned long prevMillis;

SoftwareSerial BLESerial(BLUETOOTH_RX, BLUETOOTH_TX);

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
  // put your main code here, to run repeatedly:
  if (Serial.available()) {
    String str = "";
    Serial.print("Input: ");
    
    prevMillis = millis();
    while (millis() - prevMillis < READ_TIME) {
      if (Serial.available()) {
        char c = Serial.read();
        if (c != 10 && c != 13) { // Don't send line end characters to HM10.
          str += c;
        }
      }
    }
    
    BLESerial.print(str);
    Serial.println(str);
  }

  if (BLESerial.available()) {
    String str = "";
    Serial.print("HM10: ");
    
    prevMillis = millis();
    while (millis() - prevMillis < READ_TIME) {
      if (BLESerial.available()) {
        str += (char) BLESerial.read();
      }
    }
    Serial.println(str);
  }
}

void setupBlueToothConnection(){
  BLESerial.write("AT");
  delay(100);
  BLESerial.write("AT+RENEW");
  delay(200);
  BLESerial.write("AT+IMME1");
  delay(200);
  BLESerial.write("AT+ROLE1");  // set the role as MASTER
  delay(200);
  // BLESerial.write(strcat("AT+NAME", BLENAME));  
  // delay(200);
  BLESerial.write("AT+NOTI1");  // Enable notifications (e.g. conn/disconn)
  delay(200);
  BLESerial.write("AT+SHOW3");
  delay(200);
  BLESerial.write("AT+NOTP1");  
  delay(200);
  // BLESerial.write("AT+BAUD0"); // 0 = 9600 baudAT+CHAR?
  // delay(200);


  BLESerial.write("AT+RESET");
  delay(400);
  BLESerial.write("AT+DISC?");

  // BLESerial.write("AT+NAME?");

}
