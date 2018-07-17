/*
   ESP8266 NodeMCU WeMos D1 mini with AJAX
   Updates and Gets data from webpage without page refresh
   The button was made entirely from CSS and the html part is inside "index.h";
   For the connection part, WiFiManager was used
   You can use WifiManager from below link:
   https://github.com/tzapu/WiFiManager
*/
#include <ESP8266WiFi.h> // includem headerul ESP8266WiFi.h
#include <WiFiClient.h>// includem headerul WiFiClient.h 
#include <ESP8266WebServer.h> // includem headerul useESP8266WebServer.h
#include <SPI.h> // includem headerul SPI.h
#include <Base64.h> // includem headerul Base64
#include <DNSServer.h> // includem headerul DNSServer
#include <WiFiManager.h> //includem headerul WifiManager
#include "index.h" // includem headerul index

#define RELAY D5  //Definim constanta RELAY, pentru pinul digital 5 (D5);
#define SWITCH D6  //Definim constanta SWITCH, pentru pinul digital 6 D(6);
#define inputPin D3 //Definim constanta inputPin, pentru pinul digital 3 D(3);
/// INT's ///
int val1 = 0; // reține valorile ce vor fi utilizate cu pin-ul D3;
int port = 465; // portul unde serverul de email este accesat
/// CHARS ///
char email_token[32]; // field-ul unde va fi introdusă adresa de emailș
char Gserver[] = "smtp.gmail.com"; // Valoarea unde se acceseaza serverul de smtp Gmail;
/// STRINGS ///
String emailConectare = ""; // String pentru stocarea adresei de email;
String email64 = ""; // String-ul unde va fi stocată adresa de email transformata in baza64
String t_state = ""; // variabila pentru a gestiona starea releului in interfata
/// BOOLS ///
bool state = true; //

WiFiManager wifiManager; // cream un nou obiect de tipul WiFiManager;
ESP8266WebServer server(80); //cream un nou server ce va utiliza portul 80;
WiFiClientSecure client; // cream un nou client care se va ocupa de partea de trimitere a email-ului;
//===============================================================

//Următoarele funcții sunt executate atunci când accesăm IP-ul în browser

//===============================================================
void handleRoot() //functia ce va rula prima data si va afisa pagina web
{
  String s = MAIN_page; //Citim conținutul HTML accesând valoarea din MAIN_page care este definită în index.h;
  server.send(200, "text/html", s); // trimitem pagina web catre server 
}

void handleADC() //functia care manipuleaza interfata atunci cand butonul este apasat
{
  server.send(200, "text/plane", t_state); //trimitem valoarea butonului catre server
}

void handleSwitch()  // functia care manipuleaza interfata atunci cand intrerupatorul este apasat;
{
  String switcher = String(val1);
  server.send(200, "text/plane", switcher); //trimitem valoarea intrerupatorului la server
}
void handleRELAY() //funcționează pentru a trata partea HIGH și LOW a releului;
{
  String ledState = "OFF"; // am definit un șir, care ne arată starea actuală a paginii web;
  t_state = server.arg("RELAYstate"); //referinta catre functia  xhttp.open("GET", "setRELAY?RELAYstate="+led, true);
  Serial.println(t_state);//printam valoarea releului in Monitorul Serial;
  if (t_state == "1")
  {
    digitalWrite(RELAY, LOW); //RELEU ON
    ledState = "ON"; //Parametrul de feedback
  }
  else
  {
    digitalWrite(RELAY, HIGH); //RELEU OFF
    ledState = "OFF"; //Parametrul de feedback
  }
  server.send(200, "text/plane", ledState); //la final, trimitem parametrul catre serverul web
}

//==============================================================
//                  SETUP
//==============================================================
void setup(void) //functia setup
{
  Serial.begin(115200); // începe să asculte Serial pe 115200

  wifiManager.resetSettings(); // resetam setarile wifiManager-ului pentru a putea accesa alte routere.
  WiFiManagerParameter email_text("<p>Please enter your email!</p>"); //cream un nou field pentru a fi afisat in interfata wifiManager
  WiFiManagerParameter custom_email_token("Email", "Enter your email", email_token, 32); //cream un nou parametru de tipul input
  wifiManager.addParameter(&email_text); // adaugam parametrii la wifiManager
  wifiManager.addParameter(&custom_email_token);
  strcpy(email_token, custom_email_token.getValue()); //copiem valorile in obiect;

  if (!wifiManager.autoConnect("SmartLight", "password")) //daca nu se poate conecta
  {
    Serial.println("Eroare de conectare"); // returneaza mesaj de eroare
    delay(3000); // asteapta 3 secunde

    ESP.reset(); //incearca din nou
    delay(5000);
  }
  emailConectare = custom_email_token.getValue(); //stocam valoarea email-ului intr-un string declarat mai sus
  email64 = base64::encode(emailConectare); // convertim continutul valori in Baza 64
  Serial.println(email64); // printam valorile
  Serial.println(custom_email_token.getValue());

  WiFi.begin("!","frigider");     //ne conectam la routerul Wifi
 
  sendEmail(); // apelam functia de trimitere a email-ului dupa ce sa realizat conexiunea la router.

  Serial.println("");

  //Onboard RELAY port Direction output
  digitalWrite(RELAY, 1); // deoarece folosim un releu care este activat în mod redus de putere, setăm valoarea la 1, circuitul fiind inchis;
  pinMode(RELAY, OUTPUT); // setam pinul D5 ca valoare de output, aceasta gestionează valoarea de iesire a releului
  pinMode(SWITCH, OUTPUT); // setam SWITCH ca iesire
  pinMode(SWITCH, HIGH);
  pinMode(inputPin, INPUT); //citim valoarea de la SWITCH, tot timpul trece curent continuu printr-un intrerupator și dacă conexiunea este închisă, aceasta citește 1
  // apoi asteptam sa se realizeze conexiunea
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print("."); // asteptam pana conexiunea se realizeaza
  }

  //Dacă conexiunea se realizeaza cu succes, printam IP-ul pe monitorul serial
  Serial.println("");
  Serial.print("Connected to ");
