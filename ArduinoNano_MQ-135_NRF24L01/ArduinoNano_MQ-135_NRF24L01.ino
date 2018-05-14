/////////////////////////////////// Transmitter //////////////////////////////////
//                                                                              //
// Arduino Nano used as an Transmitter with an NRF24L01+ wireless module        //
// The board has connected an MQ-135 Gas sensor which detects Air Quality       //
// The value is send to the Receiver where some conditions need to be fullfiled //
// Senzor Data it's transformed into PPM's value                                //
// Made by - Boldea Raul                                                        //
//                                                                              //
//////////////////////////////////////////////////////////////////////////////////

#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"

#define anInput A0
#define digTrigger 2
#define Co2Zero  60
RF24 radio(9,10);

const uint64_t pipes[2] = { 0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL };
//unsigned long Command = 1;
int Co2Ppm = 0;
void setup()
{
  Serial.begin(57600);
  
  pinMode (anInput, INPUT);
  
  radio.begin();
  radio.setRetries(15,15);
  radio.openReadingPipe(1,pipes[1]);
  radio.startListening();
  radio.printDetails();
  radio.openWritingPipe(pipes[0]);
  radio.openReadingPipe(1,pipes[1]);
  radio.stopListening();
}

void loop(void)
{
  radio.stopListening();
  radio.printDetails();
  //radio.write( &Command, sizeof(unsigned long) );
  
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
  Co2Ppm = map(Co2Comp,0,1023,400,5000);
  radio.write( &Co2Ppm, sizeof(int));
  radio.startListening();
  //Serial.println(Command);
  Serial.println(Co2Ppm);
  delay(1000);
}
