
  
  /*
  * Read a card using a mfrc522 reader on your SPI interface
  * Pin layout should be as follows (on Arduino Uno):
  * MOSI: D7 / ICSP-4
  * MISO: D6 / ICSP-1
  * SCK: D5 / ISCP-3
  * SS/SDA: D4
  * RST: Pin D2
  */
  #include <ESP8266WiFi.h>
  #include <LiquidCrystal.h>
  #include <SoftwareSerial.h>
  SoftwareSerial lcd(D1, D3);
  #include <SPI.h>
  #include <RFID.h>
  #include <Servo.h>
  Servo myservo;
  
  #define SS_PIN D4
  #define RST_PIN D2
  
  RFID rfid(SS_PIN,RST_PIN);
  WiFiClient client; 
  const char* ssid     = "SSID";
  const char* password = "password";
  
  
  const char* host = "";          
  int serNum[5];
  
  int Card1_Status = 0 ;
  int Card2_Status = 0 ;
  int Card3_Status = 0 ;
  int Card4_Status = 0 ;
  int Card5_Status = 0 ;
  int Card6_Status = 0 ;
  int Card_Status;
  
  String cancat_card = " ";
  String Card1 = "146,189,145,117,203";
  String Card2 = "72,149,52,16,249";
  String Card3 = "17,190,171,117,113";
  String Card4 = "117,151,207,101,72";
  String Card5 = "85,102,211,101,133";
  String Card6 = "197,248,210,101,138";
  // int cards[] = {Card1,Card2,Card3};
  /* 
  int cards[][5] = {
     {146,189,145,117,203},
     {72,149,52,16,249},
     { 17,190,171,117,113},
     {117,151,207,101,72},
     {85,102,211,101,133},
     {197,248,210,101,138}
  };
   */
     int Card1_attendance ;
     int Card2_attendance ;
     int Card3_attendance ;
     int Card4_attendance ;
     int Card5_attendance ;
     int Card6_attendance ;
     int Card_attendance;
  int flag = 0 ;  
  bool access = false;

void setup()
  {

    Serial.begin(9600);
    SPI.begin();
    rfid.init();
    myservo.attach(D0);
    myservo.write(0);
    lcd.begin(9600);
// We start by connecting to a WiFi network

    Serial.println();
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);
      
    WiFi.begin(ssid, password);
      
      while (WiFi.status() != WL_CONNECTED)
        {
           delay(500);
           Serial.print(".");
        }

    Serial.println("");
    Serial.println("WiFi connected");  
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

   
   }

