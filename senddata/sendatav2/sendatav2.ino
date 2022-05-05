/**
 * www.arduinona.com
 * ตัวอย่างการสื่อสารข้อมูลระหว่าง Arduino และ ESP8266
 */
#include <SoftwareSerial.h>

/**
 * NodeMCU v3 (ESP8266)
 * RX = GPIO 5 (D1)
 * TX = GPIO 4 (D2)
 */
SoftwareSerial arduinocon(4, 5); 


void setup()  {

  Serial.begin(115200);

  arduinocon.begin(57600);

  Serial.println("\n\n\nStart esp8266 from ArduinoNa.com");

}

 

void loop() {

  /**
   * เช็คว่าถ้ามีค่าเข้ามาจาก Serial monitor ให้วนรับค่า  
   * และแสดงออกทาง arduino ทีละตัวอักษร
   * และแสดงออกทาง Serial monitor ทีละตัวอักษร จนครบทุกตัวอักษร
   * และออกจาก loop
   */
  while (Serial.available() > 0) {
    char inByte = Serial.read();
    arduinocon.write("coin,00\n");
    Serial.write(inByte);
  }
  

  /**
   * เช็คว่าถ้ามีค่าเข้ามาจาก arduino ให้วนรับค่า 
   * และแสดงออกทาง Serial monitor ทีละตัวอักษร จนครบทุกตัวอักษร
   * และออกจาก loop
   */
  while (arduinocon.available() > 0) {
    char inByte = arduinocon.read();
    Serial.write(inByte);
  }


  
  

}
