void start_machine() {
  esp8266con.write("change,0\n");
  delay(1000);
  digitalWrite(Relay1, LOW); // ส่งให้ไฟติด
  Serial.println("LOW");
  delay(1000);
  digitalWrite(Relay1, HIGH); // ส่งให้ไฟติด
  Serial.println("HIGH");
  delay(1000);
  digitalWrite(Relay2, LOW); // ส่งให้ไฟติด
  Serial.println("LOW");
  delay(1000);
  digitalWrite(Relay2, HIGH); // ส่งให้ไฟติด
  Serial.println("HIGH");
  i = 0;
}
