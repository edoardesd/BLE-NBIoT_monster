/////////// STATE MACHINE FUNCTIONS /////////////////
void resetInterfaces() {
  Serial.println("Resetting the interfaces");
  Serial.println("Reset NB-IoT");
  NBIOTSerial.write("ATI9\r\n");
  NBIOTSerial.write("AT+NRB\r\n");
  delay(200);
}

void transmissionSM() {
  Serial.println("Ready to transmit datagrams!");

  Serial.write("AT+NUESTATS");
  NBIOTSerial.write("AT+NUESTATS\r\n");
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
      Serial.println("AT+NSOCR=\"DGRAM\",17,3365,1\r\n");
      NBIOTSerial.write("AT+NSOCR=\"DGRAM\",17,3365,1\r\n");
    }

    if (okNBIOTList == NUM_SETUPOPERATIONS + 2) {
      Serial.println("Socket done! Change state");
      setupNBIOTState = false;
      transmissionState = true;
    }
    // Serial.print(okNBIOTList);
    // Serial.println(NUM_SETUPOPERATIONS);
  }
  nbIoToldState = okNBIOTList;
}

// if (okNBIOTList > NUM_SETUPOPERATIONS) {
//       if (okNBIOTList > NUM_SETUPOPERATIONS + 1) {
//         Serial.println("Setup done!");

//         setupNBIOTState = false;
//         // transmissionState = true;
//         Serial.println(transmissionState);
//       }
//       else {
//         Serial.println("AT+NSOCR=\"DGRAM\",17,3365,1\r\n");
//         NBIOTSerial.write("AT+NSOCR=\"DGRAM\",17,3365,1\r\n");
//       }
//     } else {
//       cmd = strcat(setupIoTList[okNBIOTList].c_str(), "\r\n");
//       Serial.println(cmd);
//       NBIOTSerial.write(cmd.c_str());
//     }
//   }