void callback(char* topic, byte* payload, unsigned int length) {
  static String msgdata = "";
  //  Serial.print("Message arrived [");
  //  Serial.print(topic);
  //  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    //    Serial.print((char)payload[i]);
    msgdata += (char)payload[i];
  }
  Serial.println();
  Serial.println((char)payload[0]);
  //ดึงค่าแรก (index 0) ออกจาก String msg เก็บไว้บน value_1
  String value_1 = getValue(msgdata, ',', 0);
  //ดึงค่าแรก (index 1) ออกจาก String msg เก็บไว้บน value_2
  String value_2 = getValue(msgdata, ',', 1);
  Serial.println(value_1);
  if (value_1 == "qr") {
    qr = value_2;
    if (page == 1) {
      page = 3;
      page1 = 0;
      page2 = 0;
      page3 = 0;
    }
  }
  if (value_1 == "op") {
    arduinocon.write("op,0\n");
    Serial.println("OPEN");
  }
  if (value_1 == "dis") {
    WiFi.disconnect();
    Serial.println("disconnect");
  }
  if (value_1 == "setcoin") {
    String setcoin = "setcoin," + value_2 + "\n";
    writeString(setcoin);
    //    Serial.println("setcoin");
    //    EEPROM.begin(12);
    //    EEPROM.put(0, value_2.toInt());
    //    EEPROM.commit();
    //    EEPROM.end();
    //        coin =  value_2.toInt();
  }
  if (value_1 == "login") {
    Serial.println("login");
    EEPROM.begin(12);
    EEPROM.put(0, value_2.toInt());
    EEPROM.commit();
    EEPROM.end();
    check =  value_2.toInt();
    if (value_2.toInt() == 1) {
      page0 = 0;
      page = 1;
    } else {
      page = 0;
      page1 = 0;
      page2 = 0;
      page3 = 0;
    }
  }
  msgdata = "";

  // Switch on the LED if an 1 was received as first character

}
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      snprintf (datas, 75, "EPEW%ld", ESP.getChipId());
      client.publish("outTopic", datas);
      // ... and resubscribe
      client.subscribe(datas);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void writeString(String stringData) {
  for (int i = 0; i < stringData.length(); i++) {
    arduinocon.write(stringData[i]);
  }
}
