import RPi.GPIO as GPIO
import dht11
import time
import datetime
import urllib
import MySQLdb as mdb
import httplib, os, glob


#import logging
# initialize GPIO
GPIO.setwarnings(False)
GPIO.setmode(GPIO.BCM)
GPIO.cleanup()



GPIO.setup(2, GPIO.IN)


global DeviceId
global Datetime
global Moisture

DeviceId=11084

# read data using pin 14

instance = dht11.DHT11(pin=14)
def insertDB(Temperature,Humidity,Moisture,Datetime,DeviceId):
    try:
        con=mdb.connect('localhost','root','edge','Edgegateway');
        cursor = con.cursor()
        sql = "INSERT INTO Greenhouse(Temperature,Humidity,Moisture,Datetime,DeviceId)\
                VALUES ('%s','%s','%s','%s','%s')"%\
                (Temperature,Humidity,Moisture,Datetime,DeviceId)
        cursor.execute(sql)
        print ('Inserted')
        sql = []
        con.commit()
        con.close()
    except:
        time.sleep(1)
        print('Not Inserted')


while True:
    
    Moisture = GPIO.input(2)
    #print Moisture
    if GPIO.input(2):
	    print "LED off"	
		
    else:
	    print "LED on"
		       
    result = instance.read()
    if result.is_valid():
        Temperature = result.temperature
        Humidity = result.humidity
        Datetime = datetime.datetime.now()
        print("Last valid input: " + str(datetime.datetime.now()))
        print("Temperature: %f C" % result.temperature)
        print("Humidity: %f %%" % result.humidity)
        print("Moisture : " + str(Moisture))
        
        insertDB(Temperature,Humidity,Moisture,Datetime,DeviceId)
        params = urllib.urlencode({'field1': Temperature,'field2': Humidity, 'field3': Moisture,
                                   'key': '54XKFZJWZG0WTV5O'})
        headers = {"content-type": "application/x-www-form-urlencoded","Accept":
                   "text/plain"}
        conn = httplib.HTTPConnection("api.thingspeak.com:80")
        conn.request("POST", "/update", params, headers)
        response = conn.getresponse()
        print response.status, response.reason
        data = response.read()
        conn.close()
        

    time.sleep(10)
