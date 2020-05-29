'use strict'

// imports:

const mqtt = require("mqtt")
const fs = require('fs')
const config = JSON.parse(fs.readFileSync('config.json', 'utf8'))


// read command line parameters:

if (!process.argv[2]) {
  console.error('ERROR: Missing command line parameter "topic". Usage: "$ npm start <topic>".')
  return
}
const mqttTopic = process.argv[2]


// main:

const mqttClient = mqtt.connect("mqtt://" + config.server + ":" + config.port)

mqttClient.on("connect", async () => {
  await mqttClient.subscribe(mqttTopic)
})

mqttClient.on("message", (topic, message) => {
  let timestamp = new Date().toISOString()

  console.log(timestamp + " - " + topic.toString() + ": " + message.toString())
})
