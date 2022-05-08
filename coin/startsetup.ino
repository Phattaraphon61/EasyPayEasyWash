void startsetup() {
  delay(1000);
  Serial.begin(9600);
  esp8266con.begin(57600);
  int checkcoin;
  EEPROM.begin(12);
  EEPROM.get(0, checkcoin);
  Serial.print("Read checkcoin = ");
  Serial.println(checkcoin);
  if (100 < checkcoin || checkcoin < 10 ) {
    coin = 20;
  } else {
    coin = checkcoin;
  }
  EEPROM.end();
  pinMode(sensor, INPUT);
  pinMode(ldr, INPUT);
  pinMode(Relay1, OUTPUT); // กำหนดขาทำหน้าที่ให้ขา D0 เป็น OUTPUT
  digitalWrite(Relay1, HIGH);
  pinMode(Relay2, OUTPUT); // กำหนดขาทำหน้าที่ให้ขา D0 เป็น OUTPUT
  digitalWrite(Relay2, HIGH);
  attachInterrupt(digitalPinToInterrupt(sensor), doCounter, RISING);
}
