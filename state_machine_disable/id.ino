void setupId(){
  UniqueIDdump(Serial);

  if(UniqueID[8] == 25){
    Serial.println(F("Sns B"));
    strcpy(BLENAME, "m0");
    strcpy(MAC_TO_CONNECT, "94A9A83B7B35");
    SLEEP_TIME = 25000;
  }
  if(UniqueID[8] == 22){
    Serial.println(F("Sn N"));
    strcpy(BLENAME, "m1");
    strcpy(MAC_TO_CONNECT, "A06C65CF7F9C");
    SLEEP_TIME = 10000;
  }
}