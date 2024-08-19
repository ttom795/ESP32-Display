#include <TJpg_Decoder.h>
#include <TFT_eSPI.h>
#include <SPI.h>
#include <stdlib.h>

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 160
#define BAUD_RATE 3000000  // Maximum baud rate for ESP32
#define MAX_JPEG_SIZE 6144 // Maximum size of the JPEG image

TFT_eSPI tft = TFT_eSPI();
uint8_t buf[MAX_JPEG_SIZE];  // Fixed-size buffer for JPEG data

TaskHandle_t Task1;
const uint8_t* arrayName;
bool doDecoding = false;
bool mcuReady = false;
uint16_t mcuBuffer[32 * 32];
int16_t mcu_x, mcu_y, mcu_w, mcu_h;
uint16_t* scaledBitmap;

bool mcu_decoded(int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t* bitmap) {
  if (y >= tft.height()) return 0;
  while (mcuReady) yield();

  memcpy(mcuBuffer, bitmap, 32 * 32 * 2);
  mcu_x = x;
  mcu_y = y;
  mcu_w = w;
  mcu_h = h;
  mcuReady = true;
  return 1;
}

void decodeJpg(void* p) {
  for (;;) {
    if (doDecoding) {
      TJpgDec.drawJpg(0, 0, arrayName, MAX_JPEG_SIZE);
      doDecoding = false;
    }
    yield();
  }
}

unsigned long startMillis;
unsigned long frameCount = 0;

void setup() {
  xTaskCreatePinnedToCore(decodeJpg, "decodeJpg", 10000, NULL, 0, &Task1, 0);
  Serial.begin(BAUD_RATE);
  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(2);
  tft.setCursor(10, 10);
  tft.print("Waiting for image...");

  TJpgDec.setSwapBytes(true);
  TJpgDec.setCallback(mcu_decoded);

  startMillis = millis();
}

void loop() {
  size_t bytesRead = 0;
  while (bytesRead < MAX_JPEG_SIZE) {
    bytesRead += Serial.readBytes(buf + bytesRead, MAX_JPEG_SIZE - bytesRead);
  }
  
  if (bytesRead == MAX_JPEG_SIZE) {
    arrayName = buf;
    mcuReady = false;
    doDecoding = true;

    
    while (doDecoding || mcuReady) {
      if (mcuReady) {
        tft.pushImage(mcu_x, mcu_y, mcu_w, mcu_h, mcuBuffer);
        mcuReady = false;
      }
      yield();
    }
    
  }
}
