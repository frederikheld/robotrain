'use strict'

// imports:

const mqtt = require("mqtt")
const fs = require('fs')
const config = JSON.parse(fs.readFileSync('config.json', 'utf8'))


// read command line parameters:

if (!process.argv[2]) {
    console.error('ERROR: Missing first command line parameter "topic". Run "npm start help" for more information!')
    return
}
const mqttTopic = process.argv[2]

if (!process.argv[3]) {
    if (process.argv[2] == "help") {
        console.log('Usage: "npm start <topic> <message>"')
        return
    } else {    
        console.error('ERROR: Missing second command line parameter "message". Run "npm start help" for more information!')
        return
    }
}
const message = process.argv[3]


// main:

const mqttClient = mqtt.connect("mqtt://" + config.server + ":" + config.port)

mqttClient.on("connect", async () => {
  await mqttClient.publish(mqttTopic, message)
  console.log('Published "' + message + '" to topic "' + mqttTopic + '"')
  mqttClient.end()
})

