#include <ESP8266WiFi.h>

const char* ssid     = "your SSID";
const char* password = "password";

int motor1Pin1 = D0; // pin 7 on L293D IC
int motor1Pin2 = D2; // pin 2 on L293D IC

int motor2Pin1 = D3; // pin 15 on L293D IC
int motor2Pin2 = D4; // pin 10 on L293D IC

int LED1=D5;  //Forword
int LED2=D6;  //Right and Revwrse
int LED3=D7;  //Left and Revwrse


WiFiServer server(80);
String readString;

void setup() {
  Serial.begin(115200);
  delay(10);
  
  // sets the pins as outputs:
  
    pinMode(motor1Pin1, OUTPUT);
    pinMode(motor1Pin2, OUTPUT);    
    pinMode(motor2Pin1, OUTPUT);
    pinMode(motor2Pin2, OUTPUT);
    pinMode(LED1, OUTPUT);    
    pinMode(LED2, OUTPUT);
    pinMode(LED3, OUTPUT);
    
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
  delay(2000);
 
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
        delay(1);
           //stopping client
           client.stop();
           //controls the Arduino if you press the buttons

         
       if (readString.indexOf("DESTINATIONBC") >0){
                    TurnLeft();
                    delay(840);
                    Forward();
                    delay(5000);
                      Stop();
                     delay(100);
                     TurnLeft();
                     delay(840);
                      Forward();
                     delay(5000);
                      Stop();
       }
          if (readString.indexOf("DESTINATIONCB") >0){
                    TurnLeft();
                    delay(1700);
                    Forward();
                    delay(5000);
                      Stop();
                     delay(100);
                    TurnRight();
                     delay(840);
                      Forward();
                     delay(5000);
                      Stop();
          }
         if (readString.indexOf("?DESTINATIONBD") >0){
                  delay(1000);
                  TurnRight();
                  delay(840);
                  Forward();
                  delay(4000);
                  Stop();
                  delay(100);
                  TurnLeft();
                  delay(840);
                  Stop();
                  delay(1000);
                  Forward();
                  delay(4000);
                  Stop();
                  delay(1000);
                  TurnRight();
                  delay(840);
                  Stop();
                  delay(1000);
                  Forward();
                  delay(4000);
                  Stop();
                   }
       if (readString.indexOf("?DESTINATIONDB") >0){          
                  TurnRight();
                  delay(1700);
                  Forward();
                  delay(4000);
                  Stop();
                  delay(100);
                  TurnRight();
                  delay(840);
                  Stop();
                  delay(1000);
                  Forward();
                  delay(4000);
                  Stop();
                  delay(1000);
                  TurnLeft();
                  delay(840);
                  Stop();
                  delay(1000);
                  Forward();
                  delay(4000);
                  Stop();
                   }
       if (readString.indexOf("DESTINATIONBE") >0){                
                   TurnRight();
                    delay(840);
                    Forward();
                    delay(6000);
                      Stop();
       }
          if (readString.indexOf("DESTINATIONEB") >0){
                  TurnLeft();
                    delay(1700);
                    Forward();
                    delay(6000);
                      Stop();
          }
            //clearing string for next read
            readString="";  
           
         }
       }
    }
}
}
void TurnLeft(){
              digitalWrite(motor1Pin1, LOW); 
              digitalWrite(motor1Pin2, LOW); 
              digitalWrite(motor2Pin1, LOW);
              digitalWrite(motor2Pin2, HIGH);
              digitalWrite(LED1, LOW); 
              digitalWrite(LED2, LOW);
              digitalWrite(LED3, HIGH);
              Serial.println("Turn Left");
}

void TurnRight(){
                digitalWrite(motor1Pin1, HIGH); 
                digitalWrite(motor1Pin2, LOW); 
                digitalWrite(motor2Pin1, LOW);
                digitalWrite(motor2Pin2, LOW);
                digitalWrite(LED1, LOW); 
                digitalWrite(LED2, HIGH);
                digitalWrite(LED3, LOW);                
                Serial.println("Turn Right");
}
void Forward(){
                digitalWrite(motor1Pin1, HIGH); 
                digitalWrite(motor1Pin2, LOW); 
                digitalWrite(motor2Pin1, LOW);
                digitalWrite(motor2Pin2, HIGH);
                digitalWrite(LED1, HIGH); 
                digitalWrite(LED2, LOW);
                digitalWrite(LED3, LOW);
                Serial.println("Forward");
}
void Reverse(){
             
                digitalWrite(motor1Pin1, LOW); 
                digitalWrite(motor1Pin2, HIGH); 
                digitalWrite(motor2Pin1, HIGH);
                digitalWrite(motor2Pin2, LOW);
                digitalWrite(LED1, LOW); 
                digitalWrite(LED2, HIGH);
                digitalWrite(LED3, HIGH);
                Serial.println("Go Reverse");
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
