#include <SoftwareSerial.h>   //Software Serial Port

/*Connect your Bluetooth Module to D8 otherwise it will not work due to Arduino Leonardo */

#define RxD         8
#define TxD         9
 
SoftwareSerial blue(RxD,TxD);

void setup()
{
    Serial.begin(9600);
    blue.begin(9600); //anything other than 9600 will not work
   
    // wait for serial port to connect. Needed for native USB
    while(!Serial){
    ;
    }
   
    pinMode(RxD, INPUT);
    pinMode(TxD, OUTPUT);
   
    Serial.print("Started\n");
    setupBlueToothConnection();
   
}

void loop() {

        if(blue.available())
        {//check if there's any data sent from the remote bluetooth shield
            Serial.write( blue.read());
           
        }
        if(Serial.available())
        {//check if there's any data sent from t he local serial terminal, you can add the other applications here
            blue.write(Serial.read());

        }

}

void setupBlueToothConnection()
{
  //blue.begin(9600);
 
  blue.print("AT");
  delay(400);
 
  blue.print("AT+ROLE0");  // set the role as slave
  delay(400);
 
  blue.print("AT+NAMElol");   // set the bluetooth name as "Slave" ,the length of bluetooth name must less than 12 characters.
  delay(400);
 
}