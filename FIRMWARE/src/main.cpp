#include <Arduino.h>

#define CH0  12
#define CH1  11
#define CH2  10
#define CH3  9
#define CH4  8
 
// variables
byte ch_number;
uint32_t res;
const uint32_t res_table[5] = {100, 1000, 10000, 100000, 2000000};
char _buffer[11];
 
void setup(void) {
  Serial.begin(9600);
 
 
  pinMode(CH0, OUTPUT);
  pinMode(CH1, OUTPUT);
  pinMode(CH2, OUTPUT);
  pinMode(CH3, OUTPUT);
  pinMode(CH4, OUTPUT);
 
  ch_number = 4;
  ch_select(ch_number);
 
}
 
// main loop
void loop() {
  float readings[5];
  // here we take the average of 5 readings to smooth out the data.

  for (int i=0;i<5;i++){
    readings[i] = ohmMeter();
  }
  float average = 0;
  for (int i=0;i<5;i++){
    average += readings[i];
  }
  average /= 5;
  Serial.print("Average: ");
  Serial.println(average);
 
}






float ohmMeter(){
  uint16_t volt_image = analogRead(A1) + 1;
   float rawValue;
 

  //scan up through each channel
  if(volt_image >= 550 && ch_number < 4) {
    ch_number++;
    ch_select(ch_number);
    delay(50);                            //BLOCKING DELAY
    return;
  }
  //scan down through each channel
  if(volt_image <= 90 && ch_number > 0) {
    ch_number--;
    ch_select(ch_number);
    delay(50);                            //BLOCKING DELAY
    return;
  }
  
  if(volt_image < 900) {
    float value = (float)volt_image*res/(1023 - volt_image);
    if(value < 1000.0)
      sprintf(_buffer, "%03u.%1u", (uint16_t)value, (uint16_t)(value*10)%10);
      
      rawValue=String(_buffer).toFloat();

  }
 
  else
    sprintf(_buffer, "Over Load ");
 
  Serial.println(_buffer);
  Serial.println();
 
  delay(500);   // wait some time                            //BLOCKING DELAY

  return rawValue;
}
 
void ch_select(byte n) {
  switch(n) {
    case 0:
      digitalWrite(CH0, LOW);
      digitalWrite(CH1, HIGH);
      digitalWrite(CH2, HIGH);
      digitalWrite(CH3, HIGH);
      digitalWrite(CH4, HIGH);
      break;
    case 1:
      digitalWrite(CH0, HIGH);
      digitalWrite(CH1, LOW);
      digitalWrite(CH2, HIGH);
      digitalWrite(CH3, HIGH);
      digitalWrite(CH4, HIGH);
      break;
    case 2:
      digitalWrite(CH0, HIGH);
      digitalWrite(CH1, HIGH);
      digitalWrite(CH2, LOW);
      digitalWrite(CH3, HIGH);
      digitalWrite(CH4, HIGH);
      break;
    case 3:
      digitalWrite(CH0, HIGH);
      digitalWrite(CH1, HIGH);
      digitalWrite(CH2, HIGH);
      digitalWrite(CH3, LOW);
      digitalWrite(CH4, HIGH);
      break;
    case 4:
      digitalWrite(CH0, HIGH);
      digitalWrite(CH1, HIGH);
      digitalWrite(CH2, HIGH);
      digitalWrite(CH3, HIGH);
      digitalWrite(CH4, LOW);
  }
  res = res_table[n];
}
 
// end of code.