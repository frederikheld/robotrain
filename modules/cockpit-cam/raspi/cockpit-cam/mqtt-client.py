import paho.mqtt.client as mqtt
import subprocess

def on_connect(client, userdata, flags, rc):
	print("Connected with result code " + str(rc))
	client.subscribe("robotrain/cockpit-cam/request-stream")

def on_message(client, userdata, msg):

	print("1:")
	print(str(msg.payload))
	
	print("2:")
	print(msg.payload)
	# print(str(msg.payload).decode('ASCII')) # TODO: This doesn't work!

#	if msg.topic == "robotrain/cockpit-cam/request-stream":
	subprocess.run(["sh", "start-stream.sh", msg.payload])
	# subprocess.run(["sh", "hello-world.sh"])

mqttClient = mqtt.Client("robotrain-cockpit-cam")
mqttClient.on_connect = on_connect
mqttClient.on_message = on_message

mqttClient.connect("serenity", 1883)
print("Connected.")

mqttClient.publish("robotrain/test/foo", "bar")

print("Looping forever...")
mqttClient.loop_forever()
