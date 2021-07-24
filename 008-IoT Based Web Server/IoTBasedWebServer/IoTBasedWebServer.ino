/*
 *  This sketch demonstrates how to set up a simple HTTP-like server.
 *  The server will set a GPIO pin depending on the request
 *    http://server_ip/gpio/0 will set the GPIO2 low,
 *    http://server_ip/gpio/1 will set the GPIO2 high
 *  server_ip is the IP address of the ESP8266 module, will be 
 *  printed to Serial when the module is connected.
 */

#include <ESP8266WiFi.h>


const char* ssid = "your SSID";
const char* password = "password";
int AC = D1;
int Light = D2;
String readString;

// Create an instance of the server
// specify the port to listen on as an argument
WiFiServer server(80);

void setup() {
  Serial.begin(115200);
  delay(10);
  

  // prepare GPIO5
  pinMode(D1, OUTPUT);
  digitalWrite(D1, 0);
   pinMode(D2, OUTPUT);
  digitalWrite(D2, 0);
  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  
  // Start the server
  server.begin();
  Serial.println("Server started");

  // Print the IP address
  Serial.println(WiFi.localIP());
}

void loop() {
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

        client.println("<h1>IoT Based Web Server</h1>");
        client.println("<h2>AIR CONDITIONER</h2>");
        client.println("<div class=\"row\">");
        client.println("<div class=\"col-md-2\"><a href=\"?pin=ON1\" class=\"btn btn-block btn-lg btn-success\" role=\"button\">ON</a></div>");
        client.println("<div class=\"col-md-2\"><a href=\"?pin=OFF1\" class=\"btn btn-block btn-lg btn-danger\" role=\"button\">OFF</a></div>");
        client.println("</div>");
        client.println("<h2>LIGHT</h2>");
        client.println("<div class=\"row\">");
        client.println("<div class=\"col-md-2\"><a href=\"?pin=ON2\" class=\"btn btn-block btn-lg btn-primary\" role=\"button\">ON</a></div>");
        client.println("<div class=\"col-md-2\"><a href=\"?pin=OFF2\" class=\"btn btn-block btn-lg btn-warning\" role=\"button\">OFF</a></div>");
        client.println("</div></div>");

           delay(1);
           //stopping client
           client.stop();
           //controls the Arduino if you press the buttons
           if (readString.indexOf("?pin=ON1") >0){
               digitalWrite(AC, HIGH);
           }
           else if (readString.indexOf("?pin=OFF1") >0){
               digitalWrite(AC, LOW);
           }
           else if (readString.indexOf("?pin=ON2") >0){
               digitalWrite(Light, HIGH);
           }
           else if (readString.indexOf("?pin=OFF2") >0){
               digitalWrite(Light, LOW);
           }
         
            //clearing string for next read
            readString="";  
           
         }
       }
    }
}
}
