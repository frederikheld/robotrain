'use strict'

const mqtt = require("mqtt");

const mqttClient = mqtt.connect("mqtt://robotrain:1883");

mqttClient.on("connect", async () => {
  await mqttClient.subscribe("robotrain/#");
});

mqttClient.on("message", (topic, message) => {
  let timestamp = new Date().toISOString();

  console.log(timestamp + " - " + topic.toString() + ": " + message.toString());
});