//  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  //printam adresa IP asignata de catre placa de dezvoltare
  
  server.on("/", handleRoot);      //apelam rutinele create anterior
  server.on("/setRELAY", handleRELAY);
  server.on("/readADC", handleADC);
  server.on("/readSwitch", handleSwitch);

  server.begin();                  // pornim serverul
  Serial.println("HTTP server started");
}
//==============================================================
//                     LOOP FUNCTION
//==============================================================
void loop(void)
{
  server.handleClient();          //Gestioneaza cererile clientului
  val1 = digitalRead(inputPin);//citim valoarea lui inputPin(D3)
  if (val1 == HIGH && state == true) //logica pentru a determina daca intrerupatorul a fost actionat sau nu
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

byte sendEmail() // sendEmail functia de trimitere a email-ului dupa ce serverul s-a conectat la credentiale, aceasta functie este declansata in setup.
{
  byte thisByte = 0;
  byte respCode;

  if (client.connect(Gserver, port) == 1)
  {
    Serial.println(F("connected to email server"));
  }
  else
  {
    Serial.println(F("connection failed"));
    return 0;
  }

  if (!eRcv()) return 0;

  Serial.println(F("Sending hello"));
  client.println("EHLO 192.168.1.10");
  if (!eRcv()) return 0;

  Serial.println(F("Sending auth login"));
  client.println("auth login");
  if (!eRcv()) return 0;

  Serial.println(F("Sending User"));
  client.println("");     //adresa de email codata in baza64

  if (!eRcv()) return 0;

  Serial.println(F("Sending Password"));

  client.println("");     //parola adreseu de enauk in baza 64

  if (!eRcv()) return 0;

  // se schimba cu adresa expeditorului
  Serial.println(F("Sending From"));
  client.println("MAIL From: <IoT for Light>");
  if (!eRcv()) return 0;

  // se schimba cu adresa destinatarului
  Serial.println(F("Sending To"));
  client.println("RCPT To: <"+emailConectare+">");
  if (!eRcv()) return 0;

  Serial.println(F("Sending DATA"));
  client.println("DATA");
  if (!eRcv()) return 0;

  Serial.println(F("Sending email"));

  // change to recipient address
  client.println("<"+emailConectare+">");

  // change to your address
  //  client.println("From: Me <@gmail.com>");

  client.println(F("Subject: Emails from ESp8266\r\n"));

  client.println("Va rog introduce-ti urmatoarea adresa pe browser-ul dumneavoastra \r\n" + WiFi.localIP().toString());

  client.println(".");

  if (!eRcv()) return 0;

  Serial.println(F("Sending QUIT"));
  client.println("QUIT");
  if (!eRcv()) return 0;

  client.stop();

  Serial.println(F("disconnected"));

  return 1;
}

byte eRcv()
{
  byte respCode;
  byte thisByte;
  int loopCount = 0;

  while (!client.available())
  {
    delay(1);
    //Serial.println("Clientul nu este disponibil");
    loopCount++;

    if (loopCount > 20000) {

      client.stop();
      Serial.println(F("\r\nTimeout"));
      return 0;
    }
  }

  respCode = client.peek();

  while (client.available())
  {
    thisByte = client.read();
    Serial.write(thisByte);
  }

  if (respCode >= '4')
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

  while (!client.available())
  {
    delay(1);
    loopCount++;

    if (loopCount > 100000)
    {
      Serial.println("Second while");
      client.stop();
      Serial.println(F("\r\nTimeout"));
      return;
    }
  }

  while (client.available())
  {
    thisByte = client.read();
    Serial.write(thisByte);
  }

  client.stop();

  Serial.println(F("disconnected"));
}
