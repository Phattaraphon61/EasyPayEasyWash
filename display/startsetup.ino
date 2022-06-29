void startsetup() {
  delay(1000);
  Serial.begin(115200);
  arduinocon.begin(57600);
  tft.begin();
  qrcode.init();
  tft.setRotation(1);
  uint16_t calData[5] = { 304, 3621, 284, 3498, 7 };
  tft.setTouch(calData);
  EEPROM.begin(12);
  EEPROM.get(0, check);
  Serial.print("Read check = ");
  Serial.println(check);
  if (check == 1) {
    page = 1;
  }
  setup_wifi();
  EEPROM.end();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  arduinocon.write("coin,0\n");
}
