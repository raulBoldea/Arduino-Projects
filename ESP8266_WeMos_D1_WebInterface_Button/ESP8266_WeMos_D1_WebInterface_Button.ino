/*********
  Rui Santos
  Complete project details at http://randomnerdtutorials.com
*********/

// Load Wi-Fi library
#include <ESP8266WiFi.h>

// Replace with your network credentials
const char* ssid     = "!";
const char* password = "frigider";

// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;

// Auxiliar variables to store the current output state
String output5State = "off";
String output4State = "off";

// Assign output variables to GPIO pins
const int output5 = 5;
const int output4 = 4;

void setup() {
  Serial.begin(115200);
  // Initialize the output variables as outputs
  pinMode(output5, OUTPUT);
  pinMode(output4, OUTPUT);
  // Set outputs to LOW
  digitalWrite(output5, LOW);
  digitalWrite(output4, LOW);

  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop() {
  WiFiClient client = server.available();   // Listen for incoming clients

  if (client) {                             // If a new client connects,
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

            // turns the GPIOs on and off
            if (header.indexOf("GET /5/on") >= 0) {
              Serial.println("GPIO 5 on");
              output5State = "on";
              digitalWrite(output5, HIGH);
            } else if (header.indexOf("GET /5/off") >= 0) {
              Serial.println("GPIO 5 off");
              output5State = "off";
              digitalWrite(output5, LOW);
            } else if (header.indexOf("GET /4/on") >= 0) {
              Serial.println("GPIO 4 on");
              output4State = "on";
              digitalWrite(output4, HIGH);
            } else if (header.indexOf("GET /4/off") >= 0) {
              Serial.println("GPIO 4 off");
              output4State = "off";
              digitalWrite(output4, LOW);
            }

            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" charset=\"utf-8\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            client.println("<link rel=\"stylesheet\" href=\"https://cdnjs.cloudflare.com/ajax/libs/font-awesome/4.7.0/css/font-awesome.min.css\">");
            client.println("<script src=\"https://ajax.googleapis.com/ajax/libs/jquery/3.1.0/jquery.min.js\"></script>");
            client.println("<script> $(document).ready(function(){$('#button').click(function(){$(this).toggleClass('on');});}); </script>");
            // CSS to style the on/off buttons
            // Feel free to change the background-color and font-size attributes to fit your preferences
            client.println("<style>");
//            client.println(".button { background-color: #00ff99; border: none; color: white; padding: 16px 40px;");
//            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println("body { background: url('http://subtlepatterns.com/patterns/micro_carbon.png'); }");
            client.println("section { margin: 150px auto 0; width: 75px; height: 95px; position: relative; text-align: center;}");
            client.println(":active, :focus { outline: 0;}");
            client.println("@font-face { font-family: \"FontAwesome\"; src: url(\"fonts/fontawesome-webfont.eot\");src: url(\"fonts/fontawesome-webfont.eot?#iefix\") format('eot'), url(\"fonts/fontawesome-webfont.woff\") format('woff'), url(\"fonts/fontawesome-webfont.ttf\") format('truetype'), url(\"fonts/fontawesome-webfont.svg#FontAwesome\") format('svg'); font-weight: normal; font-style: normal;}");
            client.print("a { font-family: \"FontAwesome\" ; text-shadow: 0px 1px 1px rgba(250,250,250,0.1); font-size: 32pt; display: block; position: relative; text-decoration: none; box-shadow: 0px 3px 0px 0px rgb(34,34,34), 0px 7px 10px 0px rgb(17,17,17),inset 0px 1px 1px 0px rgba(250, 250, 250, .2), inset 0px -12px 35px 0px rgba(0, 0, 0, .5); width: 70px; height: 70px; border: 0; color: rgb(37,37,37); border-radius: 35px; text-align: center; line-height: 79px; background-color: rgb(83,87,93); transition: color 350ms ease, text-shadow 350ms; -o-transition: color 350ms ease, text-shadow 350ms; -moz-transition: color 350ms ease, text-shadow 350ms; -webkit-transition: color 350ms ease, text-shadow 350ms;}");
            client.println("a:before { content:\"\"; width: 80px; height: 80px; display: block; z-index: -2; position: absolute; background-color: rgb(26,27,29); left: -5px; top: -2px; border-radius: 40px; box-shadow: 0px 1px 0px 0px rgba(250,250,250,0.1), inset 0px 1px 2px rgba(0, 0, 0, 0.5);}");
            client.println("a:active { box-shadow: 0px 0px 0px 0px rgb(34,34,34), 0px 3px 7px 0px rgb(17,17,17), inset 0px 1px 1px 0px rgba(250, 250, 250, .2), inset 0px -10px 35px 5px rgba(0, 0, 0, .5); background-color: rgb(83,87,93); top: 3px; }");
            client.println("a.on { box-shadow: 0px 0px 0px 0px rgb(34,34,34), 0px 3px 7px 0px rgb(17,17,17), inset 0px 1px 1px 0px rgba(250, 250, 250, .2), inset 0px -10px 35px 5px rgba(0, 0, 0, .5); background-color: rgb(83,87,93); top: 3px; color: #fff; text-shadow: 0px 0px 3px rgb(250,250,250); }");
            client.println("a:active:before, a.on:before { top: -5px; background-color: rgb(26,27,29); box-shadow: 0px 1px 0px 0px rgba(250,250,250,0.1), inset 0px 1px 2px rgba(0, 0, 0, 0.5); }");
            client.println("a + span { display: block; width: 8px; height: 8px; background-color: rgb(226,0,0); box-shadow: inset 0px 1px 0px 0px rgba(250,250,250,0.5), 0px 0px 3px 2px rgba(226,0,0,0.5); border-radius: 4px; clear: both; position: absolute; bottom: 0; left: 42%; transition: background-color 350ms, box-shadow 700ms; -o-transition: background-color 350ms, box-shadow 700ms; -moz-transition: background-color 350ms, box-shadow 700ms; -webkit-transition: background-color 350ms, box-shadow 700ms; }");
            client.println("a.on + span { box-shadow: inset 0px 1px 0px 0px rgba(250,250,250,0.5), 0px 0px 3px 2px rgba(135,187,83,0.5); background-color: rgb(135,187,83); }");
            client.println("h1 {text-align : center; color: white; }");
            client.println("p {text-align : center ; color: white; }");
            client.println("</style></head>");


            // Web Page Heading
            client.println("<body><h1>ESP8266 Web Server</h1>");
            client.println("<section><a href=\"#\" id=\"button\">&#xF011;</a><span></span></section>");
            // Display current state, and ON/OFF buttons for GPIO 5
//            client.println("<p>GPIO 5 - State " + output5State + "</p>");
            // If the output5State is off, it displays the ON button
            if (output5State == "off") {
//              client.println("<p><a href=\"/5/on\"><button class=\"button\" >&#xF011</button></a></p>");
            } else {
//              client.println("<p><a href=\"/5/off\"><button class=\"button button2\">&#xF011</button></a></p>");
            }

            // Display current state, and ON/OFF buttons for GPIO 4
//            client.println("<p>GPIO 4 - State " + output4State + "</p>");
            // If the output4State is off, it displays the ON button
            if (output4State == "off") {
//              client.println("<p><a href=\"/4/on\"><button class=\"button\">ON</button></a></p>");
            } else {
//              client.println("<p><a href=\"/4/off\"><button class=\"button button2\">OFF</button></a></p>");
            }
            client.println("<br>");
            client.println("<p>©Boldea Raul </p>");
            client.println("</body></html>");

            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}
