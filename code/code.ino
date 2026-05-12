#include <GxEPD2_BW.h>
#include <SPI.h>
#include <Fonts/FreeMonoBold9pt7b.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <esp_wpa2.h>
#include <WiFiClientSecure.h>

#define EPD_CS   15
#define EPD_DC   27
#define EPD_RST  26
#define EPD_BUSY 25
#define SPI_MOSI 14
#define SPI_CLK  13

GxEPD2_BW<GxEPD2_750_T7, GxEPD2_750_T7::HEIGHT> epaper(
  GxEPD2_750_T7(EPD_CS, EPD_DC, EPD_RST, EPD_BUSY)
);

void setup() 
{
  Serial.begin(115200);
  SPI.begin(SPI_CLK, -1, SPI_MOSI, EPD_CS);

  epaper.init(115200, true, 2, false);

  epaper.setRotation(0);
  epaper.setFont(&FreeMonoBold9pt7b);
  epaper.setTextColor(GxEPD_BLACK);
  epaper.setFullWindow();
  epaper.firstPage();
}

void loop()
{
  
}

