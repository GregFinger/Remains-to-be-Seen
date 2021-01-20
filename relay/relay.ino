#include <limits.h>

void setup()
{
  Serial.begin(57600);
  Serial.setTimeout(LONG_MAX);
  delay(500);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);  
}

void loop()
{
  if (Serial.available() > 0) {
    int Toggle1;
    Toggle1 = Serial.read();
    digitalWrite(7, Toggle1);
    digitalWrite(6, Toggle1);
    digitalWrite(5, Toggle1);
    digitalWrite(4, Toggle1);    
  }
}
