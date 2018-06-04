/*
 * ESP8266 NodeMCU WeMos D1 mini with AJAX
 * Updates and Gets data from webpage without page refresh
 * The button was made entirely from CSS and the html part is inside "index.h";
 */
#include <ESP8266WiFi.h> // use ESP8266WiFi.h header
#include <WiFiClient.h>// use WiFiClient.h header
#include <ESP8266WebServer.h> // useESP8266WebServer.h

#include "index.h" //Our HTML webpage contents with AJAX and jQuery

#define LED D5  //use Digital Output No. 5 from board
#define SWITCH D6  //use Digital Output No. 5 for switch
#define Jos D4 // read value from D7
int inputPin = D3;
int val1 = 0, val2 = 0;
bool state = true;
//SSID and Password of your WiFi router
const char* ssid = "@";
const char* password = "laborator5";

ESP8266WebServer server(80); //Server on port 80

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
  server.send(200, "text/plane", adcValue); //Send ADC value only to client ajax request
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
  String t_state = server.arg("LEDstate"); //Refer  xhttp.open("GET", "setLED?LEDstate="+led, true);
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
