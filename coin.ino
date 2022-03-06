#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ESP8266WebServer.h>
#include <AutoConnect.h>
#include <ESP.h>
#include <EEPROM.h>
#include <SoftwareSerial.h>
int Relay1 = D5;
int Relay2 = D6;
int sensor = D7;
//int led = 4;
int i = 0;
//int c = 0;
const char* mqtt_server = "mqtt.easypayeasywash.tk";

ESP8266WebServer Server;
AutoConnect      Portal(Server);
WiFiClient espClient;
PubSubClient client(espClient);

void ICACHE_RAM_ATTR doCounter();
//void rootPage() {
//  char content[] = "Hello, world";
//  Server.send(200, "text/plain", content);
//}

String espid = "EPEW";
char datas[50];
int coin;
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
    Serial.println(value_2);
  }
  if (value_1 == "op") {
    Serial.println("OPEN");
    EEPROM.begin(12);
    // write a 0 to all 512 bytes of the EEPROM
    for (int i = 0; i < 512; i++) {
      EEPROM.write(i, 0);
    }
    EEPROM.end();
  }
  if (value_1 == "check") {
    Serial.println("check");
    EEPROM.begin(12);
    EEPROM.put(0, value_2.toInt());
    EEPROM.commit();
    EEPROM.end();
    coin =  value_2.toInt();
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

void setup() {
  delay(1000);
  Serial.begin(9600);
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
void loop() {
  static String msg = "";
  client.loop();
  Portal.handleClient();
  if (!client.connected()) {
    reconnect();
  }
  if (i == 20) {
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
  while (Serial.available() > 0) {
    char inByte = Serial.read();


    //เก็บสะสมค่าไว้ใน String ชื่อ msg
    msg += inByte;

    //จนกว่าค่าที่อ่านได้จะเป็นการขึ้นบรรทัดใหม่
    if (inByte == '\n') {

      //ดึงค่าแรก (index 0) ออกจาก String msg เก็บไว้บน value_1
      String value_1 = getValue(msg, ',', 0);

      //ดึงค่าแรก (index 1) ออกจาก String msg เก็บไว้บน value_2
      String value_2 = getValue(msg, ',', 1);
      if (value_1.toInt() == 1) {
        Serial.print("value_1 : ");
        Serial.println(value_2.toFloat());
        snprintf (datas, 75, "EPEW%ld,%ld", ESP.getChipId(), value_2.toInt());
        client.publish("payment", datas);
      }
      if (value_1.toInt() == 2) {
        Serial.print("value_2 : ");
        //        qrcode.create(value_2);
        Serial.println(value_2.toFloat());
        Serial.println(coin);
      }
      Serial.print( value_1.toInt() ); //แปลงค่าจาก String เป็นจำนวนเต็มด้วย toInt()
      Serial.print(" and ");
      Serial.println( value_2.toFloat() ); //แปลงค่าจาก string เป็นทศนิยมด้วย toFloat()
      msg = "";
    }
  }
}

void doCounter() { // เมื่อเซ็นเซอร์ตรวจจับวัตถุ
  i += 10;
  Serial.println(i);
  delay(1000);
}
String getValue(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length() - 1;

  for (int i = 0; i <= maxIndex && found <= index; i++) {
    if (data.charAt(i) == separator || i == maxIndex) {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }

  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}
