// Smart Home Automation with web application control
#include <ESP8266WiFi.h>
int LIGHT = D2;
int FAN   = D3;
int AC    = D4;

String readString;

const char* ssid     = "your SSID";
const char* password = "password";

// Create an instance of the server
// specify the port to listen on as an argument
WiFiServer server(80);
  
void setup() 
{
Serial.begin(9600);
delay(10);
  pinMode(D2, OUTPUT);
  digitalWrite(D2, 1);
   pinMode(D3, OUTPUT);
  digitalWrite(D3, 1);
     pinMode(D4, OUTPUT);
  digitalWrite(D4, 1);

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


void loop()
{

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

        client.println("<h1>SMART HOME AUTOMATION HUB</h2>");
        client.println("<div class=\"row\">");
        client.println("<div class=\"col-md-2\"><a href=\"?pin=LIGHTON\" class=\"btn btn-block btn-lg btn-success\" role=\"button\">LIGHT ON</a></div>");
        client.println("<div class=\"col-md-2\"><a href=\"?pin=LIGHTOFF\" class=\"btn btn-block btn-lg btn-danger\" role=\"button\">LIGHT OFF</a></div>");
        client.println("</div>");
        client.println("<h2></h2>");
        client.println("<div class=\"row\">");
        client.println("<div class=\"col-md-2\"><a href=\"?pin=FANON\" class=\"btn btn-block btn-lg btn-primary\" role=\"button\">FAN ON</a></div>");
        client.println("<div class=\"col-md-2\"><a href=\"?pin=FANOFF\" class=\"btn btn-block btn-lg btn-warning\" role=\"button\">FAN OFF</a></div>");
        client.println("</div>");
        client.println("<h3></h2>");
        client.println("<div class=\"row\">");
        client.println("<div class=\"col-md-2\"><a href=\"?pin=ACON\" class=\"btn btn-block btn-lg btn-primary\" role=\"button\">AC ON</a></div>");
        client.println("<div class=\"col-md-2\"><a href=\"?pin=ACOFF\" class=\"btn btn-block btn-lg btn-warning\" role=\"button\">AC OFF</a></div>");
        client.println("</div>");

           delay(1);
           //stopping client
           client.stop();
           //controls the Arduino if you press the buttons
         if (readString.indexOf("?pin=LIGHTON") >0){
               digitalWrite(LIGHT, LOW);
           }
            else if (readString.indexOf("?pin=LIGHTOFF") >0){
               digitalWrite(LIGHT, HIGH);
           }
           if (readString.indexOf("?pin=FANON") >0){
               digitalWrite(FAN, LOW);
           }
            else if (readString.indexOf("?pin=FANOFF") >0){
               digitalWrite(FAN, HIGH);
           }
             if (readString.indexOf("?pin=ACON") >0){
               digitalWrite(AC, LOW);
           }
           else if (readString.indexOf("?pin=ACOFF") >0){
               digitalWrite(AC, HIGH);
           }
         
            //clearing string for next read
            readString="";  
           
         }
       }
    }
 }
}
