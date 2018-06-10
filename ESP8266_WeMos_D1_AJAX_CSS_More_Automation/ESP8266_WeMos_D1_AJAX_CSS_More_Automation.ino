/*
 * ESP8266 NodeMCU WeMos D1 mini with AJAX
 * Updates and Gets data from webpage without page refresh
 * The button was made entirely from CSS and the html part is inside "index.h";
 */
#include <ESP8266WiFi.h> // use ESP8266WiFi.h header
#include <WiFiClient.h>// use WiFiClient.h header
#include <ESP8266WebServer.h> // useESP8266WebServer.h
#include <SPI.h>

#include "index.h" //Our HTML webpage contents with AJAX and jQuery
#define LED D5  //use Digital Output No. 5 from board
#define SWITCH D6  //use Digital Output No. 5 for switch
#define Jos D4 // read value from D7
char Gserver[] = "smtp.gmail.com";
int port = 465;
int inputPin = D3;
int val1 = 0, val2 = 0;
bool state = true;
String t_state ="";
//SSID and Password of your WiFi router
const char* ssid = "@pass";
const char* password = "@@!@!";

ESP8266WebServer server(80); //Server on port 80
WiFiClientSecure client;
//===============================================================
// The following routine is executed when you open its IP in browser
//===============================================================
void handleRoot() {
 String s = MAIN_page; //Read HTML contents
 server.send(200, "text/html", s); //Send web page
}

void handleADC() // functon to read if there is voltage on board 
{
  int a = analogRead(A0); 
  String adcValue = String(a);
  server.send(200, "text/plane", t_state); //Send ADC value only to client ajax request
//  server.send(200, "text/plane", switchValue);
}

void handleSwitch()
{
  int noua = 99;
  String switcher = String(val1);
  server.send(200,"text/plane",switcher);
  }
void handleLED() //function to handle the HIGH and the LOW part of the LED;
{
  String ledState = "OFF"; // we defined a string, which show us the current status of the led on our webpage;
  t_state = server.arg("LEDstate"); //Refer  xhttp.open("GET", "setLED?LEDstate="+led, true);
  Serial.println(t_state);//we are printing the state of the led inside Serial Monitor
  if(t_state == "1")
  {
    digitalWrite(LED,LOW); //LED ON
    ledState = "ON"; //Feedback parameter
  }
  else
  {
    digitalWrite(LED,HIGH); //LED OFF
    ledState = "OFF"; //Feedback parameter
  }
  server.send(200, "text/plane", ledState); //Send web page
}
//==============================================================
//                  SETUP
//==============================================================
void setup(void)
{
  Serial.begin(115200);
  
  WiFi.begin(ssid,password);     //Connect to your WiFi router

  Serial.println(WiFi.localIP());
//  IPAddress ip(192,168,1,99); // set an StaticIp Address in order to not look for IP all the times
//  IPAddress gateway(192,168,1,254); // set the Gateway
//  IPAddress subnet(255,255,255,0); // set the Subnet Mask
//  WiFi.config(ip,gateway,subnet); // add the configuration to the server
  Serial.println("");

  //Onboard LED port Direction output
  digitalWrite(LED,1); // since we're using an Relay which is activated in Low Power Mode, we're setting the value to 1, so it start closed;
  pinMode(LED,OUTPUT);
  sendEmail(); 
  pinMode(SWITCH,OUTPUT);
  pinMode(SWITCH,HIGH);
  pinMode(BUILTIN_LED,OUTPUT);
  pinMode(inputPin,INPUT);
  pinMode(Jos,INPUT);
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }

  //If connection successful show IP address in serial monitor
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  //IP address assigned to your ESP
  if(sendEmail()) Serial.println(F("Email sent"));
      else Serial.println(F("Email failed"));
  server.on("/", handleRoot);      //Which routine to handle at root location. This is display page
  server.on("/setLED", handleLED);
  server.on("/readADC", handleADC);
  server.on("/readSwitch", handleSwitch);
  
  server.begin();                  //Start server
  Serial.println("HTTP server started");
}
//==============================================================
//                     LOOP
//==============================================================
void loop(void){
  server.handleClient();          //Handle client requests
  
  val1 = digitalRead(inputPin);
  digitalWrite(BUILTIN_LED,val1);
//  Serial.println(val1);
//  val2 = digitalRead(Jos);
//  digitalWrite(SWITCH,HIGH);
  if(val1 == HIGH && state == true)
  {
    Serial.println("Intrerupator actionat in sus");
    Serial.println(val1);
    state = false;
    }
    else if (val1 == LOW && state == false)
    {
      Serial.println("Intrerupator actionat in jos");
      Serial.println(val1);
      state = true;
      }
}

byte sendEmail()
{
  byte thisByte = 0;
  byte respCode;

  if(client.connect(Gserver,port) == 1) {
    Serial.println(F("connected 1"));
  } else {
    Serial.println(F("connection failed"));
    return 0;
  }

  if(!eRcv()) return 0;

  Serial.println(F("Sending hello"));
// replace 1.2.3.4 with your Arduino's ip
  client.println("EHLO 192.168.1.10");
  if(!eRcv()) return 0;

  Serial.println(F("Sending auth login"));
  client.println("auth login");
  if(!eRcv()) return 0;

  Serial.println(F("Sending User"));
// Change to your base64 encoded user
  client.println("gmailuser");     //my user name

  if(!eRcv()) return 0;

  Serial.println(F("Sending Password"));
// change to your base64 encoded password
  client.println("gmailpassword");     //password

  if(!eRcv()) return 0;

// change to your email address (sender)
  Serial.println(F("Sending From"));
  client.println("MAIL From: <@gmail.com>");
  if(!eRcv()) return 0;

// change to recipient address
  Serial.println(F("Sending To"));
  client.println("RCPT To: <@gmail.com>");
  if(!eRcv()) return 0;

  Serial.println(F("Sending DATA"));
  client.println("DATA");
  if(!eRcv()) return 0;

  Serial.println(F("Sending email"));

// change to recipient address
  client.println("<@gmail.com>");

// change to your address
//  client.println("From: Me <@gmail.com>");

//  client.println("Subject: Arduino email test\r\n");

  client.println("This is from my Arduino!:" + WiFi.localIP().toString());

  client.println(".");

  if(!eRcv()) return 0;

  Serial.println(F("Sending QUIT"));
  client.println("QUIT");
  if(!eRcv()) return 0;

  client.stop();

  Serial.println(F("disconnected"));

  return 1;
}

byte eRcv()
{
  byte respCode;
  byte thisByte;
  int loopCount = 0;

  while(!client.available()) {
    delay(1);
//    Serial.println("Client not available");
    loopCount++;

    // if nothing received for 10 seconds, timeout
    if(loopCount > 20000) {
      
      client.stop();
      Serial.println(F("\r\nTimeout"));
      return 0;
    }
  }

  respCode = client.peek();

  while(client.available())
  {  
    thisByte = client.read();    
    Serial.write(thisByte);
  }

  if(respCode >= '4')
  {
    efail();
    return 0;  
  }

  return 1;
}


void efail()
{
  byte thisByte = 0;
  int loopCount = 0;

  client.println(F("QUIT"));

  while(!client.available()) {
    delay(1);
    loopCount++;

    // if nothing received for 10 seconds, timeout
    if(loopCount > 100000) {
      Serial.println("Second while");
      client.stop();
      Serial.println(F("\r\nTimeout"));
      return;
    }
  }

  while(client.available())
  {  
    thisByte = client.read();    
    Serial.write(thisByte);
  }

  client.stop();

  Serial.println(F("disconnected"));
}
