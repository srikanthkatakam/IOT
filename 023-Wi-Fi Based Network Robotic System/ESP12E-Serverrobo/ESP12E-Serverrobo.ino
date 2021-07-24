 #include <ESP8266WiFi.h>
 #define trigPin D8
 #define echoPin D1


const char* ssid     = "your SSID";
const char* password = "password";

int motor1Pin1 = D0; // pin 7 on L293D IC
int motor1Pin2 = D2; // pin 2 on L293D IC

int motor2Pin1 = D3; // pin 15 on L293D IC
int motor2Pin2 = D4; // pin 10 on L293D IC


int LED1=D5;  //Forword
int LED2=D6;  //Right and Revwrse
int LED3=D7;  //Left and Revwrse


const char* host = "192.168.43.239";

WiFiServer server(80);
String readString;
String Action;
int duration, distance;

void setup() {
  Serial.begin(115200);
  delay(10);

    pinMode(motor1Pin1, OUTPUT);
    pinMode(motor1Pin2, OUTPUT);    
    pinMode(motor2Pin1, OUTPUT);
    pinMode(motor2Pin2, OUTPUT);
 
    pinMode(LED1, OUTPUT);    
    pinMode(LED2, OUTPUT);
    pinMode(LED3, OUTPUT);
    pinMode (trigPin, OUTPUT);
    pinMode (echoPin, INPUT);  


  // We start by connecting to a WiFi network

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
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}


void loop() {
  
  serverscript();
  ultrasonic();  
  delay(5000);    
}



