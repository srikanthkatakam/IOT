from AWSIoTPythonSDK.MQTTLib import AWSIoTMQTTClient
import RPi.GPIO as GPIO
import dht11Lib
import time
import json

host = "a9y0es71v6h6l.iot.us-east-1.amazonaws.com"
rootCAPath = "/home/pi/Upload DHT-11 Data to DynamoDB/rootca.pem.txt"
certificatePath = "/home/pi/Upload DHT-11 Data to DynamoDB/d1d3f32db9-certificate.pem.crt"
privateKeyPath = "/home/pi/Upload DHT-11 Data to DynamoDB/d1d3f32db9-private.pem.key"
topic = "topic"

# initialize GPIO
GPIO.setwarnings(False)
GPIO.setmode(GPIO.BOARD)
GPIO.cleanup()

# read data using pin 8
instance = dht11Lib.DHT11(pin=40)


# Custom MQTT message callback
def customCallback(client, userdata, message):
	print("Received a new message: ")
	print(message.payload)
	print("from topic: ")
	print(message.topic)
	print("--------------\n\n")
	

# Init AWSIoTMQTTClient
myAWSIoTMQTTClient = None
myAWSIoTMQTTClient = AWSIoTMQTTClient("RaspberryPi")
myAWSIoTMQTTClient.configureEndpoint(host, 8883)
myAWSIoTMQTTClient.configureCredentials(rootCAPath, privateKeyPath, certificatePath)


# AWSIoTMQTTClient connection configuration
myAWSIoTMQTTClient.configureAutoReconnectBackoffTime(1, 32, 20)
myAWSIoTMQTTClient.configureOfflinePublishQueueing(-1)  # Infinite offline Publish queueing
myAWSIoTMQTTClient.configureDrainingFrequency(2)  # Draining: 2 Hz
myAWSIoTMQTTClient.configureConnectDisconnectTimeout(10)  # 10 sec
myAWSIoTMQTTClient.configureMQTTOperationTimeout(5)  # 5 sec

# Connect and subscribe to AWS IoT
myAWSIoTMQTTClient.connect()
myAWSIoTMQTTClient.subscribe(topic, 1, customCallback)
time.sleep(2)

# Publish to the same topic in a loop forever
loopCount = 0

while True:
        result = instance.read()
        if result.is_valid():
            JSONPayload = {'Deviceid':"W0001",'temperature': result.temperature, 'humidity': result.humidity, 'sno': loopCount}
            print json.dumps(JSONPayload)
            myAWSIoTMQTTClient.publish(topic,json.dumps(JSONPayload), 1)
            loopCount += 1
            time.sleep(5)
        else:
                print "error"
        

   

    




