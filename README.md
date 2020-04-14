# ALED

ALED is a platform to control "unaddressable" RGB led strip. After reading about WLED project, I found out that WLED is for "addressable" RGB led strip. So, I started this project naturally to support "unaddressable" RGB led strip.

## Current status

A user can control led strip via ESP32 web interface.

Schematic is pretty simple. It uses 3 2N2222 transistors to connect RGB strip to ESP32

![Schematic](https://github.com/briankimstudio/aled/blob/master/schematic.png)

For color wheel, it uses https://github.com/jweir/colorwheel library.

![Web interface](https://github.com/briankimstudio/aled/blob/master/aled_web_interface.png)

Demo

![Demo](https://github.com/briankimstudio/aled/blob/master/aled_demo.gif)

## Todo

- Add WiFiManager for manage SSID easily.
- Switch color wheel interface for more function or control.

## RGB strip

My RGB strip has 30 leds and 2 meters long. When I checked power consumption with maximum brightness, it showed around 900mA. Therefore, each channel draws around 300mA at peak. 2N2222 transistor might be good enough for this project. But, more accurate calcaulation would be necessary for safety. And value of resistors in the schematic is related to the brightness of the led. After test, I removed them to make it a little bit brighter.

Any suggestions will be welcomed.

Hope you enjoy it.

Brian