void loop()
{
    if(rfid.isCard()){
                              if(rfid.readCardSerial()){
                                       Serial.print(rfid.serNum[0]);
                                       Serial.print(" ");
                                       Serial.print(rfid.serNum[1]);
                                       Serial.print(" ");            
                                       Serial.print(rfid.serNum[2]);             
                                       Serial.print(" ");
                                       Serial.print(rfid.serNum[3]);
                                       Serial.print(" ");
                                       Serial.print(rfid.serNum[4]);
                                       Serial.println("");
                                       
                                        // Cancat String
                                      cancat_card = String(rfid.serNum[0])+","+String(rfid.serNum[1])+","+String(rfid.serNum[2])+","+String(rfid.serNum[3])+","+String(rfid.serNum[4]);
                                      
                                         if(cancat_card == Card1)
                                           {
                                                access = true; 
                                                if(Card1_Status == 0)
                                            {
                                                 Card1_Status = 1 ;
                                                 Card_Status = Card1_Status ;
                                                 cancat_card ="146,189,145,117,203";
                                                 Card1_attendance =Card1_attendance + 1  ;
                                                 Serial.println("Card Id = "+cancat_card+" == Welcome == it is Card 1 ");
                                                 upload(cancat_card ,Card_attendance,Card_Status );
                                                 Card_attendance =Card1_attendance;
                                                 //delay(1000);
                                                 myservo.write(180);
                                                 delay(1500);
                                                 myservo.write(0);
                                              }
                                              else
                                              {
                                                Card1_Status = 0 ;
                                                Card_Status = Card1_Status ;
                                                cancat_card ="146,189,145,117,203";
                                                
                                                Serial.println("Card Id = "+cancat_card+" == Bye == it is Card 1");
                                                upload(cancat_card ,Card_attendance,Card_Status );
                                                myservo.write(180);
                                                delay(1500);
                                                myservo.write(0);
                                              }                                
                                           }
                                              else if(cancat_card == Card2)
                                              {
                                                access = true;
                                                if(Card2_Status == 0)
                                                {
                                                   Card2_Status = 1 ;
                                                   Card_Status = Card2_Status ;
                                                   cancat_card ="72,149,52,16,249";
                                                   Serial.println("Card Id = "+cancat_card+" == Welcome == it is Card 2");
                                                   Card2_attendance =Card2_attendance + 1  ;
                                                   Card_attendance =Card2_attendance;
                                                   upload(cancat_card ,Card_attendance,Card_Status );
                                                   //delay(1000);
                                                   myservo.write(180);
                                                   delay(1500);
                                                   myservo.write(0);
                                                }
                                                else
                                                {
                                                  Card2_Status = 0 ;
                                                  Card_Status = Card2_Status ;
                                                  cancat_card ="72,149,52,16,249";
                                                  
                                                  Serial.println("Card Id = "+cancat_card+" == Bye == it is Card 2");
                                                  upload(cancat_card ,Card_attendance,Card_Status );
                                                   //delay(1000);
                                                  myservo.write(180);
                                                  delay(1500);
                                                  myservo.write(0);
                                                   
                                                }  
                                                          
                                              } 
                                              else if(cancat_card == Card3)
                                              {
                                                access = true;
                                                if(Card3_Status == 0)
                                                {
                                                   Card3_Status = 1 ;
                                                   Card_Status = Card3_Status ;
                                                   cancat_card ="17 190 171 117 113";
                                                   Serial.println("Card Id = "+cancat_card+" == Welcome == it is Card 3");
                                                   Card3_attendance =Card3_attendance + 1  ;
                                                   Card_attendance =Card3_attendance;
                                                   upload(cancat_card ,Card_attendance,Card_Status );
                                                  // delay(1000);
                                                   myservo.write(180);
                                                   delay(1500);
                                                   myservo.write(0);
                                                }
                                                else
                                                {
                                                  Card3_Status = 0 ;
                                                  Card_Status = Card3_Status ;
                                                  cancat_card ="17 190 171 117 113";   
                                                  Serial.println("Card Id = "+cancat_card+" == Bye == it is Card 3");
                                             
                                                  upload(cancat_card ,Card_attendance,Card_Status );                                                 
                                                  myservo.write(180);
                                                  delay(1500);
                                                  myservo.write(0);
                                                }
                                                            
                                              }
                                               else if(cancat_card == Card4)
                                              {
                                                access = true;
                                                if(Card4_Status == 0)
                                                {
                                                   Card4_Status = 1 ;
                                                   Card_Status = Card4_Status ;
                                                   cancat_card ="117,151,207,101,72";
                                                   
                                                   Serial.println("Card Id = "+cancat_card+" == Welcome == it is Card 4");
                                                   Card4_attendance =Card4_attendance + 1  ;
                                                   Card_attendance =Card4_attendance;
                                                   upload(cancat_card ,Card_attendance,Card_Status );
                                                   //delay(1000);
                                                   myservo.write(180);
                                                   delay(1500);
                                                   myservo.write(0);
                                                }
                                                else
                                                {
                                                  Card4_Status = 0 ;
                                                  Card_Status = Card4_Status ;
                                                  cancat_card ="117,151,207,101,72";
                                                  
                                                  Serial.println("Card Id = "+cancat_card+" == Bye == it is Card 4");
                                                  upload(cancat_card ,Card_attendance,Card_Status );
                                                   //delay(1000);
                                                  myservo.write(180);
                                                  delay(1500);
                                                  myservo.write(0);
                                                   
                                                }  
                                                          
                                              }
                                              
                                               else if(cancat_card == Card5)
                                              {
                                                access = true;
                                                if(Card5_Status == 0)
                                                {
                                                   Card5_Status = 1 ;
                                                   Card_Status = Card5_Status ;
                                                   cancat_card ="85,102,211,101,133";
                                                   
                                                   Serial.println("Card Id = "+cancat_card+" == Welcome == it is Card 4");
                                                   Card5_attendance =Card5_attendance + 1  ;
                                                   Card_attendance =Card5_attendance;
                                                   upload(cancat_card ,Card_attendance,Card_Status );
                                                   //delay(1000);
                                                   myservo.write(180);
                                                   delay(1500);
                                                   myservo.write(0);
                                                }
                                                else
                                                {
                                                  Card5_Status = 0 ;
                                                  Card_Status = Card5_Status ;
                                                  cancat_card ="85,102,211,101,133";
                                                  
                                                  Serial.println("Card Id = "+cancat_card+" == Bye == it is Card 4");
                                                  upload(cancat_card ,Card_attendance,Card_Status );
                                                   //delay(1000);
                                                  myservo.write(180);
                                                  delay(1500);
                                                  myservo.write(0);
                                                   
                                                }  
                                                          
                                              }
                                               else if(cancat_card == Card6)
                                              {
                                                access = true;
                                                if(Card6_Status == 0)
                                                {
                                                   Card6_Status = 1 ;
                                                   Card_Status = Card6_Status ;
                                                   cancat_card ="197,248,210,101,138";
                                                   
                                                   Serial.println("Card Id = "+cancat_card+" == Welcome == it is Card 4");
                                                   Card6_attendance =Card6_attendance + 1  ;
                                                   Card_attendance =Card6_attendance;
                                                   upload(cancat_card ,Card_attendance,Card_Status );
                                                   //delay(1000);
                                                   myservo.write(180);
                                                   delay(1500);
                                                   myservo.write(0);
                                                }
                                                else
                                                {
                                                  Card6_Status = 0 ;
                                                  Card_Status = Card6_Status ;
                                                  cancat_card ="197,248,210,101,138";
                                                  
                                                  Serial.println("Card Id = "+cancat_card+" == Bye == it is Card 4");
                                                  upload(cancat_card ,Card_attendance,Card_Status );
                                                   //delay(1000);
                                                  myservo.write(180);
                                                  delay(1500);
                                                  myservo.write(0);
                                                   
                                                }  
                                                          
                                              } 
                                              else
                                              {
                                                Serial.println("Cancat Id = "+cancat_card);
                                                Serial.println("=== Invalid Card ===");
                                                access = false;
                                                delay(1000);
                                                           
                                              }                
                                            }

          }
     getTalkBack();
    rfid.halt();
    }

