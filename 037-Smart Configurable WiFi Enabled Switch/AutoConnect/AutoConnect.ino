#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino

//needed for library
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>         //https://github.com/tzapu/WiFiManager
int led1 = D1;
int led2 = D2;
String readString;

WiFiServer server(80);
void setup() {
    // put your setup code here, to run once:
    Serial.begin(115200);
    //WiFiManager
    //Local intialization. Once its business is done, there is no need to keep it around
    WiFiManager wifiManager;
    //reset saved settings
    wifiManager.resetSettings();
    
    //set custom ip for portal
    //wifiManager.setAPConfig(IPAddress(10,0,1,1), IPAddress(10,0,1,1), IPAddress(255,255,255,0));

    //fetches ssid and pass from eeprom and tries to connect
    //if it does not connect it starts an access point with the specified name
    //here  "AutoConnectAP"
    //and goes into a blocking loop awaiting configuration
    wifiManager.autoConnect("Sandeep");
    //or use this for auto generated name ESP + ChipID
    //wifiManager.autoConnect();

    
    //if you get here you have connected to the WiFi
    Serial.println("connected...yeey :)");
    Serial.println(WiFi.localIP());

     // prepare GPIO5
  pinMode(D1, OUTPUT);
  digitalWrite(D1, 0);
   pinMode(D2, OUTPUT);
  digitalWrite(D2, 0);

   server.begin();
  Serial.println("Server started");

  // Print the IP address
  Serial.println(WiFi.localIP());
}

void loop() {
    // put your main code here, to run repeatedly:
 // Check if a client has connected
  WiFiClient client = server.available();
 if (client) {
    while (client.connected()) {   
      if (client.available()) {
        char c = client.read();
     
        //read char by char HTTP request
        if (readString.length() < 100) {
          //store characters to string
          readString += c;
          //Serial.print(c);
         }

         //if HTTP request has ended
         if (c == '\n') {          
        Serial.println(readString); //print to serial monitor for debuging
        client.println("<head>");
        client.println("<meta charset=\"utf-8\">");
        client.println("<meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\">");
        client.println("<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
        client.println("<script src=\"https://code.jquery.com/jquery-2.1.3.min.js\"></script>");
        client.println("<link rel=\"stylesheet\" href=\"https://maxcdn.bootstrapcdn.com/bootstrap/3.3.5/css/bootstrap.min.css\">");
        client.println("</head><div class=\"container\">");

        client.println("<h1>Web Server</h1>");
        client.println("<h2>GPIO 0</h2>");
        client.println("<div class=\"row\">");
        client.println("<div class=\"col-md-2\"><a href=\"?pin=ON1\" class=\"btn btn-block btn-lg btn-success\" role=\"button\">ON</a></div>");
        client.println("<div class=\"col-md-2\"><a href=\"?pin=OFF1\" class=\"btn btn-block btn-lg btn-danger\" role=\"button\">OFF</a></div>");
        client.println("</div>");
        client.println("<h2>GPIO 2</h2>");
        client.println("<div class=\"row\">");
        client.println("<div class=\"col-md-2\"><a href=\"?pin=ON2\" class=\"btn btn-block btn-lg btn-primary\" role=\"button\">ON</a></div>");
        client.println("<div class=\"col-md-2\"><a href=\"?pin=OFF2\" class=\"btn btn-block btn-lg btn-warning\" role=\"button\">OFF</a></div>");
        client.println("</div></div>");

           delay(1);
           //stopping client
           //controls the Arduino if you press the buttons
           if (readString.indexOf("?pin=ON1") >0){
               digitalWrite(led1, HIGH);
           }
           else if (readString.indexOf("?pin=OFF1") >0){
               digitalWrite(led1, LOW);
           }
           else if (readString.indexOf("?pin=ON2") >0){
               digitalWrite(led2, HIGH);
           }
           else if (readString.indexOf("?pin=OFF2") >0){
               digitalWrite(led2, LOW);
           }
         
            //clearing string for next read
            readString="";  
           
         }
       }
    }
}
 delay(1);
    // close the connection:
    client.stop();
    
    
}
