# ESP32 Display Cloner

This project demonstrates how to clone and display images on an SP7789 display using an ESP32. It uses the `JPEGDEC` library to decode JPEG images and the `TFT_eSPI` library for handling the display.

Update 2024/08/24: Tried using DMA. It didn't really work and I got more of a performance increase from moving the decoding to a seperate core. Currently the limit on framrate is the speed of decoding - it's possible to send and render data at a faster rate, but the decoder doesn't run fast enough to keep up.

[![Watch the video](https://img.youtube.com/vi/Y05WvNFZAC0/hqdefault.jpg)](https://youtu.be/Y05WvNFZAC0)


## Requirements

- **ESP32** microcontroller
- **SP7789** display (or any other display if you want to spend time getting it to work)

## Libraries

This project requires the following libraries:

- `JPEGDEC` for decoding JPEG images
- `TFT_eSPI` for interfacing with the display

You can install these libraries through the Arduino Library Manager or download them from their respective repositories.

## Setup

### Libraries

1. Install the `JPEGDEC` library. You can find it [here](https://github.com/bitbank2/JPEGDEC).
2. Install the `TFT_eSPI` library. You can find it [here](https://github.com/Bodmer/TFT_eSPI).

### Configuration

1. Ensure that your `User_Setup.h` file is properly configured for the SP7789 display. 

   - Set the SPI frequency to `80MHz` & enable HSPI (it fixed visual issues for me):
     ```cpp
     #define SPI_FREQUENCY 80000000
     #define USE_HSPI_PORT
     ```

2. Connect your ESP32 to the SP7789 display according to the wiring diagram provided by the `TFT_eSPI` library documentation.

## Usage

1. Flash the ESP32
2. Run the Client script from your device - currently, only Ubuntu has been tested.
