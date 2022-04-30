#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ESP8266WebServer.h>
#include <AutoConnect.h>
#include <ESP.h>
#include <EEPROM.h>
#include <SoftwareSerial.h>
ESP8266WebServer Server;
AutoConnect      Portal(Server);
WiFiClient espClient;
PubSubClient client(espClient);
const char* mqtt_server = "mqtt.easypayeasywash.tk";
void ICACHE_RAM_ATTR doCounter();
int Relay1 = D5;
int Relay2 = D6;
int sensor = D7;
int ldr = D0;
int ldrval = 0;
int i = 0;
String espid = "EPEW";
char datas[50];
int coin;
static String msg = "";
void setup() {
  startsetup();
}
void loop() {
  ldrval = digitalRead(ldr);
  client.loop();
  Portal.handleClient();
  if (!client.connected()) {
    reconnect();
  }
  if (i == 20) {
    start_machine();
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
        Serial.print("ldrval = ");
        Serial.println(ldrval);
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
