import RPi.GPIO as GPIO
import dht11
import time
import datetime
import urllib, urllib2

# initialize GPIO
GPIO.setwarnings(False)
GPIO.setmode(GPIO.BCM)


# Set each pin as an output and make it low:
GPIO.setup(5, GPIO.OUT)
GPIO.output(5, GPIO.LOW)
GPIO.setup(6, GPIO.OUT)
GPIO.output(6, GPIO.LOW)
GPIO.setup(13, GPIO.OUT)
GPIO.output(13, GPIO.LOW)
GPIO.setup(19, GPIO.OUT)
GPIO.output(19, GPIO.LOW)

# read data using pin 18
instance = dht11.DHT11(pin = 14)

#api_key = "TODO: ---Thingspeak API KEY---"
base_url = ""
userkey=Key

def homecontrol():
    url = "url"
    response = urllib.urlopen(url).read()
    pinstatus = response
    print pinstatus
  
    if pinstatus == "LIGHTON":
        GPIO.output(5,GPIO.HIGH)
        
    if pinstatus == "LIGHTOFF":
        GPIO.output(5,GPIO.LOW)
            
    if pinstatus == "FANON":
        GPIO.output(6,GPIO.HIGH)    

    if pinstatus == "FANOFF":
        GPIO.output(6,GPIO.LOW)

    if pinstatus == "ACON":
        GPIO.output(13,GPIO.HIGH)

    if pinstatus == "ACOFF":
        GPIO.output(13,GPIO.LOW)

    if pinstatus == "TVON":
        GPIO.output(19,GPIO.HIGH)

    if pinstatus == "TVOFF":
        GPIO.output(19,GPIO.LOW)
             

while True:
    result = instance.read()
    if result.is_valid():
        print("Last valid input: " + str(datetime.datetime.now()))
        print("Temperature: %d C" % result.temperature)
        print("Humidity: %d %%" % result.humidity)
        try:
            url = base_url + "field1=%s&field2=%s&field3=%s" % (result.temperature, result.humidity, userkey)
            print(url)
            f = urllib2.urlopen(url)
            print f.read()
            f.close()           
        except:
            print("error")
            pass
    homecontrol()
    time.sleep(2)
    
