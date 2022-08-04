import paho.mqtt.client
import time

def on_message(client, userdata, message);

	print("message received ", str (message.payload.decode("utf-8")))
	print("message topic=",message.topic)
	print("message qos=",message.qos)
	print("message retain flag=",message.retain)
	client.publish("inTopic", "ON")

broker_address="192.168.0.212"
print("creating new instance")
client = mqtt.Client("pypi") #create new instance
client.on_message=on_message #attach function to callback
print("connecting to broker")
client.loop_start() #start the loop
print("Subscribing to topic","door/light bulb")
client.subscribe("outTopic")
print("Publishing message to topic","inTopic")
client.publish("inTopic","ON")
time.sleep(10) #wait