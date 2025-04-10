void setup() 
{
  Serial.begin(115200);
}

void loop() 
{
  delay(500);
  Serial.write(0x01);
  delay(500);
  Serial.write(0x07);
  delay(500);
  Serial.write(0x1f);
}