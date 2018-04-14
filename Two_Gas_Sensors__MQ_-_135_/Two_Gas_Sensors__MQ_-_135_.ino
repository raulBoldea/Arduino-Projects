/* Th0000002122222222222231is program will read the air quality from an MQ-135 sensor
*28/03/2018
*Boldea Raul 
*
*/
int sensorValue1, sensorValue2;
int ledRosu = 5;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("-----------");
  Serial.println("Hello");
  pinMode(ledRosu, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("-----------------");

  sensorValue1 = analogRead(A0);
  sensorValue2 = analogRead(A1);
  Serial.print("Sensor1 =");
  Serial.println(sensorValue1);
  
  Serial.print("Sensor2 =");
  Serial.println(sensorValue2);
  if(sensorValue1 == sensorValue2)
  {
  digitalWrite(ledRosu, HIGH);
  }
  else
  {
    digitalWrite(ledRosu, LOW);
    }
  delay(5000);
}