void upload( String cancat_card ,int Card_Status,int Card_attendance )
       {
            Serial.print("connecting to ");
            Serial.println(host);
  
            // Use WiFiClient class to create TCP connections 
            WiFiClient client;
            const int httpPort = 80;
                 if (!client.connect(host, httpPort))
                    {
                       Serial.println("connection failed");
                        return;
                    }
            // We now create a URI for the request
            String url="&field1=";
            url += cancat_card;//card no
            url += "&field2=";
            url +=   Card_attendance;//attendance
            url += "&field3=";
            url += Card_Status;//card status
          
             url="/API?"+url;
               if (client.connect("", 80))
                  {
                    Serial.println("host connected");
                    Serial.println(url);
                    client.print("GET "+url+" HTTP/1.1\n");
                    client.print("Host: \n");
                    client.print("Connection: close\n");
                    client.print("Content-Type: application/x-www-form-urlencoded\n");
                    client.print("Content-Length: ");
                    client.print(url.length());
                    client.print("\n\n");
                    client.print(url);
                    
                      if (client.find("@@"))
                      {
                         String Display = client.readStringUntil('\r');
                         Serial.println(Display);
                      }
                          delay(10);    
                          Serial.println();
                          Serial.println("closing connection");
                  }

    }
void getTalkBack()
   {
      String  tsData="t";
      if (client.connect("", 80)) 
          {
              Serial.println("connected");
              client.print("GET "+tsData+" HTTP/1.1\n");
              client.print("Host: \n");
              client.print("Connection: close\n");
              client.print("Content-Type: application/x-www-form-urlencoded\n");
              client.print("Content-Length: ");
              client.print(tsData.length());
              client.print("\n\n");
              client.print(tsData);
                 if(client.find("@@"))
                     {
                        String Display = client.readStringUntil('\r');
                        Serial.println(Display);
                        lcd.println(Display);
                        lcd.println();
                      }
          }
    }
