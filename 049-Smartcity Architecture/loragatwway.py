from network import LoRa
from network import WLAN
import socket
import time
t=0
WIFI_SSID  = 'your SSID'
WIFI_AUTH  = (WLAN.WPA2, 'ID')
# connect to WiFi
wifi = WLAN(mode=WLAN.STA)
wifi.connect(WIFI_SSID, auth=WIFI_AUTH)
while not wifi.isconnected():
    pass

print('IP address:', wifi.ifconfig()[0])


HOST = "api.thingspeak.com"
lora = LoRa(mode=LoRa.LORA, frequency=868000000)
#print (lora)
lora1 = socket.socket(socket.AF_LORA, socket.SOCK_RAW)
#print (s)
lora1.setblocking(False)
def thingspeak(apikey,d,h,t):
    data = b"api_key="+ API_KEY + "&field1=" + str(d) + "&field2=" + str(h)+ "&field3=" + str(t)
    s = socket.socket()
    ai = socket.getaddrinfo(HOST, 80)
    addr = ai[0][-1]
    s.connect(addr)
    s.write("POST /update HTTP/1.0\r\n")
    s.write("Host: " + HOST + "\r\n")
    s.write("Content-Length: " + str(len(data)) + "\r\n\r\n")
    s.write(data)
    print(s.read(128))
    s.close()


while True:
    #print ("HIEE")
    recieved_data =lora1.recv(80)
    #print(recieved_data)
    converted_data=str(recieved_data)

    if converted_data == "":
        print (" NO DATA")
    else:
        print("Hello")


        Data = converted_data.split('@')

        firstindex = str(Data[0])
        secondindex = str(Data[1])
        thirdindex = str(Data[2])
        #print ("FIRST INDEX :"+firstindex)
        #print ("SECOND INDEX :"+secondindex)
        #print ("THIRD INDEX :"+thirdindex)

        Data1=secondindex.split(',')
        print (Data1)

        if Data1[0] == '1':
            deviceId = Data1[0]
            airquality = Data1[1]
            carbon = Data1[2]
            print ("device id is :   "+deviceId)
            print ("air quality is :   "+airquality)
            print ("carbon content :   "+carbon)
            API_KEY = "74V0GL9OVR44JEVB"
            d = airquality
            h = carbon
            thingspeak( API_KEY,d,h,t)
        if Data1[0] == '2':
            deviceId = Data1[0]
            lat = Data1[1]
            lon = Data1[2]
            emergency = Data1[3]
            print ("device id is :   "+deviceId)
            print ("lat is :   "+lat)
            print ("lon :   "+lon)
            API_KEY = "O803BQV26W90ZR9Z"
            d =lat
            h = lon
            t = emergency
            thingspeak( API_KEY,d,h,t)
    recieved_data = False
    time.sleep(5)
