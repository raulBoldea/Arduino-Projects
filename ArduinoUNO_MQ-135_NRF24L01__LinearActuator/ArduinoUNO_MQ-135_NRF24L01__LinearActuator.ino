//////////////////////////////////// Receiver ////////////////////////////////////
//                                                                              //
// Arduino Uno used as an receiver with an NRF24L01 wireless module             //
// The board has connected an MQ-135 Gas sensor which detects Air Quality       //
// If Air Quality is poor an linear actuator it's activated                     //
// The linear actuator has the roll to open something like a window             //
// Made by - Boldea Raul                                                        //
//                                                                              //
//////////////////////////////////////////////////////////////////////////////////

#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"

#define anInput A0
#define digTrigger 2
#define Co2Zero  60

const int forwards = 7;
const int backwards = 6;
RF24 radio(9,10);

int Co2Ppm = 0;
int Count = 0;
boolean state;
const uint64_t pipes[2] = { 0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL };

typedef enum { role_ping_out = 1, role_pong_back } role_e;

const char* role_friendly_name[] = { "invalid", "Ping out", "Pong back"};

role_e role = role_pong_back;

void setup(void)
{
  Serial.begin(57600);
  pinMode(forwards, OUTPUT);//set relay as an output
  pinMode(backwards, OUTPUT);//set relay as an output
  state = false;
  radio.begin();
  radio.setRetries(15,15);
  radio.openReadingPipe(1,pipes[1]);
  radio.startListening();
  radio.printDetails();
  radio.openWritingPipe(pipes[1]);
  radio.openReadingPipe(1,pipes[0]);
  radio.startListening();
}

void loop(void)
{
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
    //    unsigned long data = 0;
    //int senzorData = 0;
    //    radio.read( &data, sizeof(unsigned long) );
    radio.printDetails();
    radio.read( &senzorData, sizeof(int) );
//    Serial.println("Transmitter senzor value");
//    Serial.println(senzorData);
  }
  
//  Serial.println("Receiver Sensor Value is: ");
//  Serial.println(Co2Ppm);
//  Serial.println(state);
  if(senzorData < 400 && Co2Ppm < 450 && state == true)
  {
    Serial.println("The air is at normal values");
    Count = 0;
    digitalWrite(forwards, HIGH);
    digitalWrite(backwards, HIGH);
  }
  else if(Co2Ppm > 500 && senzorData < 500 && state == true) 
  {
    Serial.println("Open the Window you might get intoxicated inside");
    Count++;
    if(Count == 10)
    {
      Serial.println("The air outside is too poluted");
      
      Count = 0;
      state = false;
      
      digitalWrite(forwards, HIGH);
      digitalWrite(backwards, LOW);//Activate the relay the other direction, they must be different to move the motor
      Serial.println("Second way");
      delay(3000);// wait 3 seconds
  
      digitalWrite(forwards, HIGH);
      digitalWrite(backwards, HIGH);//Deactivate both relays to brake the motor
    }
  }
  else if (senzorData > 500 && state == false)
  {
    state = true;
    
    digitalWrite(forwards, LOW);
    digitalWrite(backwards, HIGH);//Activate the relay one direction, they must be different to move the motor
    Serial.println("One way");
    delay(3000); // wait 3 seconds
    
    digitalWrite(forwards, HIGH);
    digitalWrite(backwards, HIGH);//Deactivate both relays to brake the motor 
    
    Count = 0;
  }
  else
  {
    Serial.println("The air is at normal values2");
    Count = 0;
    
    digitalWrite(forwards, HIGH);
    digitalWrite(backwards, HIGH);
  }
  delay(1100);
}
