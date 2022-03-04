/**
   www.arduinona.com
   ตัวอย่างการสื่อสารข้อมูลระหว่าง Arduino และ ESP8266
*/

#include <SoftwareSerial.h>

/**
   RX = 10
   TX = 11
*/
SoftwareSerial esp8266con(10, 11);


void setup()  {
  pinMode(2, INPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  digitalWrite(3, 1);
  digitalWrite(4, 1);
  digitalWrite(5, 1);
  digitalWrite(6, HIGH);
  digitalWrite(7, HIGH);
  digitalWrite(8, HIGH);
  Serial.begin(115200);
  attachInterrupt(0, doCounter, FALLING);
  esp8266con.begin(57600);
  Serial.println("\n\n\nStart arduino from ArduinoNa.com");

}

int count = 0;
int mode = 0;
void loop() {
  if (mode == 1) {
    if (count == 3) {
      Serial.println("30");
      count = 0;
      mode = 0;
      digitalWrite(3, 0);
      delay(1000);
      digitalWrite(3, 1);
      delay(1000);
      digitalWrite(4, 0);
      delay(1000);
      digitalWrite(4, 1);
      delay(1000);
      digitalWrite(5, 0);
      delay(1000);
      digitalWrite(5, 1);
      delay(1000);

    }
  }

  if (mode == 2) {
    if (count == 2) {
      Serial.println("20");
      count = 0;
      mode = 0;
      digitalWrite(3, 0);
      delay(1000);
      digitalWrite(3, 1);
      delay(1000);
      digitalWrite(4, 0);
      delay(1000);
      digitalWrite(4, 1);
      delay(1000);
      digitalWrite(4, 0);
      delay(1000);
      digitalWrite(4, 1);
      delay(1000);
      digitalWrite(5, 0);
      delay(1000);
      digitalWrite(5, 1);
      delay(1000);
    }
  }

  /**
     เช็คว่าถ้ามีค่าเข้ามาจาก esp8266 ให้วนรับค่าทีละตัวอักษร
     และส่งออก Serial monitor ทีละตัวอักษร จนครบทุกตัวอักษร
     และออกจาก loop
  */
  while (esp8266con.available() > 0) {
    char inByte = esp8266con.read();
    char check = 'm';
    if (inByte == 'N') {
      Serial.println('n');
      mode = 1;
    }
    if (inByte == 'Q') {
      Serial.println('q');
      mode = 2;
    }
    if (inByte == check) {
      //      Serial.write(inByte);
      Serial.println("M");
    }
  }

  /**
     เช็คว่าถ้ามีค่าเข้ามาจาก Serial monitor ให้วนรับค่าทีละตัวอักษร
     และส่งออกไปยัง esp8266 ทีละตัวอักษร
     และส่งออก Serial monitor ทีละตัวอักษร จนครบทุกตัวอักษร
     และออกจาก loop
  */
  while (Serial.available()) {

    /**
       รับตัวอักษร 1 ตัว (1byte)
    */
    char inChar = Serial.read();

    /**
       ส่งตัวอักษรนั้นออกไปที่ esp8266
    */
    esp8266con.write(inChar);
    Serial.write(inChar);
  }

}
void doCounter() { // เมื่อเซ็นเซอร์ตรวจจับวัตถุ
  Serial.println("YES");
  esp8266con.write('C');
  if (mode != 0) {
    count++;
  };
  delay(1000);
}
