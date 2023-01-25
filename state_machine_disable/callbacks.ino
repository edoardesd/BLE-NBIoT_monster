/////////// STATE MACHINE CALLBACK FUNCTIONS /////////////////
void onEnter() {
  Serial.print(F("freeMem="));
  Serial.println(freeMemory());
  // Serial.println(F("------------"));
  Serial.print(F("Ent: "));
  Serial.println(stateMachine.ActiveStateName());
}
// Define "on leaving" callback function (the same for all "light"  states)
void onExit() {
  // Serial.print(F("freeMemory()="));
  // Serial.println(freeMemory());
  // Serial.println(F("------------"));
  Serial.print(F("Ex: "));
  Serial.println(stateMachine.ActiveStateName());
  Serial.println();
}