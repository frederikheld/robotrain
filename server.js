'use strict'

//
//      CONFIG
//

// Config:
var config = {
    "hostname" : "robotrain",
    "socketio_port": 4242
}


//
//      DATA HANDLING
//

// Store data:
var value = 0; // Current value // TODO: Replace with "speed"
var client_id = 0; // Running number that is assigned to the clients

// Manipulate data:
function updateValue(currValue, updateValue) {

    // Handle positive and negative values different:
    if (updateValue > 0) {
        if (currValue <= (100 - updateValue)) {
            currValue += updateValue;
        }
    } else if (updateValue < 0) {
        if (currValue >= -updateValue) {
            currValue += updateValue;
        }
    }
    return currValue;

}


//
//      FUNCTIONS
//

// Emit new value to all clients:
function emitValue(value) {
    console.log("[socket.io] Value updated: " + value);
    io.sockets.emit("value_update", value);
}


//
//      SOCKET.IO SERVER
//
//      socket.io server to receive events from
//      and update all connected clients
//

// Create socket.io server:
var express = require("express");
var path = require("path");
var app = express();
var server = app.listen(config.socketio_port, function() {
    console.log(Date.now() + " [socket.io] LISTENING on port " + config.socketio_port);
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

    // Send welcome message and initial value to new client:
    console.log(Date.now() + " [socket.io] Another client has connected.");
    console.log(Date.now() + " [socket.io] SENDING \"welcome\" event to new client.");
    socket.emit("welcome", {
        "message": "Hello World!",
        "client_id": client_id
    });
    client_id++;
    console.log(Date.now() + " [socket.io] SENDING \"value_update\" event to new client: " + value);
    socket.emit("value_update", value);

    // Listen to events from client:
    socket.on("user_agent", function (data) {
        console.log(Date.now() + " [socket.io] RECEIVING \"user agent\" event from client: " + data);
    });

    socket.on("gui_value_update", function(data) {
        console.log(Date.now() + " [socket.io] RECEIVING \"gui_value_update\" event from client: " + data);
        value = updateValue(value, data);
        emitValue(value);
    });

});


//
//      BOARD
//

function emitValueUpdate(value) {
    console.log("[j5] Value updated: " + value);
    io.sockets.emit("value_update", value);
}

// Connect to board via j5:
var five = require("johnny-five");
var board = new five.Board();

// Set up board features:
board.on("ready", function() {
    console.log("[j5] JohnnyFive ready");


    var buttonPlus = new five.Button("7");
    var buttonMinus = new five.Button("6");

    buttonPlus.on("press", function() {
        // wait for release
    }).on("release", function() {
        value = updateValue(value, 10);
        emitValueUpdate(value);
    });

    buttonMinus.on("press", function() {
        // wait for release
    }).on("release", function() {
       value = updateValue(value, -10);
       emitValueUpdate(value);
    });

});
