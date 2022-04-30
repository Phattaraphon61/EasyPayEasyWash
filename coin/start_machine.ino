void start_machine(){
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
