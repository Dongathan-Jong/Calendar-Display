#include <GxEPD2_BW.h>
#include <SPI.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>
#include "centurygothic16pt7b.h"
#include "centurygothic_bold16pt7b.h"
#include "centurygothic24pt7b.h"

#define EPD_CS   15
#define EPD_DC   27
#define EPD_RST  26
#define EPD_BUSY 25
#define SPI_MOSI 14
#define SPI_CLK  13

const unsigned char locationSymbol[] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 
  0x00, 0x0f, 0xc0, 0x00, 
  0x00, 0x3f, 0xf0, 0x00, 
  0x00, 0x7f, 0xf8, 0x00, 
  0x00, 0xff, 0xfc, 0x00, 
  0x01, 0xff, 0xfe, 0x00, 
  0x01, 0xf8, 0x7e, 0x00, 
  0x03, 0xf0, 0x3f, 0x00, 
  0x03, 0xe0, 0x1f, 0x00, 
  0x03, 0xe0, 0x1f, 0x00, 
  0x03, 0xe0, 0x1f, 0x00, 
  0x03, 0xe0, 0x1f, 0x00, 
  0x01, 0xf0, 0x3e, 0x00, 
  0x01, 0xf8, 0x7e, 0x00, 
  0x00, 0xff, 0xfc, 0x00, 
  0x00, 0xff, 0xfc, 0x00, 
  0x00, 0x7f, 0xf8, 0x00, 
  0x00, 0x7f, 0xf8, 0x00, 
  0x00, 0x3f, 0xf0, 0x00, 
  0x00, 0x3f, 0xf0, 0x00, 
  0x00, 0x1f, 0xe0, 0x00, 
  0x00, 0x0f, 0xc0, 0x00, 
  0x00, 0x0f, 0xc0, 0x00, 
  0x00, 0x07, 0x80, 0x00, 
  0x00, 0x07, 0x80, 0x00, 
  0x00, 0x03, 0x00, 0x00, 
  0x00, 0x03, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00
};

GxEPD2_BW<GxEPD2_750_T7, GxEPD2_750_T7::HEIGHT> epaper(
  GxEPD2_750_T7(EPD_CS, EPD_DC, EPD_RST, EPD_BUSY)
);

const char* ssid = "";
const char* password = "";
const char* appsScript = "";

String eventData[] = {"", "", "", "", "", "", "", ""};

WiFiClientSecure client;
HTTPClient http;

void setup() 
{
  Serial.begin(115200);
  SPI.begin(SPI_CLK, -1, SPI_MOSI, EPD_CS);

  epaper.init(115200, true, 2, false);
  epaper.setRotation(0);
  epaper.setFont(&centurygothic16pt7b);
  epaper.setTextColor(GxEPD_BLACK);
  epaper.setFullWindow();
  epaper.firstPage();

  do
  {
    epaper.fillScreen(GxEPD_WHITE);
    epaper.setCursor(250,250);
    epaper.write("Connecting to WiFi");
  }
  while(epaper.nextPage());

  connectingToWifi();

  do
  {
    epaper.setFont(&centurygothic_bold16pt7b);
    epaper.fillScreen(GxEPD_WHITE);

    for(int i = 20; i < 40; i++)
    {
      for(int j = 29; j < 148; j++)
      {
        if((i + j) % 2 == 0)
        {
          epaper.drawPixel(i, j, GxEPD_BLACK);
        }
      }
    }
  }
  while(epaper.nextPage());

}

void loop()
{
  //TESTING

  int currentIndex = 0;
  int indicies[] = {0, 0, 0, 0, 0, 0, 0};
  String payload = "14h, Flight to Chicago (UA 1990), Austin-Bergstrom Airport, Tomorrow, 10:00 AM, Cloudy, 14.8 C, 19.4 km/h";

  for(int i = 0; i < payload.length(); i++)
  {
    if (payload.charAt(i) == ',')
    {
      indicies[currentIndex] = i;
      currentIndex++;
    }
  }

  eventData[0] = payload.substring(0, indicies[0]);
  eventData[1] = payload.substring(indicies[0] + 2, indicies[1]);
  eventData[2] = payload.substring(indicies[1] + 2, indicies[2]);
  eventData[3] = payload.substring(indicies[2] + 2, indicies[3]);
  eventData[4] = payload.substring(indicies[3] + 2, indicies[4]);
  eventData[5] = payload.substring(indicies[4] + 2, indicies[5]);
  eventData[6] = payload.substring(indicies[5] + 2, indicies[6]);
  eventData[7] = payload.substring(indicies[6] + 2);
  //updateData();
  atAGlance();
  delay(50000000);
}

void updateData()
{
  int currentIndex = 0;
  int indicies[] = {0, 0, 0, 0, 0, 0, 0};

  String payload = "";
  http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
  client.setInsecure();

  http.begin(client, appsScript);

  int httpCode = http.GET();

  if (httpCode > 0)
  {
    payload = http.getString();
    Serial.println(payload);
  }
  else
  {
    Serial.println(httpCode);
  }
  http.end();

  for(int i = 0; i < payload.length(); i++)
  {
    if (payload.charAt(i) == ',')
    {
      indicies[currentIndex] = i;
      currentIndex++;
    }
  }
  
  for(int i = 0; i < 7; i++)
  {
    Serial.println(indicies[i]);
  }

  eventData[0] = payload.substring(0, indicies[0]);
  eventData[1] = payload.substring(indicies[0] + 2, indicies[1]);
  eventData[2] = payload.substring(indicies[1] + 2, indicies[2]);
  eventData[3] = payload.substring(indicies[2] + 2, indicies[3]);
  eventData[4] = payload.substring(indicies[3] + 2, indicies[4]);
  eventData[5] = payload.substring(indicies[4] + 2, indicies[5]);
  eventData[6] = payload.substring(indicies[5] + 2, indicies[6]);
  eventData[7] = payload.substring(indicies[6] + 2);
  
  for(int i = 0; i < 8; i++)
  {
    Serial.println(eventData[i]);
  }
}

void connectingToWifi()
{
  WiFi.disconnect(true);
  WiFi.mode(WIFI_STA);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("Connected!");  
}

void atAGlance()
{
  do
  {
    // draw until: 
    epaper.setFont(&centurygothic_bold16pt7b);
    if(eventData[0] != "Now")
    {
      epaper.setCursor(50, 50);
      epaper.write("In ");
      epaper.write(eventData[0].c_str());
      epaper.write(", ");
      epaper.write(eventData[3].c_str());
      epaper.write(" at ");
      epaper.write(eventData[4].c_str());
    }
    else
    {
      epaper.setCursor(50, 50);
      epaper.write(eventData[0].c_str());
    }

    // draw next event
    epaper.setFont(&centurygothic24pt7b);
    epaper.setCursor(48,100);
    epaper.write(eventData[1].c_str());

    // draw event location
    epaper.setFont(&centurygothic16pt7b);
    epaper.drawBitmap(48, 120, locationSymbol, 30, 30, GxEPD_BLACK);
    epaper.setTextSize(1);
    epaper.setCursor(85, 145);
    epaper.write(eventData[2].c_str());

    // draw event Date + Time
    //epaper.draw
  }
  while(epaper.nextPage());
}
