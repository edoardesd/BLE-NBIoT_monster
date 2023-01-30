void setupStateMachine() {
  stateMachine.AddState(stateName[INIT], nullptr, nullptr, onExit);
  stateMachine.AddState(stateName[RESET], resetInterfaces, nullptr, onExit);
  stateMachine.AddState(stateName[SETUP_BLE], onEnter, setupBlueToothConnection, onExit);
  stateMachine.AddState(stateName[SETUP_NBIOT], onEnter, setupNBIoTConnection, onExit);
  stateMachine.AddState(stateName[SLEEP], SLEEP_TIME, onSleep, nullptr, onExit);
  stateMachine.AddState(stateName[WAKEUP], onWakeUp, sendNBIOT, onExit);
  stateMachine.AddState(stateName[BLE_MASTER], onEnter, onMaster, onExit);
  stateMachine.AddState(stateName[BLE_CONNECTED], CONNECTION_TIME, bleConnected, nullptr, onExit);
  stateMachine.AddState(stateName[BLE_DISCONNECT], bleDisc, bleDiscLoop, onExit);
  stateMachine.AddState(stateName[NBIOT_FORWARD], forwardNBIOT, sendNBIOT, onExit);


  // bool val at true activate the transition
  stateMachine.AddTransition(INIT, RESET, resetState);
  stateMachine.AddTransition(RESET, SETUP_BLE, setupBLEState);
  stateMachine.AddTransition(SETUP_BLE, SETUP_NBIOT, setupNBIOTState);
  stateMachine.AddTransition(SETUP_NBIOT, SLEEP, sleepState);
  stateMachine.AddTransition(WAKEUP, SLEEP, sleepState);
  stateMachine.AddTransition(WAKEUP, NBIOT_FORWARD, forwardState);
  stateMachine.AddTransition(NBIOT_FORWARD, SLEEP, sleepState);
  stateMachine.AddTransition(WAKEUP, BLE_MASTER, masterState);
  stateMachine.AddTransition(BLE_MASTER, BLE_CONNECTED, connectedState);
  stateMachine.AddTransition(BLE_DISCONNECT, SLEEP, sleepState);
  stateMachine.AddTransition(BLE_MASTER, BLE_DISCONNECT, disconnectedState);
  // stateMachine.AddTransition(BLE_CONNECTED, BLE_DISCONNECT, disconnectedState);
  stateMachine.AddTimedTransition(SLEEP, WAKEUP);
  stateMachine.AddTimedTransition(BLE_CONNECTED, BLE_DISCONNECT);
}