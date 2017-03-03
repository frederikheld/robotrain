'use strict'

//
//      CONFIG
//

// Config:
var config = {
    gpio: {
        motor: {
            pwm: 1,
            dir: 2,
            brk: 7
        },
        ui: {
            btn_up: 24,
            btn_down: 25,
            led_heartbeat: 27
        }
    },
    socketio: {
        port: 4242
    },
    pwm: {
        min: 0,
        max: 1023
    },
    board: {
        btn_up_step: 50,
        btn_down_step: -50
    }
}


//
//      DATA HANDLING
//

// Store data:
var speed = 0; // Current speed (aka pwm value)
var client_id = 0; // Running number that is assigned to the clients

// Manipulate data:
function updateSpeed(currSpeed, addSpeed) {

    // Handle positive and negative speeds different:
    if (addSpeed > 0) {
        if ((currSpeed + addSpeed) > config.pwm.max) {
            currSpeed = config.pwm.max; // fill up to max
        } else {
            currSpeed += addSpeed;
        }
    } else if (addSpeed < 0) {
        if ((currSpeed + addSpeed) < config.pwm.min) {
            currSpeed = config.pwm.min; // empty down to min
        } else {
            currSpeed += addSpeed;
        }
    }

    return currSpeed;

}


//
//      SOCKET.IO SERVER
//
//      socket.io server to receive events from
//      and update all connected clients
//

// Emit new speed to all clients:
function emitSpeed(speed) {
    console.log(Date.now() + " [socket.io] Speed updated: " + speed);
    io.sockets.emit("speed_update", speed);
}

// Create socket.io server:
var express = require("express");
var path = require("path");
var app = express();
var server = app.listen(config.socketio.port, function() {
    console.log(Date.now() + " [socket.io] LISTENING on port " + config.socketio.port);
});
var io = require("socket.io").listen(server);

// Make directory "/gui" public available:
app.use(express.static(path.join(__dirname, "gui")));

// Serve "index.html":
app.get("/", function(req, res) {
    res.sendFile(path.join(__dirname, "index.html"));
});

// Communicate with clients:
io.sockets.on("connection", function(socket) {

    // Init gui:
    console.log(Date.now() + " [socket.io] Another client has connected.");
    console.log(Date.now() + " [socket.io] SENDING \"welcome\" event to new client.");
    socket.emit("welcome", {
        "message": "Hello World!",
        "client_id": client_id,
        "speed_max": config.pwm.max
    });
    client_id++;

    // Init values:
    emitSpeed(speed);

    // Listen to events from client:
    socket.on("user_agent", function (data) {
        console.log(Date.now() + " [socket.io] RECEIVING \"user agent\" event from client: " + data);
    });

    socket.on("gui_speed_update", function(data) {
        console.log(Date.now() + " [socket.io] RECEIVING \"gui_speed_update\" event from client: " + data);
        speed = updateSpeed(speed, data);
        emitSpeed(speed);
        updatePwm(speed);
    });

});


//
//      RASPI GPIO
//

console.log("DEBUG: Starting to init GPIO.");

const wpi = require("wiring-pi");
wpi.setup('wpi'); // Use wiringpi pin naming

function updatePwm(speed) {
    console.log(Date.now() + " [gpio] speed = " + speed);
    wpi.pwmWrite(config.gpio.motor.pwm, speed);
}

// Init buttons:
wpi.pinMode(config.gpio.ui.btn_down, wpi.INPUT);
wpi.pinMode(config.gpio.ui.btn_up, wpi.INPUT);

// Init motor shield:
wpi.pinMode(config.gpio.motor.pwm, wpi.PWM_OUTPUT); // min = 0, max = 1023
wpi.pinMode(config.gpio.motor.dir, wpi.OUTPUT);
wpi.pinMode(config.gpio.motor.brk, wpi.OUTPUT);

// Heartbeat:
wpi.pinMode(config.gpio.ui.led_heartbeat, wpi.OUTPUT);
var heartbeat_state = 0;

// Set initial speeds:
wpi.digitalWrite(config.gpio.motor.dir, 1);
updatePwm(speed);

// Check gpio every 50 ms:
setInterval(function() {

    // Update heartbeat:
    wpi.digitalWrite(config.gpio.ui.led_heartbeat, heartbeat_state);
    heartbeat_state = 1 - heartbeat_state;

    if (1 == wpi.digitalRead(config.gpio.ui.btn_down)) {

        speed = updateSpeed(speed, config.board.btn_down_step);
        emitSpeed(speed);
        updatePwm(speed);

        // lock button until release:
        while (0 != wpi.digitalRead(config.gpio.ui.btn_down));
        // TODO: Might be blocking! Implement non-blocking solution

    } else if (1 == wpi.digitalRead(config.gpio.ui.btn_up)) {

        speed = updateSpeed(speed, config.board.btn_up_step);
        emitSpeed(speed);
        updatePwm(speed);

        // lock button until release:
        while (0 != wpi.digitalRead(config.gpio.ui.btn_up));
        // TODO: Might be blocking! Implement non-blocking solution
    //
    }

}, 50);
