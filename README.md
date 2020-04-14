# ALED

ALED is a platform to control "unaddressable" RGB led strip. After reading about WLED project, I found out that WLED is for "addressable" RGB led strip. That is why I started this project.

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

Any suggestions will be welcomed.

Hope you enjoy it.

Brian
