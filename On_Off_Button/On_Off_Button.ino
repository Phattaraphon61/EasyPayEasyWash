// Example of drawing a graphical "switch" and using
// the touch screen to change it's state.

// This sketch does not use the libraries button drawing
// and handling functions.

// Based on Adafruit_GFX library onoffbutton example.

// Touch handling for XPT2046 based screens is handled by
// the TFT_eSPI library.

// Calibration data is stored in SPIFFS so we need to include it
#include <SPI.h>
#include "font.h"

#include <TFT_eSPI.h> // Hardware-specific library

TFT_eSPI tft = TFT_eSPI();       // Invoke custom library

// This is the file name used to store the touch coordinate
// calibration data. Change the name to start a new calibration.


// Set REPEAT_CAL to true instead of false to run calibration
// again, otherwise it will only be done once.
// Repeat calibration if you change the screen rotation.
#define REPEAT_CAL false

bool SwitchOn = false;

// Comment out to stop drawing black spots
#define BLACK_SPOT

// Switch position and size
//#define FRAME_X 200
//#define FRAME_Y 64
//#define FRAME_W 250
//#define FRAME_H 100

// Red zone size
#define REDBUTTON_X 81
#define REDBUTTON_Y 122
#define REDBUTTON_W 136
#define REDBUTTON_H 76

#define TWOBUTTON_X 274
#define TWOBUTTON_Y 122
#define TWOBUTTON_W 136
#define TWOBUTTON_H 76

// Green zone size
//#define GREENBUTTON_X 270
//#define GREENBUTTON_Y FRAME_Y
//#define GREENBUTTON_W (FRAME_W/2)
//#define GREENBUTTON_H FRAME_H

//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
void setup(void)
{
  Serial.begin(9600);
  tft.init();

  // Set the rotation before we calibrate
  tft.setRotation(1);

  // call screen calibration
  touch_calibrate();

  // clear screen
  tft.fillScreen(TFT_GOLD);

  // Draw button (this example does not use library Button class)
  redBtn();
  twoBtn();
}
//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
void loop()
{
  uint16_t x, y;

  // See if there's any touch data for us
  if (tft.getTouch(&x, &y))
  {
    Serial.println(x,y);
    // Draw a block spot to show where touch was calculated to be
    //    #ifdef BLACK_SPOT
    //      tft.fillCircle(x, y, 2, TFT_BLACK);
    //    #endif

    if ((x > REDBUTTON_X) && (x < (REDBUTTON_X + REDBUTTON_W))) {
      if ((y > REDBUTTON_Y) && (y <= (REDBUTTON_Y + REDBUTTON_H))) {
        Serial.println("Red btn hit");
        redBtn();
      }
    }
    if ((x > TWOBUTTON_X) && (x < (TWOBUTTON_X + TWOBUTTON_W))) {
      if ((y > TWOBUTTON_Y) && (y <= (TWOBUTTON_Y + TWOBUTTON_H))) {
        Serial.println("Green btn hit");
        //          greenBtn();
      }
    }
    delay(1000);
  }
}
//------------------------------------------------------------------------------------------

void drawFrame()
{
  //  tft.drawRect(FRAME_X, FRAME_Y, FRAME_W, FRAME_H, TFT_BLACK);
}

// Draw a red button
void redBtn()
{
  tft.fillRect(REDBUTTON_X, REDBUTTON_Y, REDBUTTON_W, REDBUTTON_H, TFT_RED);
  //  tft.fillRect(GREENBUTTON_X, GREENBUTTON_Y, GREENBUTTON_W, GREENBUTTON_H, TFT_DARKGREY);
  drawFrame();
  tft.drawRect(REDBUTTON_X, REDBUTTON_Y, REDBUTTON_W, REDBUTTON_H, TFT_BLACK);
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(2);
  tft.setTextDatum(MC_DATUM);
  tft.drawString("จ่ายด้วยเหรียญ", (REDBUTTON_W / 2) + REDBUTTON_X, 220);
  SwitchOn = true;
}
void twoBtn()
{
  tft.fillRect(TWOBUTTON_X, TWOBUTTON_Y, TWOBUTTON_W, TWOBUTTON_H, TFT_RED);
  //  tft.fillRect(GREENBUTTON_X, GREENBUTTON_Y, GREENBUTTON_W, GREENBUTTON_H, TFT_DARKGREY);
  drawFrame();
  tft.drawRect(TWOBUTTON_X, TWOBUTTON_Y, TWOBUTTON_W, TWOBUTTON_H, TFT_BLACK);
  tft.setTextColor(TFT_WHITE);
  tft.loadFont(Prompt_ExtraLight);
  tft.setTextSize(5);
  tft.setTextDatum(MC_DATUM);
  tft.drawString("สแกนจ่าย", (TWOBUTTON_W / 2) + TWOBUTTON_X, 220);
  tft.unloadFont();
  SwitchOn = true;
}

// Draw a green button
//void greenBtn()
//{
//  tft.fillRect(GREENBUTTON_X, GREENBUTTON_Y, GREENBUTTON_W, GREENBUTTON_H, TFT_GREEN);
//  tft.fillRect(REDBUTTON_X, REDBUTTON_Y, REDBUTTON_W, REDBUTTON_H, TFT_DARKGREY);
//  drawFrame();
//  tft.setTextColor(TFT_WHITE);
//  tft.setTextSize(2);
//  tft.setTextDatum(MC_DATUM);
//  tft.drawString("OFF", REDBUTTON_X + (REDBUTTON_W / 2) + 1, REDBUTTON_Y + (REDBUTTON_H / 2));
//  SwitchOn = true;
//}
