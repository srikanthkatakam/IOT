import RPi.GPIO as GPIO
import time
import datetime
import urllib2

GPIO.setmode(GPIO.BCM)

TRIG = 23
ECHO = 24

print "Distance Mesurement In Progress"

GPIO.setup(TRIG, GPIO.OUT)
GPIO.setup(ECHO, GPIO.IN)

print "Waiting for Sensor Data"

api_key = "thingspeak write key"
base_url = "http://api.thingspeak.com/update?api_key=%s" % api_key

while True:
    GPIO.output(TRIG, False)
    
    time.sleep(2)

    GPIO.output(TRIG, True)
    time.sleep(0.00001)
    GPIO.output(TRIG, False)

    while GPIO.input(ECHO)==0:
        pulse_start = time.time()

    while GPIO.input(ECHO)==1:
        pulse_end = time.time()


    pulse_duration = pulse_end - pulse_start

    # 34300=Distance/Time/2,    17150=Distance/Time,   17150 X Time = Distance (cm)

    distance = pulse_duration * 17150
    distance1 = round(distance,0)
    print("Last valid input: " + str(datetime.datetime.now()))
    print("Distance: %d C" % distance)
    try:
        url = base_url + "&field1=%s" % (distance1)
        print(url)
        f = urllib2.urlopen(url)
        print f.read()
        f.close()           
    except:
        print("error")
        pass
    time.sleep(600)     
