#include <Arduino.h>

#if defined(ARDUINO_AVR_LEONARDO)
/* Arduino Leonardo + SODAQ NB-IoT Shield */
#define DEBUG_STREAM Serial 
#define MODEM_STREAM Serial1
#define powerPin 7 
#else
#error "Please use one of the listed boards or add your board."
#endif

const char *NBIOT_TAG = "NB+";
const int READ_TIME = 500; //ms
unsigned long prevMillis;

char *strremove(char *str, const char *sub);

void setup() 
{
    // Put voltage on the nb-iot module
    pinMode(powerPin, OUTPUT);
    digitalWrite(powerPin, HIGH);

  // Start communication
  DEBUG_STREAM.begin(9600);
  MODEM_STREAM.begin(9600);
}

void loop() 
{
  readSerial();

  while (MODEM_STREAM.available())
  {     
    DEBUG_STREAM.write(MODEM_STREAM.read());
  }
}

void readSerial(){
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

  

    if (strstr(str.c_str(), NBIOT_TAG)){
      Serial.print("NB-IoT Stream: ");
      str = strremove(str.c_str(), NBIOT_TAG);
      str = strcat(str.c_str(), "\r\n");
      MODEM_STREAM.write(str.c_str() ); //use strcat and add \r\n
    }
    Serial.println(str);
  }
}

char *strremove(char *str, const char *sub) {
    size_t len = strlen(sub);
    if (len > 0) {
        char *p = str;
        while ((p = strstr(p, sub)) != NULL) {
            memmove(p, p + len, strlen(p + len) + 1);
        }
    }
    return str;
}
