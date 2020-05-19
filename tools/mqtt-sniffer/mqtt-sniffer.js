'use strict'

// imports:

const mqtt = require("mqtt")
const fs = require('fs')
const config = JSON.parse(fs.readFileSync('config.json', 'utf8'))

console.log(config)


// main:

const mqttClient = mqtt.connect("mqtt://" + config.server + ":" + config.port)

mqttClient.on("connect", async () => {
  await mqttClient.subscribe(config.topic)
})

mqttClient.on("message", (topic, message) => {
  let timestamp = new Date().toISOString()

  console.log(timestamp + " - " + topic.toString() + ": " + message.toString())
})
