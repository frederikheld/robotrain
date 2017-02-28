'use strict'

// console.log("Remember to run scripts, that use GPIO, as root!");


//
//      CONFIG
//

// Config:
var config = {
    "hostname" : "robotrain",
    "socketio_port": 4242
}


//
//      RASPI
//

var wpi = require("wiring-pi");
// var async = require("async");

wpi.setup('wpi'); // Use wiringpi pinning

const pin_pwm = 1;
const pin_dir = 2;
const pin_brk = 7;
wpi.pinMode(pin_pwm, wpi.PWM_OUTPUT); // min = 0, max = 1023
wpi.pinMode(pin_dir, wpi.OUTPUT);
wpi.pinMode(pin_brk, wpi.OUTPUT);

// wpi.digitalWrite(pin_brk, 0);
wpi.digitalWrite(pin_dir, 0);

var pwm_value = 20;

// async.series([
//     functin(cb) {
//         wpi.pwmWrite(pin, value);
//         cb();
//     },
//
// ]);

setInterval(function() {
    if (pwm_value <= 1023) {
        pwm_value += 20;
    } else {
        pwm_value = 0;
    }
    wpi.pwmWrite(pin_pwm, pwm_value);
    console.log("pwm_value = " + pwm_value);

}, 500);

// const raspi = require("raspi");
// const gpio = require("raspi-gpio");
//
// raspi.init(() => {
//     const led = new gpio.DigitalOutput(11);
//
//     function ledOn() {
//         console.log("Switching on LED");
//         led.write(1);
//     }
//
//     function ledOff() {
//         console.log("Switching off LED");
//         led.write(0);
//     }
//
//     ledOn();
//     setTimeout(ledOff, 500000);
//
// });
