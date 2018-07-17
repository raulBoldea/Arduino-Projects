/////////////////////////////////// Transmitator ////////////////////////////////////////
// Arduino Nano a fost folosit pe post de transmițător cu un modul wireless NRF24L01 +//
// La placa a fost conectat un senzor de gaz MQ-135 care detectează calitatea aerului //
// De aici valoarea este trimisă către receptor                                       //
// Valoarea intergistrata de catre senzor este transformata in parti per milion (PPM) //
////////////////////////////////////////////////////////////////////////////////////////

//fisierul antet pentru interfata seriala periferica(Serial Peripheral Interface )
#include <SPI.h>
//fisierul antet pentru transmisiunea radio
#include "nRF24L01.h" 
// fisierul antet pentru transmisiunea radio
#include "RF24.h" 

// definim pin-ul de intrare
#define anInput A0 
// constanta pentru calcularea calitatii aerului
#define digTrigger 2 
// constanta pentru calcularea calitatii aerului
#define Co2Zero  60 

// Configurația modulului radioul nRF24L01 pe magistrala SPI plus pinii 9 & 10
//instantiere obiect radio pentru valorile (9,10) pentru pini digitali

RF24 radio(9,10); 

const uint64_t pipes[2] = { 0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL }; //crearea canalelor de comunicare pentru cele 2 noduri

int Co2Ppm = 0;
void setup()
{
  Serial.begin(57600); //pornim comunicatia Seriala pentru 57600 Bauds
  
  pinMode (anInput, INPUT); // utilizat pentru citirea valori senzorului
  
  radio.begin(); //pornim transmisiunea radio
  radio.setRetries(15,15); // afiseaza de cate ori modemul va reincerca retransmisia datelor.
  radio.openReadingPipe(1,pipes[1]); // deschidem canalul pentru citire
  radio.startListening(); //Începe să asculți pe canalele deschise spre citire.
  radio.printDetails(); // se printeaza configuratia, necesara pentru debbuging
  radio.openWritingPipe(pipes[0]); //deschidem canalul unde trimitem datele pentru scriere
  radio.openReadingPipe(1,pipes[1]); 
  radio.stopListening(); // se trece modemul la modul de transmitere a datelor.
}

void loop(void)
{
  //Se trece modemul la modul de transmitere a datelor.
  radio.stopListening(); 
  radio.printDetails();
  //calculul pentru valoarea calitatii aerului
  int Co2Now[10]; 
  int Co2Raw = 0;
  int Co2Comp = 0;
  int zzz = 0;
  for (int x = 0; x < 10 ; x++){
    Co2Now[x] = analogRead(A0);
  }
  for(int x = 0 ; x < 10; x++){
    zzz = zzz + Co2Now[x];  
  }
  Co2Raw = zzz/10;
  Co2Comp = Co2Raw - Co2Zero;
  Co2Ppm = map(Co2Comp,0,1023,400,5000);
  //utilizand obiectul radio, scriem valoarea pe care vrem sa 
  //o trimitem si asteptam confirmarea primiri
  radio.write( &Co2Ppm, sizeof(int)); 
  //permitem primirea datelor prin modem. Din acest moment, 
  //modemul așteaptă datele trimise la adresa specificată.
  radio.startListening(); 
  Serial.println(Co2Ppm);
  delay(1000);
}
