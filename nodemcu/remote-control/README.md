# Simple remote control for _robotrain_

A simple remote control for _robotrain_ with a little display and two buttons. It can be used to send speed and direction commands to [_motor-control_](../motor-control). It also displays the actual speed of the train.

![Picture of remote-control on a breadboard](remote-control_picture.jpg)

## Setup your hardware

> Note: Wire colors do not match picture above!

![Wiring diagram of remote-control](remote-control_bb.png)

This is an export from the [Fritzing file](remote-contorl.fzz) in this directory.

### Note on the OLED display

128x64 px OLED displays come in many different configurations which all look very similar or even the same. Your display will most likely not work on the first try!

This Sketch uses the library [_U8g2_ library by _oliver_](https://github.com/olikraus/u8g2) because it supporty a wide variety of configurations of this display.

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

> Note that you have to provide your WiFi secret in plaintext. This is a security risk as Arduino programs can be downloaded from the board. Therefore I recommend to use a separate WiFi for your _robotrain_ setup.

The delays and timeouts for WiFi and MQTT usually don't need to be changed. If your infrastructure is particularily slow or instable, changing the values might help to fix issues.

If you use a different wiring, you can configure it in the pinning section.

The same applies to the MQTT topics. You should only change those if you use different topics in your whole _robotrain_ setup.

### Install libraries

This sketch uses the following standard libraries that come with the Arduino IDE:

* [_WiFi_](https://www.arduino.cc/en/Reference/WiFi) (1.2.7)

It also uses the following additional libraries that can be installed via _Tools > Manage Libraries..._:

* [_PubSubClient_ by _Nick O'Leary_](https://pubsubclient.knolleary.net/) (2.7.0)
* [_U8g2_ library by _oliver_](https://github.com/olikraus/u8g2) (2.27.6)

The version used and tested with this sketch is indicated in round brackets.

### Upload the Sketch

If you haven't used _NodeMCU_ boards before, you need to install it via _Tools > Board Manager_ first. The package name is [_esp8266_ by _ESP8266 Community_](https://github.com/esp8266/Arduino).

Then select _NodeMCU 1.0 (ESP-12E Module)_ in the _Tools_ menu and upload the sketch to your board.

## Usage

### Control the train

This remote control can send _nominal_ values to the remote control and display _actual_ values received from the train.

#### Buttons

You can increase the nominal speed in steps with the right button up to the max value. If max value is reached, clicking the button will not increase the speed further.

You can decrease the nominal speed in steps with the left button down to 0. If 0 is reached, clicking the down button again will change the _nominal_ direction of the train.

Step width as well as min and max speed values can be configured in _config.h_. You should interpret SPEED_NOMINAL_MAX as "max speed in forward direction" and SPEED_NOMINAL_MIN as "max speed in reverse direction". As the scale is intended to represent precentage, _MAX_ value should be 100, _MIN_ value -100. If your train can't run the same speed in both directions, you can change one of the values accordingly. It is possible to use values above/below 100/-100, but the recommendation is to stick with 100/-100 for the direction the train can run faster.

Each click of a button will send a message for _nominal speed_ and _nominal direction_ each to the MQTT broker.

#### Display

![Annotated picture of the displayed information](remote-control_display-annotated.jpg)

<table>
    <thead>
        <tr>
            <th>no.</th>
            <th>name</th>
            <th>descripton</th>
        </tr>
    </thead>
    <tbody>
        <tr>
            <td>1</td>
            <td>nominal speed caret</td>
            <td>Indicates the currently selected nominal speed sent to the train. If the caret overlaps the actual speed bar or limiter indicator, it will be displayed with inverse colors.</td>
        </tr>
        <tr>
            <td>2</td>
            <td>limiter indicator</td>
            <td>If SPEED_NOMINAL_MAX != abs(SPEED_NOMINAL_MIN), the limiter indicates the reduced maximum speed.</td>
        </tr>
        <tr>
            <td>3</td>
            <td>textual representation</td>
            <td>This line indicates the currently selected nominal values in textual form. "REV" or "FWD" indicate the current direction followed by the numbers for current and maximum nominal speed.</td>
        </tr>
        <tr>
            <td>4</td>
            <td>actual speed bar</td>
            <td>Indicates the actual speed received from the train.</td>
        </tr>
    </tbody>
</table>
