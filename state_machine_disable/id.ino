void setupId(){
  // UniqueIDdump(Serial);

  if(UniqueID[8] == 25){
    Serial.println(F("Sns B"));
    strlcpy(BLENAME, "m0", sizeof(BLENAME));
    strlcpy(MAC_TO_CONNECT, "94A9A83B7B35", sizeof(strlcpy));
    // getConnectString();
    SLEEP_TIME = 18000;
    forceNBIOT = false;
  }
  if(UniqueID[8] == 22){
    Serial.println(F("Sn N"));
    strlcpy(BLENAME, "m1", sizeof(BLENAME));
    strlcpy(MAC_TO_CONNECT, "A06C65CF7F9C", sizeof(MAC_TO_CONNECT));
    // getConnectString();
    SLEEP_TIME = 20000;
    forceNBIOT = false;
  }
}

// void getConnectString(){
//   strcat(connectCMD, MAC_TO_CONNECT);
//   connectCMD[18] = '\0';
// }