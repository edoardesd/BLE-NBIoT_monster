#include <SoftwareSerial.h>
#define BLENAME "meter0"
SoftwareSerial BLESerial(8, 9); // RX, TX


void setup() {
  Serial.begin(9600);
  BLESerial.begin(9600);
  //BLESerial.setTimeout(10);

  while (!Serial) {
    Serial.println("No serial");
  }

  Serial.println("==== BLE Module started ====");
  Serial.println();
  setupBlueToothConnection();
}

void loop() { 
  if (BLESerial.available() > 0) {
    String str = BLESerial.readString();
    Serial.println("Reply: " + str);
    Serial.write("\n");

    //Serial.write(BLESerial.read());
  }
  if (Serial.available()) {
    BLESerial.write(Serial.read());
    Serial.write("\n");

  }
}


void setupBlueToothConnection(){
  BLESerial.write("AT");
  delay(200);
 
  BLESerial.write("AT+ROLE0");  // set the role as slave
  delay(200);
  Serial.write("\n");
 
  BLESerial.write(strcat("AT+NAME", BLENAME));  
  //BLESerial.write(strcat("AT+PASS", 12345));  

  delay(200);
 
  BLESerial.write("AT+AUTH1");
  BLESerial.write("AT+NOTI1"); // Enable notifications (e.g. conn/disconn)
  //BLESerial.write("AT+RESET");
  Serial.write("\n");

  delay(200);

  //blue.flush();
}
