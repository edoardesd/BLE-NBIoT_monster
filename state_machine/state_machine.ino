#include <SoftwareSerial.h>
#include <YA_FSM.h>  // https://github.com/cotestatnt/YA_FSM

#define BLENAME "meter0"

/***** SERIAL READ VARS *****/
unsigned long BAUD_RATE = 9600;
const int BLUETOOTH_TX = 9;
const int BLUETOOTH_RX = 8;
const int READ_TIME = 500; //ms

unsigned long prevMillis;

SoftwareSerial BLESerial(BLUETOOTH_RX, BLUETOOTH_TX);

/***** FINITE STATE MACHINE VARS *****/
YA_FSM stateMachine; // Create new FSM

// State Alias
enum State {INIT, SETUP, SLAVE, MASTER};

// Helper for print labels instead integer when state change
const char * const stateName[] PROGMEM = {"INIT", "SETUP", "SLAVE", "MASTER"};

// Output variables
bool setupState = false;
bool slaveState = false;
bool masterState = false;



/////////// STATE MACHINE CALLBACK FUNCTIONS //////////////////
void onSetup() {
  delay(2000);
  setupState = false;
  slaveState = true;
}

void setupBlueToothConnection(){
  Serial.println("Doing setup operation");

  Serial.println("AT");
  BLESerial.write("AT");
  delay(1000);
  Serial.println("AT+RENEW");
  BLESerial.write("AT+RENEW");
  delay(400);
  Serial.println("AT+ROLE0");
  BLESerial.write("AT+ROLE0");  // set the role as slave
  delay(2000);
  
  Serial.println(strcat("AT+NAME", BLENAME));
  BLESerial.write(strcat("AT+NAME", BLENAME));  
  //BLESerial.write(strcat("AT+PASS", 12345));  

  delay(2000);
 
  BLESerial.write("AT+AUTH1"); //1 auto , 0 not auto?
  delay(200);
  BLESerial.write("AT+NOTI1"); // Enable notifications (e.g. conn/disconn)
  delay(200);
  BLESerial.write("AT+NOTP1"); // Show also the address of the notification
  delay(200);
  BLESerial.write("AT+ADTY0"); // 0 connectable advertising scanresponse // 1 and 2 not useful for us // 3 only advertising
  delay(200);
  BLESerial.write("AT+BAUD0"); // 0 = 9600 baudAT+CHAR?
  delay(200);
  BLESerial.write("AT+IMME0"); // 1 = Only on. Until AT + START, AT+CON, AT+CONNL commands is received. // 0: on and active to work -> IMME1 when needs to become central mode
  delay(200);
  BLESerial.write("AT+ADVI1"); // speed of the transmission of the advertisements, 0 = 100ms F= 9000 ms
  //BLSSerial.write("AT+FLAG0"); // allow advertisements, should work even without the command
  delay(200);
  BLESerial.write("AT+SHOW3"); // in discovery: 1 only name, 2 only rssi, 3 both (3 is too much sometimes)
  delay(200);
  BLESerial.write("AT+RESET");

  delay(200);

  BLESerial.write("AT+NAME?");
  
  setupState = false;
  slaveState = true;
}

void onSlave() {
  Serial.println("Doing the slave");
  delay(5000);
  slaveState = false;
  masterState = true;
}

void onMaster() {
  Serial.println("Doing the master");
  delay(5000);
  masterState = false;
  slaveState = true;
}

void onEnter() {
  Serial.println("------------");
  Serial.print(F("Enter in: "));
  Serial.println(stateMachine.ActiveStateName());
}
// Define "on leaving" callback function (the same for all "light"  states)
void onExit() {
  Serial.print(F("Exit from: "));
  Serial.println(stateMachine.ActiveStateName());
  Serial.println();
}

void setupStateMachine() {
  stateMachine.AddState(stateName[INIT], nullptr, nullptr, onExit);
  stateMachine.AddState(stateName[SETUP], onEnter, setupBlueToothConnection, onExit);
  stateMachine.AddState(stateName[SLAVE], onEnter, onSlave, onExit);
  stateMachine.AddState(stateName[MASTER], onEnter, onMaster, onExit);

  // bool val at true activate the transition
  stateMachine.AddTransition(SETUP, SLAVE, slaveState);    
  stateMachine.AddTransition(SLAVE, MASTER, masterState);
  stateMachine.AddTransition(MASTER, SLAVE, slaveState);
  stateMachine.AddTransition(INIT, SETUP, setupState);
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
    BLESerial.print(str);
    Serial.println(str);
  }
}

void readBLE(){
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

void setup() {
  Serial.begin(BAUD_RATE);
  BLESerial.begin(BAUD_RATE);

  Serial.println("==== BLE Module started ====");
  Serial.println();
  Serial.println("Starting the Finite State Machine...");
  setupStateMachine();
  //switch from INIT to SETUP
  setupState = true;
}

void loop() {
  readSerial();
  readBLE();  

  if(stateMachine.Update()){
    Serial.print(F("Active state: "));
    Serial.println(stateMachine.ActiveStateName());
  }
}