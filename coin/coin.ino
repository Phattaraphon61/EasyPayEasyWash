#include <ESP.h>
#include <EEPROM.h>
#include <SoftwareSerial.h>
void ICACHE_RAM_ATTR doCounter();
SoftwareSerial esp8266con(5, 4);
int Relay1 = D5;
int Relay2 = D6;
int sensor = D7;
int ldr = D0;
int ldrval = 0;
int i = 0;
char datas[50];
int coin;
static String msg = "";
void setup() {
  startsetup();
}
void loop() {
  delay(1000);
  ldrval = digitalRead(ldr);
  Serial.println(ldrval);
  if (ldrval == 1) {
    start_machine();
  };
  if (i == coin) {
    start_machine();
  }
  while (esp8266con.available() > 0) {
    char inByte = esp8266con.read();
    Serial.print(inByte);

    //เก็บสะสมค่าไว้ใน String ชื่อ msg
    msg += inByte;

    //จนกว่าค่าที่อ่านได้จะเป็นการขึ้นบรรทัดใหม่
    if (inByte == '\n') {

      //ดึงค่าแรก (index 0) ออกจาก String msg เก็บไว้บน value_1
      String value_1 = getValue(msg, ',', 0);
      //ดึงค่าแรก (index 1) ออกจาก String msg เก็บไว้บน value_2
      String value_2 = getValue(msg, ',', 1);
      if (value_1 == "coin") {
        writeString("coin," + String(coin) + "\n");
      }
      if (value_1 == "setcoin") {
        EEPROM.begin(12);
        EEPROM.put(0, value_2.toInt());
        EEPROM.commit();
        EEPROM.end();
        coin =  value_2.toInt();
      }
      if (value_1 == "op") {
        start_machine();
      }
      Serial.print( value_1 ); //แปลงค่าจาก String เป็นจำนวนเต็มด้วย toInt()
      Serial.print(" and ");
      Serial.println( value_2.toFloat() ); //แปลงค่าจาก string เป็นทศนิยมด้วย toFloat()
      msg = "";
    }
  }
}

void doCounter() { // เมื่อเซ็นเซอร์ตรวจจับวัตถุ
  i += 10;
  esp8266con.write("setcoin,0\n");
  Serial.println(i);
  delay(1000);
}

void writeString(String stringData) {
  for (int i = 0; i < stringData.length(); i++) {
    esp8266con.write(stringData[i]);
  }
}
