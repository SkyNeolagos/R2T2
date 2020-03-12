#include <math.h>

#define RH_ENCODER_A 21 
#define RH_ENCODER_B 20
#define LH_ENCODER_A 18
#define LH_ENCODER_B 19

#define ena 8
#define in1 9
#define in2 10
#define enb 6
#define in3 5
#define in4 4

volatile unsigned long leftCount = 0;
volatile unsigned long rightCount = 0;

int vitesseG = 0;
int vitesseD = 0;

int vitesseMax=64;
int vitesseMin=14;

int motor_steps = 960;  
int del = 0;            
int ampl = 0;           
int lift = 0;  

 
void setup() {
  pinMode(LH_ENCODER_A, INPUT);
  pinMode(LH_ENCODER_B, INPUT);
  pinMode(RH_ENCODER_A, INPUT);
  pinMode(RH_ENCODER_B, INPUT);
  pinMode(ena,OUTPUT);
  pinMode(enb,OUTPUT);
  pinMode(in1,OUTPUT);
  pinMode(in2,OUTPUT);
  pinMode(in3,OUTPUT);
  pinMode(in4,OUTPUT);
  
  // initialize hardware interrupts
  attachInterrupt(digitalPinToInterrupt(LH_ENCODER_A), leftEncoderEvent, CHANGE);
  attachInterrupt(digitalPinToInterrupt(RH_ENCODER_A), rightEncoderEvent, CHANGE);
  
  Serial.begin(9600);
}
 
void loop() {
  Serial.print("Right Count: ");
  Serial.println(rightCount);
  Serial.print("Left Count: ");
  Serial.println(leftCount);
  Serial.println();

  avancer();
  
}



void avancer(){

  motor_steps=960;

  if (leftCount<960){
      ampl = (vitesseMin-vitesseMax)/2;
      lift = (vitesseMin-vitesseMax)/2 + 2;
     
      del = vitesseMax*(float(motor_steps-leftCount)/motor_steps)+vitesseMin;

      Serial.println(vitesseMax);
      Serial.println(motor_steps-leftCount);
      Serial.println(del);

      digitalWrite(in2,HIGH);
      digitalWrite(in1,LOW);
      digitalWrite(in4,LOW);
      digitalWrite(in3,HIGH);
      analogWrite(ena,del);
      analogWrite(enb,del);
  }
}  
 
// encoder event for the interrupt call
void leftEncoderEvent() {
  if (digitalRead(LH_ENCODER_A) == HIGH) {
    if (digitalRead(LH_ENCODER_B) == LOW) {
      leftCount++;
    } else {
      leftCount--;
    }
  } else {
    if (digitalRead(LH_ENCODER_B) == LOW) {
      leftCount--;
    } else {
      leftCount++;
    }
  }
}
 
// encoder event for the interrupt call
void rightEncoderEvent() {
  if (digitalRead(RH_ENCODER_A) == HIGH) {
    if (digitalRead(RH_ENCODER_B) == LOW) {
      rightCount++;
    } else {
      rightCount--;
    }
  } else {
    if (digitalRead(RH_ENCODER_B) == LOW) {
      rightCount--;
    } else {
      rightCount++;
    }
  }
}
