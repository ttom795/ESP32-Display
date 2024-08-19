# ESP32 Display Cloner

This project demonstrates how to clone and display images on an SP7789 display using an ESP32. It uses the `TJpg_Decoder` library to decode JPEG images and the `TFT_eSPI` library for handling the display.

## Requirements

- **ESP32** microcontroller
- **SP7789** display (or any other display if you want to spend time getting it to work)

## Libraries

This project requires the following libraries:

- `TJpg_Decoder` for decoding JPEG images
- `TFT_eSPI` for interfacing with the display

You can install these libraries through the Arduino Library Manager or download them from their respective repositories.

## Setup

### Libraries

1. Install the `TJpg_Decoder` library. You can find it [here](https://github.com/Bodmer/TJpg_Decoder).
2. Install the `TFT_eSPI` library. You can find it [here](https://github.com/Bodmer/TFT_eSPI).

### Configuration

1. Ensure that your `User_Setup.h` file is properly configured for the SP7789 display. 

   - Set the SPI frequency to `80MHz`:
     ```cpp
     #define SPI_FREQUENCY 80000000
     ```

2. Connect your ESP32 to the SP7789 display according to the wiring diagram provided by the `TFT_eSPI` library documentation.

## Usage

1. Flash the ESP32
2. Run the Client script from your device - currently, only Ubuntu has been tested.
