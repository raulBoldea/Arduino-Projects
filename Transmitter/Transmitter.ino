#include <SPI.h>
#include "RF24.h"
#include "nRF24L01.h"
#include "printf.h"

RF24 myRadio (7,8);
const uint64_t pipe = 0xB01DFACECEL;
//int sensorValue;
struct package
{
  int id=1;
  int sensorValue = 0;
  int code = 2;
  char text[100] = "text";
};

typedef struct package Package;
Package data;

void setup() {
 Serial.begin(9600);
 printf_begin();
 delay(1000);

 myRadio.begin();
 myRadio.setChannel(115);
 myRadio.openWritingPipe(pipe);
 myRadio.setRetries(15,15);
 myRadio.printDetails();
 myRadio.stopListening();
 Serial.println("Set-Up Done");
 delay(1000);
}

void loop() {
   if(!myRadio.write(&data, sizeof(data)))
   {
    Serial.println("error!!");
//    myRadio.printDetails();
//    sensorValue = analogRead(A0);
   }
   Serial.print("\nPackage Sent");
   Serial.println(data.id);
   data.sensorValue = analogRead(A0);
   Serial.println(data.sensorValue);
   Serial.println(data.code);
   Serial.println(data.text);
//   sensorValue = analogRead(A0);
   data.id += 1;
   data.code += 1;
   delay(500);
}
