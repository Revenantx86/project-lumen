#include <Arduino.h>

// put function declarations here:
int myFunction(int, int);

void setup() {
  // put your setup code here, to run once:
  int result = myFunction(2, 3);

  // Initalize serial module
  Serial.begin(9600); //begin Serial Communication

}

void loop() {
  // put your main code here, to run repeatedly:
  int light_sensor = A3; 

 
  int raw_light = analogRead(light_sensor); // read the raw value from light_sensor pin (A3)
  int light = map(raw_light, 0, 1023, 0, 1023); // map the value from 0, 1023 to 0, 100
 
  if(Serial.available())
  {
    Serial.println(light); // print the light value in Serial Monitor
    delay(100); // add a delay to only read and print every 1 second
  }
  //Serial.print("Light level: "); 
  
}

// put function definitions here:
int myFunction(int x, int y) {
  return x + y;
}