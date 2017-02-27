# robo-train
Remote controllable LEGO train.

The train's heart is a RasPi that is connected to a local network via WiFi. This RasPi is equipped with a motor shield which is controlled via johnny-five. It is also running a socket.io server that provides a web ui that can be accessed from all devices that are in the same network.

## Installation

Load ```StandardFirmata.ino``` from the examples in the standard Arduino IDE onto your Arduino.

Copy the whole folder to /home/pi/robotrain on your RasPi, then run
```
$ cd /home/pi/robotrain
$ sh install/setup_robotrain.sh
```
to install all needed packages.

After that install this node package by running in the same directory
```
$ npm install
```

## Usage

Start the server on the RasPi with
```
$ cd /home/pi/robotrain
$ npm start
```

You can use any computer or smartphone/tablet as cockpit. It just has to be in the same network as the RasPi.
Just browse to the server's IP or hostname on port ```4242```. So for hostname ```robotrain``` type in your browser
```
http://robotrain:4242/
```
You can open as many cockpits as you want on different clients.

## Server is using npm packages:
    * express
    * johnny-five
    * raspi-io
    * serve-static
    * socket.io
See package.json for details!

## GUI is using js libraries:
    * jquery
    * d3.js
    * radial-progress-chart.js
