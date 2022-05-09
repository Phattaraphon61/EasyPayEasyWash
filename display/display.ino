
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ESP8266WebServer.h>
#include <AutoConnect.h>
#include <ESP.h>
#include <EEPROM.h>
#include <TFT_eSPI.h>
#include <qrcode_espi.h>
#include <SoftwareSerial.h>
#include <JPEGDecoder.h>
#define minimum(a,b)     (((a) < (b)) ? (a) : (b))
#include "jpeg1.h"
#include "jpeg2.h"
#include "jpeg3.h"
#include "jpeg4.h"
ESP8266WebServer Server;
AutoConnect      Portal(Server);
WiFiClient espClient;
PubSubClient client(espClient);
const char* mqtt_server = "mqtt.easypayeasywash.tk";
TFT_eSPI tft = TFT_eSPI();
QRcode_eSPI qrcode (&tft);
SoftwareSerial arduinocon(5, 4);
uint32_t icount = 0;
char datas[50];
int coin;
String qr;
int page = 0;
int page0 = 0;
int page1 = 0;
int page2 = 0;
int page3 = 0;
int check;

void setup() {
  startsetup();
}

void loop() {
  client.loop();
  Portal.handleClient();
  if (!client.connected()) {
    reconnect();
  }
  static String msg = "";
  uint16_t x, y;
  if (check != 1 && page0 == 0) {
    tft.fillScreen(TFT_WHITE);
    String id = "EPEW" + String(ESP.getChipId());
    qrcode.create(id);
    page0 = 1;
  }
  if (page == 1 && page1 == 0) {
    drawArrayJpeg(Menu, sizeof(Menu), 0, 0);
    //     tft.fillRect(40, 120, 150, 130, TFT_RED);
    page1 = 1;
  }
  if (page == 2 && page2 == 0) {
    drawArrayJpeg(Menucoin, sizeof(Menucoin), 0, 0);
    String str = String(coin);
    page2 = 1;
  }
  if (page == 2) {
    tft.setTextColor(TFT_RED, TFT_WHITE);
    String str = String(coin);
    tft.drawString(str, 210, 140, 8);
  }
  if (page == 3 && page3 == 0) {
    drawArrayJpeg(Menuqrcode, sizeof(Menuqrcode), 0, 0);
    qrcode.create(qr);
    page3 = 1;
  }
  if (tft.getTouch(&x, &y))
  {
    if ((x > 40) && (x < 180)) {
      if ((y > 120) && (y < 255)) {
        arduinocon.write("coin,0\n");
        Serial.println("Coin");
        if (page == 1) {
          page = 2;
          page1 = 0;
        }
      }
    }
    if ((x > 290) && (x < 430)) {
      if ((y > 120) && (y < 255)) {
        arduinocon.write("coin,0\n");
        if (coin != 0) {
          snprintf (datas, 75, "EPEW%ld,%ld", ESP.getChipId(), coin);
          client.publish("payment", datas);
          Serial.println("Qrcode");
        }
      }
    }
    if ((x > 175) && (x < 300)) {
      if ((y > 265) && (y < 305)) {
        Serial.println("Menu");
        if (page != 1) {
          page = 1;
          page2 = 0;
          page3 = 0;
        }
      }
    }
    delay(1000);
  }

  while (arduinocon.available() > 0) {
    char inByte = arduinocon.read();
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
        coin = value_2.toInt();
      }
      if (value_1 == "setcoin") {
        coin -= 10;
      }
      if (value_1 == "change") {
        if (page != 1) {
          page = 1;
          page2 = 0;
          page3 = 0;
        }
      }
      Serial.print( value_1 ); //แปลงค่าจาก String เป็นจำนวนเต็มด้วย toInt()
      Serial.print(" and ");
      Serial.println( value_2 ); //แปลงค่าจาก string เป็นทศนิยมด้วย toFloat()
      msg = "";
    }
  }

}
void drawArrayJpeg(const uint8_t arrayname[], uint32_t array_size, int xpos, int ypos) {

  int x = xpos;
  int y = ypos;
  JpegDec.decodeArray(arrayname, array_size);
  renderJPEG(x, y);

  Serial.println("#########################");
}
void renderJPEG(int xpos, int ypos) {

  // retrieve infomration about the image
  uint16_t *pImg;
  uint16_t mcu_w = JpegDec.MCUWidth;
  uint16_t mcu_h = JpegDec.MCUHeight;
  uint32_t max_x = JpegDec.width;
  uint32_t max_y = JpegDec.height;

  // Jpeg images are draw as a set of image block (tiles) called Minimum Coding Units (MCUs)
  // Typically these MCUs are 16x16 pixel blocks
  // Determine the width and height of the right and bottom edge image blocks
  uint32_t min_w = minimum(mcu_w, max_x % mcu_w);
  uint32_t min_h = minimum(mcu_h, max_y % mcu_h);

  // save the current image block size
  uint32_t win_w = mcu_w;
  uint32_t win_h = mcu_h;

  // record the current time so we can measure how long it takes to draw an image
  uint32_t drawTime = millis();

  // save the coordinate of the right and bottom edges to assist image cropping
  // to the screen size
  max_x += xpos;
  max_y += ypos;

  // read each MCU block until there are no more
  while (JpegDec.read()) {

    // save a pointer to the image block
    pImg = JpegDec.pImage ;

    // calculate where the image block should be drawn on the screen
    int mcu_x = JpegDec.MCUx * mcu_w + xpos;  // Calculate coordinates of top left corner of current MCU
    int mcu_y = JpegDec.MCUy * mcu_h + ypos;

    // check if the image block size needs to be changed for the right edge
    if (mcu_x + mcu_w <= max_x) win_w = mcu_w;
    else win_w = min_w;

    // check if the image block size needs to be changed for the bottom edge
    if (mcu_y + mcu_h <= max_y) win_h = mcu_h;
    else win_h = min_h;

    // copy pixels into a contiguous block
    if (win_w != mcu_w)
    {
      uint16_t *cImg;
      int p = 0;
      cImg = pImg + win_w;
      for (int h = 1; h < win_h; h++)
      {
        p += mcu_w;
        for (int w = 0; w < win_w; w++)
        {
          *cImg = *(pImg + w + p);
          cImg++;
        }
      }
    }

    // calculate how many pixels must be drawn
    uint32_t mcu_pixels = win_w * win_h;

    tft.startWrite();

    // draw image MCU block only if it will fit on the screen
    if (( mcu_x + win_w ) <= tft.width() && ( mcu_y + win_h ) <= tft.height())
    {

      // Now set a MCU bounding window on the TFT to push pixels into (x, y, x + width - 1, y + height - 1)
      tft.setAddrWindow(mcu_x, mcu_y, win_w, win_h);

      // Write all MCU pixels to the TFT window
      while (mcu_pixels--) {
        // Push each pixel to the TFT MCU area
        tft.pushColor(*pImg++);
      }

    }
    else if ( (mcu_y + win_h) >= tft.height()) JpegDec.abort(); // Image has run off bottom of screen so abort decoding

    tft.endWrite();
  }

  // calculate how long it took to draw the image
  drawTime = millis() - drawTime;

  // print the results to the serial port
  Serial.print(F(  "Total render time was    : ")); Serial.print(drawTime); Serial.println(F(" ms"));
  Serial.println(F(""));
}
