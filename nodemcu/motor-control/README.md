# Motor control for _robotrain_

Motor control allows to command the _robotrain_ via Wifi. Command are sent and received via MQTT. Details below.

![Wiring diagram of motor-control](motor-control_bb.png)

This is an export from the [Fritzing file](motor-control.fzz) in this directory.

The black wires are ground (GND). The other wires are explained n the sections below.

The motor control has several features that are described in detail below.

## 12 V propulsion supply system

The original LEGOⓇ train system is powered by 0..9 V direct current that is put on the tracks and collected by the wheels of the drive units. The collectors and motor are directly connected, so the voltage on the tracks directly determines how fast the train is running.

I have modified the original drive units to lead the current out of the chassis through one pair of cables and take the motor power in through another pair of cables. There's no direct electric connection between the wheels and the motor anymore.

This allows to provide constant voltage on the tracks and also allows to run several trains independently on the same tracks by controlling the motor power from within each train.

I have also raised the voltage provided by the tracks from the original 9 V to 12 V. This is possible because the motors run at up to 12 V and it also compensates for voltage drops due to additional consumer loads.

### Recitifiers

The current picked up from the tracks is rectified because it shouldn't matter in which direction the train is put on the tracks. This also allows to build looped tracks.

A train can have multiple drive units. It is important that each drive unit has their own rectifier!

### Uninterruptible Power Supply

The train carries a 12 V Uninterruptible Power Supply (UPS) unit that serves two purposes:

A) It compensates for possible voltage drops if a section has reduced conductivity (like soiled tracks or railroad switches).

B) It allows to run the train on tracks that are not powered at all. This helps to reduce cost for the whole layout, as the original powered tracks are not sold anymore and prices on eBay are high. But it is possible to combine powered tracks with the cheaper non-powered tracks that were introduced by LEGOⓇ together with the battery powered train system. They have exactly the same structural shape and can be used interchangeably.

I'm using [revolt Mini-USV, unterbrechungsfreie DC-Stromversorgung, 12 V, 22 W, 2000 mAh](https://www.revolt-power.de/Mini-UPS-fuer-Router-PX-1931-919.shtml) UPS. It fits into [LEGOⓇ 4547 Club Car](https://www.bricklink.com/v2/catalog/catalogitem.page?S=4547-1&name=Club%20Car&category=%5BTrain%5D%5B9V%5D#T=S&O={%22iconly%22:0}) if the interior is removed. To connect the wiring I'm using [Delock Adapter DC 5,5 x 2,5 mm Stecker > Terminalblock 2 Pin](https://www.delock.de/produkt/65487/merkmale.html) connectors.

### Wiring

The rectified 12 V direct current system is indicated by the yellow and black wires in the diagram.

The un-rectified 12 V direct current sections are indicated by the brown wires in the diagram.

If now UPS is used, the yellow and black wires can be connected with their counterpart of the same color.

## 5 V logic supply system

A DC-DC 12 V to 5 V converter provides 5 V as a power supply for the logic level.

The 5 V system is indicated by the black and red wires in the diagram.

## Connectivity and computing

The connectivity and computing unit of _robotrain_ is the NodeMCU Mini. It is basically a little breakout board with USB port and reset button for the _ESP8266 12-F_ module. ESP8266 is an Arduino-compatible microprocessor with integrated WiFi.

Like the Arduino it provides digital and analog input and output pins. A sketch can be uploaded via the USB port like with the Arduino.

## Motor Control

The motor control board is based on the popular _L298N_ motor driver. The board itself is very popular as well. There's a very good description how this board works at [lastminuteengineers.com](https://lastminuteengineers.com/l298n-dc-stepper-driver-arduino-tutorial/).

The motor control takes 12 V propulsion power and 5 V logic power. It is also connected to 2 analog output pins on the _NodeMCU Mini_ that provide the PWM signal that determine rotation speed and direction of the motors. As both motors should rotate in the same direction with the same speed, the input for both motors are connected to the same input signal.

Each output is connected to the pair of input cables of the respective drive unit.

