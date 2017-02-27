'use strict'

console.log("Remember to run scripts, that use GPIO, as root!");

var five = require("johnny-five");
var rpi = require("raspi-io");

var board = new five.Board({
    io: new rpi()
});

board.on("ready", function() {

    var led = new five.Led(0);
    led.blink();

});
