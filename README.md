# robo-train
Remote controllable LEGO train.

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

## Background

The train is controlled by an Arduino that is equipped with a motor shield. Communication between server and train via johnny-five. Therefore a USB connection between Server and Arduino is still needed, so right now the train isn't really remote controllable.

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
