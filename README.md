# ALED

ALED is a platform to control an **unaddressable** RGB LED strip. After reading about WLED project, I found out that WLED is for an **addressable** RGB LED strip. So, I started this project naturally to support an **unaddressable** RGB led strip.

## Required parts

### Hardware

- ESP32
- RGB LED strip(**unaddressable**). It must have 4 pins(+,R,G,B).
- 2N2222 transistor * 3 (Any NPN transistor would be ok as long as it can handle at least 300mA)
- Resistor * 3 (Optional)

### Software

- Arduino IDE
- ESP32 filesystem uploader https://github.com/me-no-dev/arduino-esp32fs-plugin
- ALED source code

## How to install

1. Upload sketch data using ESP32 filesystem uploader. And create credentials.h file in sketch folder then modify values for SSID, PASSWORD like this;

```
/* 
 * WIFI credentials
 * 
 */
#define SSID "SSID OF MY ROUTER"
#define PASSWORD "PASSWORD OF MY ROUTER"
```

2. Upload sketch in Arduino IDE
3. Reset ESP32 and wait until it shows IP address in Arduino IDE's serial monitor
4. Connect to ESP32 with IP address on web browser
5. Control LED's color using color wheel

## Current status

A user can control the LED strip via ESP32 web interface.

## Wiring diagram

The schematic is pretty simple. It uses 3 2N2222 transistors to connect RGB strip to ESP32

![Schematic](https://github.com/briankimstudio/aled/blob/master/schematic.png)

## Web interface

For the color wheel, it uses https://github.com/jweir/colorwheel library.

![Web interface](https://github.com/briankimstudio/aled/blob/master/aled_web_interface.png)

## Demo

![Demo](https://github.com/briankimstudio/aled/blob/master/aled_demo.gif)

## Todo

- Add WiFiManager to manage SSID easily.
- Switch color wheel interface for more function or control.

## RGB strip

My RGB strip has 30 LEDs and 2 meters long. When I checked power consumption with maximum brightness, it showed around 900mA. Therefore, each channel draws around 300mA at peak. 2N2222 transistor might be good enough for this project. But, more accurate calculation would be necessary for safety. And the value of resistors in the schematic is related to the brightness of the LED. After the test, I removed them to make it a little bit brighter.

Additionally, be aware that the RGB pin's layout on the strip may not as accurate as you might think. As shown below, it labeled G R B(from top), but, after test, it turned out B R G(from top). Make sure the correct layout of your strip and modify PIN assignment in the source code accordingly. 

![RGB strip](https://github.com/briankimstudio/aled/blob/master/rgb_led_strip.jpg)

Any suggestions will be welcomed.

Hope you enjoy it.

Brian
