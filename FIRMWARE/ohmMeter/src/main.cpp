#include <Arduino.h>

///Arduino autoranging ohmmeter with electromagnet

//Pin definitions
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
float stuffer;



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

void magnetOn(){
  digitalWrite(2, LOW); 
  digitalWrite(3, HIGH);
  digitalWrite(4, LOW);
}
void setup(void) {
  Serial.begin(9600);
  pinMode(CH0, OUTPUT);
  pinMode(CH1, OUTPUT);
  pinMode(CH2, OUTPUT);
  pinMode(CH3, OUTPUT);
  pinMode(CH4, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  digitalWrite(2, LOW); 
  digitalWrite(3, HIGH);
  digitalWrite(4, LOW);
  ch_number = 4;
  ch_select(ch_number);
 
}


void electroMagnet(){
  digitalWrite(2, LOW); 
  digitalWrite(3, HIGH);
  digitalWrite(4, HIGH);
  Serial.print("MAGNET ON");
  Serial.println();
  delay(10000);
  digitalWrite(2, LOW); 
  digitalWrite(3, HIGH);
  digitalWrite(4, LOW);
  Serial.print("MAGNET OFF");
  Serial.println();
  delay(1000);

}

// main loop
void loop() {
  float resistorReads[5],sum=0;
  
  // loop to find average of 5 readings
  for (int i=0;i<5;i++){

    uint16_t volt_image = analogRead(A1) + 1;

    if(volt_image >= 550 && ch_number < 4) {
      ch_number++;
      ch_select(ch_number);
      delay(50);
      return;
    }
  
    if(volt_image <= 90 && ch_number > 0) {
      ch_number--;
      ch_select(ch_number);
      delay(50);
      return;
    }
    float value;
    if(volt_image < 900) {
      value = (float)volt_image*res/(1023 - volt_image);
      
      
    }
  
    else
      value=0;
    if (value!=0){
        resistorReads[i]=value;
        sum += resistorReads[i];
          //  for debugging purpose
          if (i==0){
            Serial.print("Reading");
          }
        Serial.print(".");
    }

    delay(500);   // wait some time
  }
  
  if (sum>0){
    Serial.print("AVERAGE = ");
    Serial.println(sum / 5);
    Serial.println();
    electroMagnet();
  }
 
}

 

