import RPi.GPIO as GPIO
import dht11
import time
import datetime
import MySQLdb as mdb
import httplib, os, glob
import urllib, urllib2


#import logging
# initialize GPIO
GPIO.setwarnings(False)
GPIO.setmode(GPIO.BCM)
GPIO.cleanup()





global DeviceId
global Datetime
# Set each pin as an output and make it low:
GPIO.setup(5, GPIO.OUT)
GPIO.output(5, GPIO.LOW)
GPIO.setup(6, GPIO.OUT)
GPIO.output(5, GPIO.LOW)

# read data using pin 18
instance = dht11.DHT11(pin = 14)
DeviceId=11084

# read data using pin 14

instance = dht11.DHT11(pin=14)
def insertDB(Temperature,Humidity,Datetime,DeviceId):
    try:
        con=mdb.connect('localhost','root','root','multiprotocol');
        cursor = con.cursor()
        sql = "INSERT INTO multiprotocol(Temperature,Humidity,Datetime,DeviceId)\
                VALUES ('%s','%s','%s','%s')"%\
                (Temperature,Humidity,Datetime,DeviceId)
        cursor.execute(sql)
        print ('Inserted')
        sql = []
        con.commit()
        con.close()
    except:
        time.sleep(1)
        print('Not Inserted')

def homecontrol():
    url = "url"
    response = urllib.urlopen(url).read()
    pinstatus = response
    print pinstatus
  
    if pinstatus == "LIGHTON":
        GPIO.output(5,GPIO.HIGH)
        print "light is on"
        
    if pinstatus == "LIGHTOFF":
        GPIO.output(5,GPIO.LOW)
        print "light is off"
            
    if pinstatus == "FANON":
        GPIO.output(6,GPIO.HIGH)
        print "fan is on"

    if pinstatus == "FANOFF":
        GPIO.output(6,GPIO.LOW)
        print "fanoff is off"



while True:
    		       
    result = instance.read()
    if result.is_valid():
        Temperature = result.temperature
        Humidity = result.humidity
        Datetime = datetime.datetime.now()
        print("Last valid input: " + str(datetime.datetime.now()))
        print("Temperature: %f C" % result.temperature)
        print("Humidity: %f %%" % result.humidity)
        
        insertDB(Temperature,Humidity,Datetime,DeviceId)
        params = urllib.urlencode({'field1': Temperature,'field2': Humidity,
                                   'key': 'LYGLWJE7UJXX72OG'})
        headers = {"content-type": "application/x-www-form-urlencoded","Accept":
                   "text/plain"}
        conn = httplib.HTTPConnection("api.thingspeak.com:80")
        conn.request("POST", "/update", params, headers)
        response = conn.getresponse()
        print response.status, response.reason
        data = response.read()
        conn.close()
    homecontrol()
    time.sleep(10)
