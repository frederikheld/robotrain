# Simple remote control for _robotrain_

A simple remote control for _robotrain_ with a little display and two buttons. It can be used to send speed and direction commands to [_motor-control_](../motor-control). It also displays the actual speed of the train.


![Picture of remote-control on a breadboard](remote-control_picture.jpg)


## Setup your hardware

> Note: Wire colors do not match picture above!

![Wiring diagram of remote-control](remote-control_bb.png)

This is an export from the [Fritzing file](remote-contorl.fzz) in this directory.

### Note on the OLED display

128x64 px OLED displays come in many different configurations which all look very similar or the same. Your display will most likely not work on the first try!

This Sketch uses the library [_U8g2_ library by _oliver_](https://github.com/olikraus/u8g2), because it supporty a wide variety of configurations of this display.

To learn more about your display, it recommend to [watch this video](https://www.youtube.com/watch?v=0ZNhzXrhBVA).

You can select a different display by changing the following line right at the beginning of the sketch:

```C
// OLED display:
U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, PIN_DISPLAY_SCL, PIN_DISPLAY_SDA, U8X8_PIN_NONE);
```

## Configure and upload the Sketch

### Configuration file

This directory contains a file named `config.template.h`. Please create a copy of this file and re-name it `config.h`.

You need to configure your WiFi (ssid and secret) as well as MQTT broker (server and port) in `config.h`.

The dealys and timeouts for WiFi and MQTT usually don't need to be changed. If your infrastructure is particularily slow or instable, changing the values might help to fix issues.

If you use a different wiring, you can configure it in the pinning section.

The same applies to the MQTT topics. You should only change those if you use different topics in your whole _robotrain_ setup.

### Install libraries

This sketch uses the following standard libraries that come with the Arduino IDE:

* [_WiFi_](https://www.arduino.cc/en/Reference/WiFi) (1.2.7)

It also uses the following libraries that can be installed via the Library Manager:

* [_PubSubClient_ by _Nick O'Leary_](https://pubsubclient.knolleary.net/) (2.7.0)
* [_U8g2_ library by _oliver_](https://github.com/olikraus/u8g2) (2.27.6)

The version used and tested with this sketch is indicated in round brackets.

### Upload the Sketch

Select _NodeMCU 1.0 (ESP-12E Module)_ in the _Tools_ menu. 

This board is not available in a fresh installation of Arduino IDE! If you haven't worked with NodeMCU before, you have to install the board via the _Board Manager_. The package name is [_esp8266_ by _ESP8266 Community_](https://github.com/esp8266/Arduino).

Now you can upload the sketch to your board.

## Usage

// tbd