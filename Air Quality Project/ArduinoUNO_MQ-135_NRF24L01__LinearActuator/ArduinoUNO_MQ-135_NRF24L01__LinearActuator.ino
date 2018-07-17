//////////////////////////////////// Receptor ////////////////////////////////////
// Arduino Uno folosit ca receptor cu un modul wireless NRF24L01                //
// La placa a conectat un senzor de gaz MQ-135 care detectează calitatea aerului//
// Dacă calitatea aerului este scăzută un actuator liniar este activat          //
// Elementul de acționare liniar are rolul de a deschide o fereastră            //
//////////////////////////////////////////////////////////////////////////////////
//fisierul antet pentru interfata seriala periferica de comunicare(Serial Peripheral Interface )
#include <SPI.h>
//fisierul antet pentru transmisiunea radio 
#include "nRF24L01.h"
// fisierul antet pentru transmisiunea radio
#include "RF24.h"

// definim pin-ul de intrare pentru senzor
#define anInput A0 
// constanta pentru calcularea calitatii aerului
#define digTrigger 2 
// constanta pentru calcularea calitatii aerului
#define Co2Zero  60 

// constanta de tip intreg utlizata la releu
const int inainte = 7;
// constanta de tip intreg utilizata la releu 
const int inapoi = 6; 

// Configurația modulului radioul nRF24L01 pe magistrala SPI plus pinii 9 & 10
//instantiere obiect radio pentru valorile (9,10) pentru pini digitali
RF24 radio(9,10); 

int LED = 5;
int input = 0;
int Co2Ppm = 0;
int Count = 0;

boolean state;

const uint64_t pipes[2] = { 0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL }; //crearea canalelor de comunicare pentru cele 2 noduri
typedef enum { role_ping_out = 1, role_pong_back } role_e; // Diferitele roluri susținute de aceasta schita
const char* role_friendly_name[] = { "invalid", "Ping out", "Pong back"}; // Numele rolurilor pentru debug
role_e role = role_pong_back; // rolul curent utilizat

void setup(void)
{
  Serial.begin(57600); //pornim comunicatia Seriala pentru 57600 Bauds
  pinMode(inainte, OUTPUT); //utilizat pentru trimiterea valorilor catre releu
  pinMode(inapoi, OUTPUT);  //utilizat pentru trimiterea valorilor catre releu
  state = false;
   //transmitem valoarea HIGH pe pinul 7 pentru ca releu sa fie in starea inactiva
  digitalWrite(inainte, HIGH);
  //transmitem valoarea HIGH pe pinul 6 pentru ca releu sa fie in starea inactiva
  digitalWrite(inapoi, HIGH);  
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);
  radio.begin(); //pornim transmisiunea radio
  radio.setRetries(15,15); // afiseaza de cate ori modemul va reincerca retransmisia datelor.
  radio.openReadingPipe(1,pipes[1]); // deschidem canalul pentru citire
  radio.startListening();  // se printeaza configuratia, necesara pentru debbuging
  radio.printDetails();
  radio.openWritingPipe(pipes[1]); //deschidem canalul unde trimitem datele pentru scriere
  radio.openReadingPipe(1,pipes[0]);
  radio.startListening(); // se trece modemul la modul de recepție a datelor.
}

void loop(void)
{
  //calculul pentru valoarea calitatii aerului
  int Co2Now[10];
  int Co2Raw = 0;
  int Co2Comp = 0;
  int zzz = 0;
  for (int x = 0; x < 10 ; x++)
  {
    Co2Now[x] = analogRead(A0);
    //  delay(200);
  }
  for(int x = 0 ; x < 10; x++)
  {
    zzz = zzz + Co2Now[x];  
  }
  Co2Raw = zzz/10;
  Co2Comp = Co2Raw - Co2Zero;
  Co2Ppm = map(Co2Comp,0,1023,410,5000);
  int senzorData = 0;
  if ( radio.available() )
  {
    radio.printDetails();
    radio.read( &senzorData, sizeof(int) );
  }
  if(senzorData < 400 && Co2Ppm < 450)
  {
    Serial.println("Calitatea aerului este la valori normale");
    Count = 0;
    digitalWrite(inainte, HIGH);
    digitalWrite(inapoi, HIGH);
  }
  else if(Co2Ppm > 500 && senzorData < 500) 
  {
    Serial.println("Calitatea aerului din incapere este periculoasa !");
    Count++;
    if(state == false)
    {
      if(Count == 10)
      {
           Count = 0;
          digitalWrite(inainte, HIGH);
          digitalWrite(inapoi, LOW);
          delay(3000);// asteapta 3 secunde

          state = true;
          digitalWrite(inainte, HIGH);
          digitalWrite(inapoi, HIGH);//Dezactivați ambele relee pentru a frâna motorul
          Serial.println("Calitatea aerului din incapere este periculoasa  !");
          Serial.println("Calitatea aerului din incapere este periculoasa !");

                  }
    }
  }
  else if (senzorData > 500)
  {
    Serial.println("Calitatea aerului din exterior este rea!");
    Count++;
    if(state == true)
    {
      if(Count == 10)
      {
        Count = 0;
        state = false;
        digitalWrite(inainte, LOW);
        digitalWrite(inapoi, HIGH);
        delay(3000); // asteapta 3 secunde
        Count = 0;
        digitalWrite(inainte, HIGH);
        digitalWrite(inapoi, HIGH);//Dezactivați ambele relee pentru a frâna motorul 
        Serial.println("Calitatea aerului din exterior este rea !");
        Serial.println("Calitatea aerului din exterior este rea!");
      }
    }
    
  }
  delay(1100);
    if (Serial.available())
  {
    input = Serial.parseInt();
    if (input == 1)//retragere motor
    {
        digitalWrite(inainte, HIGH);
        digitalWrite(inapoi, LOW);
        delay(3000);// wait 3 seconds
        digitalWrite(inainte, HIGH);
        digitalWrite(inapoi, HIGH);
        state = true;
    }
    else if( input ==2)//actionare motor
    {
      digitalWrite(inainte, LOW);
      digitalWrite(inapoi, HIGH);
      delay(3000);
      digitalWrite(inainte, HIGH);
      digitalWrite(inapoi, HIGH);
       state = false;
    }
  }
}
