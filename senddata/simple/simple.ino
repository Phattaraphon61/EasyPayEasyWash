#include <qrcode_espi.h>


#include <SPI.h>
#include <TFT_eSPI.h>

TFT_eSPI display = TFT_eSPI();
QRcode_eSPI qrcode (&display);


void setup() {

  Serial.begin(115200);
  Serial.println("");
  Serial.println("Starting...");

  // enable debug qrcode
  // qrcode.debug();
  display.init();
  // Initialize QRcode display using library
  qrcode.init();
  // create qrcode

}

void loop() {
  static String msg = "";

  /**
     เช็คว่าถ้ามีค่าเข้ามาจาก Serial monitor ให้วนรับค่า
     และแสดงออกทาง arduino ทีละตัวอักษร
     และแสดงออกทาง Serial monitor ทีละตัวอักษร จนครบทุกตัวอักษร
     และออกจาก loop
  */


  /**
     เช็คว่าถ้ามีค่าเข้ามาจาก arduino ให้วนรับค่า
     และแสดงออกทาง Serial monitor ทีละตัวอักษร จนครบทุกตัวอักษร
     และออกจาก loop
  */
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
      }
      if (value_1.toInt() == 2) {
        Serial.print("value_2 : ");
        qrcode.create(value_2);
        Serial.println(value_2.toFloat());
      }
      Serial.print( value_1.toInt() ); //แปลงค่าจาก String เป็นจำนวนเต็มด้วย toInt()
      Serial.print(" and ");
      Serial.println( value_2.toFloat() ); //แปลงค่าจาก string เป็นทศนิยมด้วย toFloat()
      msg = "";
    }
  }
}


/**
   ใช้ดึงค่า String ออกจาก String โดยใช้การตัดคำจาก separator ซึ่ง
   อาจจะเป็น , ; : หรืออะไรก็ได้ที่ใช้แยกคำ
*/
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
