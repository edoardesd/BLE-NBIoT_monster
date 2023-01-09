#include <TimedAction.h>
//NOTE: This library has an issue on newer versions of Arduino. After
//      downloading the library you MUST go into the library directory and
//      edit TimedAction.h. Within, overwrite WProgram.h with Arduino.h


//GLOBALS

int timerCounter = 0;   // incrementing counter. will crash eventually.
int stringNo = 0;       //which text string to show
//                   "16 CHARACTER MAX"
char* stringArray[]={"Check it out... ",
                     "I have 2 threads",
                     "going at once...",
                     "Cool, huh?! :D  "};
                     
//INIT

// This should probably be done inside setup(), but whatever.
// initialize the LCD library with the numbers of the interface pins
//FUNCTIONS

//this is our first task, print an incrementing number to the LCD
void incrementNumber(){
   // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  // add one to the counter, then display it.
  timerCounter = timerCounter + 1;  
  Serial.println(timerCounter);
}

//our second task, fires every few seconds and rotates text strings
void changeText(){  
  // Print a message to the LCD.
  Serial.println(stringArray[stringNo]);

  //nasty hack to get number of Array elements
  if (stringNo >= sizeof(stringArray)/sizeof(char *)){  
    stringNo = 0;
    changeText();
  }
  else{
    stringNo = stringNo + 1;  
  }
}

//Create a couple timers that will fire repeatedly every x ms

//edit: these lines used to be in front of incrementNumber and changeText
//      functions. this didn't work because the functions weren't defined yet!
TimedAction numberThread = TimedAction(10,incrementNumber);
TimedAction textThread = TimedAction(3000,changeText);

// where's our third task? well, it's the main loop itself :) the task
// which repeats most often should be used as the loop. other
// tasks are able to "interrupt" the fastest repeating task.


void setup() {
  Serial.begin(9600);

  Serial.print("test");
  //define the LCD's number of columns and rows:
  //fire changeText once to paint the initial string [0]
  changeText();
}


void loop() {
  

  //check on our threads. based on how long the system has been
  //running, do they need to fire and do work? if so, do it!
  numberThread.check();
  textThread.check();  

  
}