'use strict'

//
//      CONFIG
//

// Config:
var config = {
    "hostname" : "robotrain",
    "socketio_port": 4242,
    "arduino_port": 4243
}


//
//      BOARD
//

var five = require("johnny-five");
var EtherPortClient = require("etherport-client").EtherPortClient;

// Create virtual serial port to communicate with Arduino via WiFi:
var serialport = new EtherPortClient({
    host: config.hostname,
    type: "udp4",
    port: config.arduino_port
});

// Connect to board via j5:
var board = new five.Board({
    port: serialport,
    repl: false
});

board.on("ready", function() {
    console.log("[j5] JohnnyFive ready");

    var led = new five.Led(2);
    led.blink(500);

}));
