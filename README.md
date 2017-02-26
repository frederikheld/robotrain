# robo-train
Remote controllable LEGO train.

## How to
This package is installed on a RasPi or other Linux machine. Start the server with
```
$ npm start
```
The train is controlled by an Arduino that is equipped with a motor shield. Communication between server and train via johnny-five. Therefore a USB connection between Server and Arduino is still needed, so right now the train isn't really remote controllable.

You can use any computer or smartphone/tablet as cockpit. It just has to be in the same network as the server. Just browser to the server's IP or hostname on port 4242. So for hostname "robotrain" type in your browser
```
http://robotrain:4242/
```
You can open as many cockpits as you want on different clients.

## Server is using npm packages:
    * express
    * johnny-five
    * serialport
    * serve-static
    * socket.io
See package.json for details!

## GUI is using js libraries:
    * jquery
    * d3.js
    * radial-progress-chart.js
