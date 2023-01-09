/////////// STATE MACHINE FUNCTIONS /////////////////
void resetInterfaces() {
  digitalWrite(LED_BUILTIN, HIGH);
  Serial.println("Resetting the interfaces");
  Serial.println("Reset NB-IoT");
  NBIOTSerial.write("ATI9\r\n");
  NBIOTSerial.write("AT+NRB\r\n");
  delay(200);
}

void readStats() {
  NBIOTSerial.write(STATScmd.c_str());
}

void sendDatagram(){
  idDatagram++;
  Serial.println("Ready to transmit datagrams!");
  previousMillis = millis();

}

void waitNextTransmission(){

if (millis() - messagePreviousMillis >= MSG_DELAY) {
  if (msg_counter<NUM_MESSAGES_NBIOT){
    Serial.print("Send message number :");
    Serial.println(msg_counter);
    NBIOTSerial.write(TRANScmd);
    msg_counter++;
    messagePreviousMillis = millis();
  }
}  


  if (millis() - previousMillis >= BURST_MESSAGE_NBIOT) {
    previousMillis = millis();

    Serial.println("Waited enough");
    digitalWrite(LED_BUILTIN, HIGH);
    transState = false;
    statsState = true;
    msg_counter = 0;
    }
  else{
      digitalWrite(LED_BUILTIN, LOW);
    }
}

void setupNBIoTConnection() {
  // if operation is increased
  if (nbIoToldState != okNBIOTList) {
    if (okNBIOTList < NUM_SETUPOPERATIONS) {
      cmd = strcat(setupIoTList[okNBIOTList].c_str(), "\r\n");
      Serial.println(cmd);
      NBIOTSerial.write(cmd.c_str());
    }
    if (okNBIOTList == NUM_SETUPOPERATIONS + 1) {
      Serial.println(DGRAMcmd);
      NBIOTSerial.write(DGRAMcmd.c_str());
    }

    if (okNBIOTList == NUM_SETUPOPERATIONS + 2) {
      Serial.println("Socket done! Change state");
      digitalWrite(LED_BUILTIN, LOW);
      setupNBIOTState = false;
      statsState = true;
    }
    // Serial.print(okNBIOTList);
    // Serial.println(NUM_SETUPOPERATIONS);
  }
  nbIoToldState = okNBIOTList;
}