void serverscript(){  
 
    // Start the server
  server.begin();
  delay(1000);
  Serial.println("Server started");

  // Print the IP address
  Serial.println(WiFi.localIP());

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
          
       // And here we begin including the HTML
        client.println("<head>");
        client.println("<meta charset=\"utf-8\">");
        client.println("<meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\">");
        client.println("<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
        client.println("<script src=\"https://code.jquery.com/jquery-2.1.3.min.js\"></script>");
        client.println("<link rel=\"stylesheet\" href=\"https://maxcdn.bootstrapcdn.com/bootstrap/3.3.5/css/bootstrap.min.css\">");
        client.println("</head><div class=\"container\">");

        client.println("<h1> SMART ROBOCAR</h1>");
        client.println("<h2> TURN LEFT AND RIGHT</h2>");
        client.println("<div class=\"row\">");
        client.println("<div class=\"col-md-2\"><a href=\"?TURNLEFT\" class=\"btn btn-block btn-lg btn-success\" role=\"button\">LEFT</a></div>");
        client.println("<div class=\"col-md-2\"><a href=\"?TURNRIGHT\" class=\"btn btn-block btn-lg btn-danger\" role=\"button\">RIGHT</a></div>");
        client.println("</div>");
        client.println("<h2> FORWARD AND REVERSE</h2>");
        client.println("<div class=\"row\">");
        client.println("<div class=\"col-md-2\"><a href=\"?FORWARD\" class=\"btn btn-block btn-lg btn-primary\" role=\"button\">FORWARD</a></div>");
        client.println("<div class=\"col-md-2\"><a href=\"?REVERSE\" class=\"btn btn-block btn-lg btn-warning\" role=\"button\">REVERSE</a></div>");
        client.println("<div class=\"col-md-2\"><a href=\"?STOP\" class=\"btn btn-block btn-lg btn-danger\" role=\"button\">STOP</a></div>");
        client.println("<div class=\"col-md-2\"><a href=\"?DESTINATIONAB\" class=\"btn btn-block btn-lg btn-danger\" role=\"button\">DEST-AB</a></div>");   
        client.println("<div class=\"col-md-2\"><a href=\"?DESTINATIONBA\" class=\"btn btn-block btn-lg btn-danger\" role=\"button\">DEST-BA</a></div>");
        client.println("<div class=\"col-md-2\"><a href=\"?DESTINATIONBD\" class=\"btn btn-block btn-lg btn-danger\" role=\"button\">DEST-BD</a></div>");
        client.println("<div class=\"col-md-2\"><a href=\"?DESTINATIONDB\" class=\"btn btn-block btn-lg btn-danger\" role=\"button\">DEST-DB</a></div>"); 
        client.println("<div class=\"col-md-2\"><a href=\"?DESTINATIONBE\" class=\"btn btn-block btn-lg btn-danger\" role=\"button\">DEST-BE</a></div>"); 
        client.println("<div class=\"col-md-2\"><a href=\"?DESTINATIONEB\" class=\"btn btn-block btn-lg btn-danger\" role=\"button\">DEST-EB</a></div>");           
        client.println("<h3> Created by : SmartBridge!</h3>");        
        client.println("</div></div>");
       
           delay(1);
        
           //stopping client
           client.stop();
           //controls the Arduino if you press the buttons
             
        if (readString.indexOf("?TURNLEFT") >0){                
                 ultrasonic();
                  TurnLeft();                             
                    }
        if (readString.indexOf("?TURNRIGHT") >0){
                 ultrasonic();
                  TurnRight();  
                     }
        if (readString.indexOf("?FORWARD") >0){
                  ultrasonic();
                  Forward();          
                     }
        if (readString.indexOf("?REVERSE") >0){
                  ultrasonic();
                  Reverse();               
                    }
        if (readString.indexOf("?STOP") >0){
                 Stop();        
                    }     
        if (readString.indexOf("?DESTINATIONAB") >0){
                  ultrasonic();
                  Forward();
                  delay(5000);
                  Stop();
                  delay(100);
                  ultrasonic();
                  TurnLeft();
                  delay(840);
                  Stop();
                  delay(1000);
                  ultrasonic();
                  Forward();
                  delay(5000);
                  Stop();
                  delay(1000);
                  ultrasonic();
                  TurnRight();
                  delay(840);
                  Stop();
                  delay(1000);
                  ultrasonic();
                  Forward();
                  delay(5000);
                  Stop();
            Action = "DESTINATIONBC";
              clientscript(Action);
                    }
         if (readString.indexOf("?DESTINATIONBA") >0){
                  ultrasonic();
                  TurnRight();
                  delay(950);
                  ultrasonic();
                  Forward();
                  delay(5000);
                  Stop();
                  delay(100);
                  ultrasonic();                
                  TurnRight();
                  delay(840);
                  Stop();
                  delay(1000);
                  ultrasonic();
                  Forward();
                  delay(5000);
                  Stop();
                  delay(600);
                  ultrasonic();
                  TurnLeft();
                  delay(840);
                  Stop();
                  delay(1000);
                  ultrasonic();
                  Forward();
                  delay(5000);
                  Stop();
            Action = "DESTINATIONCB";
              clientscript(Action);                  
                   }
         if (readString.indexOf("?DESTINATIONBD") >0){
                  ultrasonic();
                  TurnRight();
                  delay(950);
                  ultrasonic();
                  Forward();
                  delay(5000);
                  Stop();
                  delay(100);
                  ultrasonic();
                  TurnLeft();
                  delay(840);
                  Stop();
                  delay(1000);
                  ultrasonic();
                  Forward();
                  delay(5000);
                  Stop();
                  delay(1000);
                  ultrasonic();
                  TurnRight();
                  delay(840);
                  Stop();
                  delay(1000);
                  ultrasonic();
                  Forward();
                  delay(5000);
                  Stop();
            Action = "DESTINATIONBD";
              clientscript(Action);
                   }
         if (readString.indexOf("?DESTINATIONDB") >0){
                 ultrasonic();
                 TurnRight();
                  delay(950);
                  ultrasonic();
                  Forward();
                  delay(5000);
                  Stop();
                  delay(100);
                  ultrasonic();                
                  TurnRight();
                  delay(840);
                  Stop();
                  delay(1000);
                  ultrasonic();
                  Forward();
                  delay(5000);
                  Stop();
                  delay(1000);
                  ultrasonic();
                  TurnLeft();
                  delay(840);
                  Stop();
                  delay(1000);
                  ultrasonic();
                  Forward();
                  delay(5000);
                  Stop();
            Action = "DESTINATIONDB";
              clientscript(Action);                  
                   }
         if (readString.indexOf("?DESTINATIONBE") >0){
                  ultrasonic();
                  TurnRight();
                  delay(950);
                  ultrasonic();
                  Forward();
                  delay(5000);
                  Stop();
                  delay(100);
                  ultrasonic();
                  TurnLeft();
                  delay(840);
                  Stop();
                  delay(1000);
                  ultrasonic();
                  Forward();
                  delay(5000);
                  Stop();
                  delay(1000);
                  ultrasonic();
                  TurnRight();
                  delay(840);
                  Stop();
                  delay(1000);
                  ultrasonic();
                  Forward();
                  delay(5000);
                  Stop();
            Action = "DESTINATIONBE";
              clientscript(Action);
                   }
         if (readString.indexOf("?DESTINATIONEB") >0){
                  ultrasonic();
                  TurnRight();
                  delay(950);
                  ultrasonic();
                  Forward();
                  delay(5000);
                  Stop();
                  delay(100);
                  ultrasonic();                
                  TurnRight();
                  delay(840);
                  Stop();
                  delay(1000);
                  ultrasonic();
                  Forward();
                  delay(5000);
                  Stop();
                  delay(1000);
                  ultrasonic();
                  TurnLeft();
                  delay(840);
                  Stop();
                  delay(1000);
                  ultrasonic();
                  Forward();
                  delay(5000);
                  Stop();
            Action = "DESTINATIONEB";
              clientscript(Action);                  
                   }
             //clearing string for next read
            readString=""; 
           
       }
     }
   }
 }
}

