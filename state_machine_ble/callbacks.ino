/////////// STATE MACHINE CALLBACK FUNCTIONS /////////////////


// void onMaster() {
//   Serial.println("Doing the master");
//   delay(5000);
//   masterState = false;
//   bleAdvState = true;
// }

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