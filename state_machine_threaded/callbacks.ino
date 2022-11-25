/////////// STATE MACHINE CALLBACK FUNCTIONS /////////////////
void onSlave() {
  Serial.println("Doing the slave");
  // delay(5000);
  // slaveState = false;
  // masterState = true;
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