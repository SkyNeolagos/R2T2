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

volatile unsigned long leftCount = 0.0;
volatile unsigned long rightCount = 0.0;

float acc = 240.0;//distance d'acceleration
float des = 240.0;//distance de décélération

int vitesseG = 0;//vitesse de départ de la roue gauche
int vitesseD = 0;//vitesse de départ de la roue droite

float vitesseMax=64.0;
float vitesseMin=25.0;

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

  avancer(928.0);
  
}



void avancer(float val){

  int vitesse = 0;
  
  Serial.print("Left Count: ");
  Serial.println(leftCount);

  Serial.print("Val: ");
  Serial.println(val);
  
  if (leftCount<val){
    
    if (leftCount < acc) {
      float ponderateurVitesseAcc=((leftCount/acc) * vitesseMax);//ponderateur de la vitesse 
      vitesse = ponderateurVitesseAcc + vitesseMin;//incrementation de la vitesse avec le ponderateur
      if(vitesse>vitesseMax){
        vitesse=vitesseMax;
      }
    }
    else if (leftCount > val-des) {
      float ponderateurVitesseDes=((val-leftCount)/des * vitesseMax);//ponderateur de la vitesse 
      vitesse = ponderateurVitesseDes/1.1 + vitesseMin;//incrementation de la vitesse avec le ponderateur
      if(vitesse>vitesseMax){
        vitesse=vitesseMax;
      }
      //vitesse = (vitesseMax - vitesseMin) / des * leftCount;
    }
    else{
      vitesse = vitesseMax;
    }
  }
  else{
    vitesse=0;
  }
  Serial.print("Vitesse: ");
  Serial.println(vitesse);

  //digitalWrite(in2,HIGH);
  //digitalWrite(in1,LOW);
  digitalWrite(in4,LOW);
  digitalWrite(in3,HIGH);
  //analogWrite(ena,vitesse);
  analogWrite(enb,vitesse);
}  
 
// encoder event for the interrupt call
void leftEncoderEvent() {
  if (digitalRead(LH_ENCODER_A) == HIGH) {
    if (digitalRead(LH_ENCODER_B) == LOW) {
      leftCount--;
    } else {
      leftCount++;
    }
  } else {
    if (digitalRead(LH_ENCODER_B) == LOW) {
      leftCount++;
    } else {
      leftCount--;
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
