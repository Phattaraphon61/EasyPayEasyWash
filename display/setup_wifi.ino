void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  WiFi.mode(WIFI_STA);
  //  Server.on("/", rootPage);
  if (Portal.begin()) {
    Serial.println("WiFi connected: " + WiFi.localIP().toString());
  }
  randomSeed(micros());
}
