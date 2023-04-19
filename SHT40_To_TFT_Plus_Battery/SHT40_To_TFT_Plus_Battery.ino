/***************************************************
  This is an example for the SHT4x Humidity & Temp Sensor
  Using the SHT4x sensor from Adafruit
  ----> https://www.adafruit.com/products/4885
  Connected to a Lilygo T-Display-S3
  What I consider the bare minimum to display readings on the TFT
  including the onboard buttons and powered by a battery
 ****************************************************/
#define PIN_BUTTON_1    0
#define PIN_BUTTON_2    14
#define PIN_BAT_VOLT    4

#define PIN_LCD_BL      38
#define PIN_POWER_ON    15
#define I2C_SDA 43
#define I2C_SCL 44

#include <TFT_eSPI.h> // Graphics and font library for ST7735 driver chip
TFT_eSPI tft = TFT_eSPI();
TFT_eSprite scrn = TFT_eSprite(&tft); //using a sprite to eliminate flicker

#include "Adafruit_SHT4x.h"
Adafruit_SHT4x sht4 = Adafruit_SHT4x();

void setup() {
  //The two buttons on the board
  pinMode(PIN_BUTTON_1, INPUT);
  pinMode(PIN_BUTTON_2, INPUT);

  //Needed to power TFT on battery
  pinMode(PIN_POWER_ON, OUTPUT);
  digitalWrite(PIN_POWER_ON, HIGH);

  //Initialize the screen and sprite
  tft.init();
  tft.setRotation(3);         //USB on left
  tft.fillScreen(TFT_BLACK);
  scrn.createSprite(320, 170);
  scrn.fillSprite(TFT_BLACK);
  scrn.setTextColor(TFT_CYAN, TFT_BLACK);

  //Initialize the I2C sensor
  Wire.begin(I2C_SDA, I2C_SCL);
  if (! sht4.begin()) {
    scrn.setTextColor(TFT_ORANGE, TFT_BLACK);
    scrn.drawCentreString("Couldn't find SHT4x", 160, 50, 4);
    scrn.pushSprite(0, 0);
    while (1) delay(1);
  }
  sht4.setPrecision(SHT4X_HIGH_PRECISION);
  sht4.setHeater(SHT4X_NO_HEATER);
}

void loop() {
  scrn.fillSprite(TFT_BLACK);
  
  //sht40 sensor
  sensors_event_t humidity, temp;
  sht4.getEvent(&humidity, &temp);// populate temp and humidity objects with fresh data

  scrn.setTextColor(TFT_CYAN);
  scrn.drawCentreString("SHT40", 160, 0, 4);
  scrn.drawString("Temperature:", 0, 40, 4);
  scrn.drawCentreString(String(temp.temperature*9/5+32) + " F", 220, 40, 4);
  scrn.drawString("Humidity:", 0, 80, 4);
  scrn.drawCentreString(String(humidity.relative_humidity) + " %", 220, 80, 4);

  scrn.setTextColor(TFT_GOLD);
  scrn.drawString("Battery", 0, 120, 4);
  
  float batt = (analogRead(PIN_BAT_VOLT) * 2 * 3.3) / 4096;
  if (batt < 3.4) {
    scrn.setTextColor(TFT_RED);
    scrn.drawCentreString(String(batt), 220, 120, 4);
  } else if (batt < 3.7) {
    scrn.setTextColor(TFT_YELLOW);
    scrn.drawCentreString(String(batt), 220, 120, 4);
  } else if (batt < 4) {
    scrn.setTextColor(TFT_GREEN);
    scrn.drawCentreString(String(batt), 220, 120, 4);
  } else if (batt > 4) {
    scrn.setTextColor(TFT_SILVER);
    scrn.drawCentreString("CHARGING", 220, 120, 4);
  }
  scrn.pushSprite(0, 0);
  
  delay(500);
}
