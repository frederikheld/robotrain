'use strict'

console.log("Remember to run scripts, that use GPIO, as root!");

var five = require("johnny-five");
var raspi = require("raspi-io");

var board = new five.Board({
    io: new raspi()
});

board.on("ready", function() {

    var led = new five.Led(0);
    led.blink();

    var motor = new five.Motor({
        pins: {
            pwm: "P1-35",
            dir: "P1-13"
        },
        invertPWM: true
    });

    board.repl.inject({
        l: motor
    });

    // Event is triggered when function motor.forward() was called:
    motor.on("forward", function(err, timestamp) {
        console.log("EVENT: motor.forward() was called");
        // Break after 5 seconds:
        board.wait(5000, function() {
            console.log("calling motor.reverse(255)");
            motor.reverse(255);
        });
    });

    // Event is triggered when function motor.reverse() was called:
    motor.on("reverse", function(err, timestamp) {
        console.log("EVENT: motor.reverse() was called");
        board.wait(5000, function() {
            console.log("calling motor.forward(255)");
            motor.forward(255);
        })
    });

    // Event is triggered when function motor.brake() was called:
    motor.on("brake", function(err, timestamp) {
        console.log("EVENT: motor.brake() was called");
        // Release brake after 0.1 seconds:
        board.wait(100, function() {
            console.log("calling motor.stop()");
            motor.stop();
        });
    });

    // Start motor at max speed:
    console.log("calling motor.forward(255)");
    motor.forward(255);

});
