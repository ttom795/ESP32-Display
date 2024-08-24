#include <JPEGDEC.h>
#include <TFT_eSPI.h>
#include <SPI.h>
#include <stdlib.h>

#define BAUD_RATE 3000000  // Maximum baud rate for ESP32
#define MAX_JPEG_SIZE 10000 // Maximum size of the JPEG image

TFT_eSPI tft = TFT_eSPI();
uint8_t buf[MAX_JPEG_SIZE];
JPEGDEC jpeg;

unsigned long startMillis;
unsigned long frameCount = 0;
unsigned long fps = 0;

TaskHandle_t decodeTaskHandle;

volatile bool newImageReady = false;

int drawMCUs(JPEGDRAW *pDraw) {
  tft.pushImage(pDraw->x, pDraw->y, pDraw->iWidth, pDraw->iHeight, pDraw->pPixels);
  return 1;
}

void decodeJPEGTask(void * parameter) {
  for (;;) {
    // Wait for new image to be ready
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

    if (newImageReady) {
      jpeg.openFLASH(buf, MAX_JPEG_SIZE, drawMCUs);
      jpeg.setPixelType(RGB565_BIG_ENDIAN);
      jpeg.decode(0, 0, 0);
      jpeg.close();

      // FPS display
      tft.fillRect(0, 0, 100, 20, TFT_BLACK);
      tft.setCursor(0, 0);
      tft.print("FPS: ");
      tft.print(fps);
      newImageReady = false;
    }
  }
}

void setup() {
  Serial.begin(BAUD_RATE);
  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(2);
  tft.setCursor(0, 0);
  tft.print("Waiting for image...");
  
  startMillis = millis();

  xTaskCreatePinnedToCore(decodeJPEGTask, "Decode", 10000, NULL, 1, &decodeTaskHandle, 0);
}

void loop() {
  size_t bytesRead = 0;
  bytesRead += Serial.readBytes(buf + bytesRead, MAX_JPEG_SIZE - bytesRead);

  if (bytesRead >= MAX_JPEG_SIZE) {
    newImageReady = true;
    xTaskNotifyGive(decodeTaskHandle);
  }

  // Increment frame count
  frameCount++;

  // Calculate and display FPS every second
  if (millis() - startMillis >= 1000) {
    fps = frameCount;
    frameCount = 0;
    startMillis = millis();
  }
}
