void startsetup() {
  delay(1000);
  Serial.begin(9600);
  esp8266con.begin(57600);
  EEPROM.begin(12);
  int readId;
  EEPROM.get(0, readId);
  Serial.print("Read Id = ");
  Serial.println(readId);
  if (100 < readId || readId < 10 ) {
    coin = 20;
  } else {
    coin = readId;
  }
  EEPROM.end();
  espid += String(ESP.getChipId());
  setup_wifi();
  Serial.println(espid);
  pinMode(sensor, INPUT);
  pinMode(ldr, INPUT);
  pinMode(Relay1, OUTPUT); // กำหนดขาทำหน้าที่ให้ขา D0 เป็น OUTPUT
  digitalWrite(Relay1, HIGH);
  pinMode(Relay2, OUTPUT); // กำหนดขาทำหน้าที่ให้ขา D0 เป็น OUTPUT
  digitalWrite(Relay2, HIGH);
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  //  attachInterrupt(0, doCounter, FALLING);
  //  pinMode(led, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(sensor), doCounter, RISING);
  }
