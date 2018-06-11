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

#define LED D5  //use Digital Output No. 5 from board to switch relay state
#define SWITCH D6  //use Digital Output No. 6 for switch / control inpot of D3

int inputPin = D3; // pin which read the value from D6 this can have two states 0 or 1;
int val1 = 0; // later store the value from D3;
bool state = true; // a boolean to handle the val1 value;

String t_state =""; // a string to handle the led state inside the interface

const char* ssid = "SSID"; // the value of SSID to access the router;
const char* password = "PASSWORD"; // the password to access the router;
char Gserver[] = "smtp.gmail.com"; // value to can access smtp from Gmail;
int port = 465; // the port where the mail server can be accessed

ESP8266WebServer server(80); //create a new Server on port 80
WiFiClientSecure client; // create a new WiFiClient to handle the email triggering
//===============================================================

// The following functions are executed when we're accessing the IP inside browser

//===============================================================
void handleRoot() { //the function which runs first
 String s = MAIN_page; //Read HTML contents by accessing the value from MAIN_page which is defined inside index.h
 server.send(200, "text/html", s); //Send web page
}

void handleADC() // function which handle the interface when the button is pressed
{
  server.send(200, "text/plane", t_state); //Send ADC value only to client ajax request
}

void handleSwitch()  // function which handle the interface when the switcher is pressed
{
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
  Serial.begin(115200); // begin to listen to Serial on 115200
  
  WiFi.begin(ssid,password);     //Connect to our WiFi router

  Serial.println(WiFi.localIP()); // print the Ip on Serial
  sendEmail(); // call the email function, to send the Ip address to a user when
//  IPAddress ip(192,168,1,99); // set an StaticIp Address in order to not look for IP all the times
//  IPAddress gateway(192,168,1,254); // set the Gateway
//  IPAddress subnet(255,255,255,0); // set the Subnet Mask
//  WiFi.config(ip,gateway,subnet); // add the configuration to the server
  Serial.println("");

  //Onboard LED port Direction output
  digitalWrite(LED,1); // since we're using an Relay which is activated in Low Power Mode, we're setting the value to 1, so it start closed;
  pinMode(LED,OUTPUT); // set the LED as OUTPUT, this handles the In value from Relay
  pinMode(SWITCH,OUTPUT); // set SWITCH as an OUTPUT
  pinMode(SWITCH,HIGH); // turn SWITCH value to HIGH or 1. All the time this value is high, so we can read it to our switch
  pinMode(inputPin,INPUT); //read the value from SWITCH, all the time DC pass through a real switch and if the conection is closed, this reads 1
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print("."); // print ... until the connection is made
  }

  //If connection successful show IP address in serial monitor
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  //IP address assigned to your ESP
  if(sendEmail()) Serial.println(F("Email sent"));//if the email is send succesfully, we print a message inside serial
  else Serial.println(F("Email failed"));//else we print Email failed
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
  
  val1 = digitalRead(inputPin);//read the value from inputPin(D3)
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

byte sendEmail() // sendEmail functions, this is triggered inside Setup function
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
