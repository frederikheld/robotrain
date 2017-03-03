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

const wpi = require("wiring-pi");
// var async = require("async");

wpi.setup('wpi'); // Use wiringpi pin naming

// Pinning:
const pin_pwm = 1;
const pin_dir = 2;
const pin_brk = 7;

const pin_btn_dn = 25;
const pin_btn_up = 24;

// Settings:
const pwm_step = 50;

// Init buttons:
wpi.pinMode(pin_btn_dn, wpi.INPUT);
wpi.pinMode(pin_btn_up, wpi.INPUT);

// Init motor shield:
wpi.pinMode(pin_pwm, wpi.PWM_OUTPUT); // min = 0, max = 1023
wpi.pinMode(pin_dir, wpi.OUTPUT);
wpi.pinMode(pin_brk, wpi.OUTPUT);

// wpi.digitalWrite(pin_brk, 0);
wpi.digitalWrite(pin_dir, 0);

var pwm_value = 0;
console.log("pwm_value = " + pwm_value);

setInterval(function() {

    if (1 == wpi.digitalRead(pin_btn_dn)) {

        if ((pwm_value - pwm_step) >= 0) {
            pwm_value -= pwm_step;
        } else {
            pwm_value = 0; // empty down to min
        }

        console.log("pwm_value = " + pwm_value);
        wpi.pwmWrite(pin_pwm, pwm_value);

        // lock button until release:
        while (0 != wpi.digitalRead(pin_btn_dn));
        // TODO: Might be blocking! Implement non-blocking solution

    } else if (1 == wpi.digitalRead(pin_btn_up)) {
        if ((pwm_value + pwm_step) <= 1023) {
            pwm_value += pwm_step;
        } else {
            pwm_value = 1023; // fill up to max
        }

        console.log("pwm_value = " + pwm_value);
        wpi.pwmWrite(pin_pwm, pwm_value);

        // lock button until release:
        while (0 != wpi.digitalRead(pin_btn_up));
        // TODO: Might be blocking! Implement non-blocking solution

    }

}, 50);

// async.series([
//     functin(cb) {
//         wpi.pwmWrite(pin, value);
//         cb();
//     },
//
// ]);
