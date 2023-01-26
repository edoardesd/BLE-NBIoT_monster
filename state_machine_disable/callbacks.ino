/////////// STATE MACHINE CALLBACK FUNCTIONS /////////////////
void onEnter() {
  Serial.print(F("freeMem="));
  Serial.println(freeMemory());
  Serial.println(separator);
  Serial.print(F("Ent: "));
  Serial.println(stateMachine.ActiveStateName());
}
void onExit() {
  Serial.print(F("Ex: "));
  Serial.println(stateMachine.ActiveStateName());
  Serial.println();
}