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

int acc = 240;
int des = 240;

int vitesseG = 0;
int vitesseD = 0;

int vitesseMax=64;
int vitesseMin=20;

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
  attachInterrupt(digitalPinToInterrupt(LH_ENCODER_A), leftEncoderEvent, FALLING);
  //attachInterrupt(digitalPinToInterrupt(RH_ENCODER_A), rightEncoderEvent, CHANGE);
  
  Serial.begin(9600);
}
 
void loop() {
  //Serial.print("Right Count: ");
  //Serial.println(rightCount);
  //Serial.print("Left Count: ");
  //Serial.println(leftCount);
  //Serial.println();

  avancer(928);
  
}



void avancer(int val){

  int vitesse = 0;
  
  Serial.print("Left Count: ");
  Serial.println(leftCount);

  Serial.print("Val: ");
  Serial.println(val);
  
  if (leftCount<val){
    
    /*if (leftCount < acc) {
      vitesse = ((leftCount/acc) * vitesseMax) + vitesseMin;
    }*/
    
    vitesse = vitesseMax;
    
    if (leftCount > val-des) {
      vitesse = (vitesseMax - vitesseMin) / des * leftCount;
    }

    Serial.print("Vitesse: ");
    Serial.println(vitesse);

    digitalWrite(in2,HIGH);
    digitalWrite(in1,LOW);
    digitalWrite(in4,LOW);
    digitalWrite(in3,HIGH);
    analogWrite(ena,vitesse);
    analogWrite(enb,vitesse);
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