void TurnLeft(){
if (distance < 5)
  {
   Stop();                   
  }
  else{
              digitalWrite(motor1Pin1, LOW);   
              digitalWrite(motor1Pin2, LOW); 
              digitalWrite(motor2Pin1, LOW);
              digitalWrite(motor2Pin2, HIGH);
              digitalWrite(LED1, LOW); 
              digitalWrite(LED2, LOW);
              digitalWrite(LED3, HIGH);
              Serial.println("Turn Left");
             
}
}
void TurnRight(){
  if (distance < 8)
  {
   Stop();                   
  }
  else{
                digitalWrite(motor1Pin1, HIGH); 
                digitalWrite(motor1Pin2, LOW); 
                digitalWrite(motor2Pin1, LOW);
                digitalWrite(motor2Pin2, LOW);
                digitalWrite(LED1, LOW); 
                digitalWrite(LED2, HIGH);
                digitalWrite(LED3, LOW);  
                Serial.println("Turn Right");
  }                 
}
void Forward(){
  if (distance < 8)
  {
   Stop();                   
  }
  else{
                digitalWrite(motor1Pin1, HIGH); 
                digitalWrite(motor1Pin2, LOW); 
                digitalWrite(motor2Pin1, LOW);
                digitalWrite(motor2Pin2, HIGH);
                digitalWrite(LED1, HIGH); 
                digitalWrite(LED2, LOW);
                digitalWrite(LED3, LOW);
                Serial.println("Forward");
  }                
}
void Reverse(){
             if (distance < 8)
  {
   Stop();                   
  }
  else{
                digitalWrite(motor1Pin1, LOW); 
                digitalWrite(motor1Pin2, HIGH); 
                digitalWrite(motor2Pin1, HIGH);
                digitalWrite(motor2Pin2, LOW);
                digitalWrite(LED1, LOW); 
                digitalWrite(LED2, HIGH);
                digitalWrite(LED3, HIGH);
                Serial.println("Go Reverse");
  }               
}
void Stop(){
                digitalWrite(motor1Pin1, LOW); 
                digitalWrite(motor1Pin2, LOW); 
                digitalWrite(motor2Pin1, LOW);
                digitalWrite(motor2Pin2, LOW);
                digitalWrite(LED1, LOW); 
                digitalWrite(LED2, LOW);
                digitalWrite(LED3, LOW);
                Serial.println("STOP");
               
}                 
         
void clientscript(String Action){
  
  Serial.print("connecting to ");
  Serial.println(host);
  
  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }
  
  Serial.print("Sending Message ");
  // This will send the request to the server
  
  client.print(String("GET ") + Action + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
               
  //client.println("test123?results");
  delay(10);
  
  // Read all the lines of the reply from server and print them to Serial
  while(client.available()){
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }
  
  Serial.println();
  Serial.println("closing connection");
}
void ultrasonic(){
   digitalWrite(trigPin, HIGH);
   delay(1000);
   digitalWrite (trigPin, LOW);
   duration = pulseIn(echoPin, HIGH);
   distance = (duration/2) / 29.1;
   Serial.println(distance); 
   if (distance < 8)
  {
   Stop();                   
  }
}
