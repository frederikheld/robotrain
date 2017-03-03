# robo-train
Remote controllable LEGO train.

The train's heart is a RasPi that is connected to a local network via WiFi. This RasPi is equipped with a motor shield which is controlled via wiring-pi, a package with node bindings for WiringPi. It is also running a socket.io server that provides a web ui that can be accessed from all devices that are in the same network.

## Installation

Copy the whole folder to /home/pi/robotrain on your RasPi, then run
```
$ cd /home/pi/robotrain
$ sh install/setup_robotrain.sh
```
to install all required Debian packages.

After that install this node package by running in the same directory
```
$ npm install
```

## Usage

Start the server on the RasPi with
```
$ cd /home/pi/robotrain
$ sudo npm start
```
It's important to run the node package as sudo, since WiringPi can only write to GPIO as sudo.

You can use any computer or smartphone/tablet as cockpit. It just has to be in the same network as the RasPi.
Just browse to the server's IP or hostname on port ```4242```. So for hostname ```robotrain``` type in your browser
```
http://robotrain:4242/
```
You can open as many cockpits as you want on different clients.

## Server is using npm packages:
    * express
    * serve-static
    * socket.io
    * wiring-pi
See package.json for details!

## GUI is using js libraries:
    * jquery
    * chart.js
