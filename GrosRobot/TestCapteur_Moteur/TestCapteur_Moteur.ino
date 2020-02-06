#include <SharpIR.h>


#define ena 8   ///Pin pour le controle des moteurs 
#define in1 9
#define in2 10
#define enb 6
#define in3 4
#define in4 5


#include <math.h>

const int PatternCount = 8;  //paramètres pour le réseau neuronal
const int InputNodes = 3;    //
const int HiddenNodes = 4;   //
const int OutputNodes = 2;   //

const float Input[PatternCount][InputNodes] = {         //Dataset pour l'entrainement du réseau neuronal
  { 0, 0, 0 },  // 0   {Gauche,devant,droite}
  { 0, 0, 1 },  // 1
  { 0, 1, 0 },  // 2
  { 0, 1, 1 },  // 3
  { 1, 0, 0 },  // 4
  { 1, 0, 1 },  // 5
  { 1, 1, 0 },  // 6
  { 1, 1, 1 }   // 7 
}; 

const float Target[PatternCount][OutputNodes] = {  //Matrice des sorties souhaitées du réseau neuronal 
  { 1, 1},  
  { 0.5, 1}, 
  { 0, 0}, 
  { 0.5, 0}, 
  { 1, 0.5}, 
  { 1, 1}, 
  { 0, 0.5}, 
  { 0, 0} 
};

const float HiddenWeights[InputNodes][HiddenNodes] = {    //Matrice de poids des nodes cachés 
    {4.80420380032, -3.31803870729, 3.659635567, 4.48234257054},
    {-7.46613721969, -4.97507509531, 6.78329351675, 0.143357463605},
    {-4.35918584147, 2.70760798004, -4.88714897457, -4.47254443167}
};

const float OutputWeights[HiddenNodes+1][OutputNodes] = {   //Matrice de poids des nodes de sortie 
    {17.5152427029, -0.6313397373},
    {-0.129783857661, 9.9009805956},
    {0.142179588906, -10.3748590339},
    {-8.77141418999, 10.5129521087}
};

int i, j, p, q, r;
float Accum;

float Hidden[HiddenNodes];
float Output[OutputNodes];   //Sortie du réseau neuronal

void InputToOutput(float In1, float In2, float In3)  //Fonction de conversion d'entré en sortie du réseau neuronal
{
  float TestInput[] = {0, 0, 0};
  TestInput[0] = In1;
  TestInput[1] = In2;
  TestInput[2] = In3;

  /******************************************************************
    Compute hidden layer activations
  ******************************************************************/

  for ( i = 0 ; i < HiddenNodes ; i++ ) {                //
    Accum = HiddenWeights[InputNodes][i] ;               // Multiplie la matrice d'entré par la matrice de poids cachés et réalise la sigmoïd
    for ( j = 0 ; j < InputNodes ; j++ ) {               //
      Accum += TestInput[j] * HiddenWeights[j][i] ;      //  
    }                                                    //
    Hidden[i] = 1.0 / (1.0 + exp(-Accum)) ;              //
  }

  /******************************************************************
    Compute output layer activations and calculate errors
  ******************************************************************/

  for ( i = 0 ; i < OutputNodes ; i++ ) {
    Accum = OutputWeights[HiddenNodes][i] ;
    for ( j = 0 ; j < HiddenNodes ; j++ ) {
      Accum += Hidden[j] * OutputWeights[j][i] ;       //Multiplie la matrice précedente par la matrice de poid de sortie et réalise la sigmoïd
    }
    Output[i] = 1.0 / (1.0 + exp(-Accum)) ;
  }
  Serial.print ("  Output ");
  for ( i = 0 ; i < OutputNodes ; i++ ) {
    Serial.print (Output[i], 5);
    Serial.print(" ");
  }
  Serial.println(" ");
}

int ReportEvery1000;


int distance_cmAvant;                  //
int distance_cmDroite;                 // Distance entre le capteur et les obstacles
int distance_cmGauche;                 //

float coeffMotorG;                    // coefficient d'accélération des motuers
float coeffMotorD;

byte dGauche;
byte dAvant;
byte dDroite;

float fGauche;
float fAvant;
float fDroite;


#define IRPin1 A0                //pin des télémètres
#define model1 1080              //
#define IRPin2 A1                // 
#define model2 430               //
#define IRPin3 A2                //
#define model3 1080              //

/* Model :
  GP2Y0A02YK0F --> 20150
  GP2Y0A21YK0F --> 1080
  GP2Y0A710K0F --> 100500
  GP2YA41SK0F --> 430
*/


SharpIR sensorAvant = SharpIR(IRPin3, model3);   //
SharpIR sensorGauche = SharpIR(IRPin2, model2);  //création des objets capteurs
SharpIR sensorDroite = SharpIR(IRPin1, model1);  //



void setup() {
/*                               // Modifie la fréquence du timer 4 qui contorle les pin PWM 8,9,10s
  int myEraser = 7;             // this is 111 in binary and is used as an eraser
  TCCR4B &= ~myEraser;  
  int myPrescaler = 3;         // this could be a number in [1 , 6]. In this case, 3 corresponds in binary to 011.   
  TCCR4B |= myPrescaler;  //this operation (OR), replaces the last three bits in TCCR2B with our new value 011
 */ 
  Serial.begin(9600);
  pinMode(ena,OUTPUT);        //set les modes des pin 
  pinMode(enb,OUTPUT);
  pinMode(in1,OUTPUT);
  pinMode(in2,OUTPUT);
  pinMode(in3,OUTPUT);
  pinMode(in4,OUTPUT);
}

void loop() {

  
  distance_cmAvant = sensorAvant.distance();   //récupère les distances renvoyées par les télémètres
  distance_cmGauche = sensorGauche.distance();
  distance_cmDroite = sensorDroite.distance();

  printDebug(distance_cmGauche,distance_cmAvant,distance_cmDroite);

  if (distance_cmGauche < 10){distance_cmGauche = 10;}
  if (distance_cmGauche > 80){distance_cmGauche = 80;}
  if (distance_cmDroite < 10){distance_cmDroite = 10;}     //set les valeurs min et max des capteurs entre 7 et 80 cm  
  if (distance_cmDroite > 80){distance_cmDroite = 80;}
  if (distance_cmAvant < 10){distance_cmAvant = 10;}
  if (distance_cmAvant > 80){distance_cmAvant = 80;}
  
  dGauche=map(distance_cmGauche,7,80,0,255);   //transforme les valeurs de distance en valeurs comprise entre 0 et 255
  dAvant=map(distance_cmAvant,7,80,0,255);
  dDroite=map(distance_cmDroite,7,80,0,255);

  fGauche=dGauche/255.0;  //divise les valeurs par 255 pour quelle soit comprise entre 0.0 et 1.0 
  fAvant=dAvant/255.0;
  fDroite=dDroite/255.0;
  
  Serial.print("fGauche");
  Serial.println(fGauche); 
  

  InputToOutput(fGauche,fAvant,fDroite); //transforme les valeurs des capteurs en valeur de controle des moteurs 

  coeffMotorG=Output[0];  //récupère les valeurs de sortie
  coeffMotorD=Output[1];

  Serial.print("coeffG");
  Serial.println(coeffMotorG); 
  Serial.print("coeffD");
  Serial.println(coeffMotorD); 

  //digitalWrite(in1,LOW);   //sens rotation avant
  //digitalWrite(in2,HIGH);
  //digitalWrite(in3,HIGH);
  //digitalWrite(in4,LOW);

  if (coeffMotorG <0.5){         //controle le sens et la vitesse du moteur avec un offset 
    digitalWrite(in3,LOW);
    digitalWrite(in4,HIGH);
    analogWrite(enb,(1-coeffMotorG+0.2)*64);
  }
  else if (coeffMotorG > 0.5){
    digitalWrite(in3,HIGH);
    digitalWrite(in4,LOW);
    analogWrite(enb,(coeffMotorG+0.2)*64);
  }

  if (coeffMotorD <0.5){
    digitalWrite(in1,LOW);
    digitalWrite(in2,HIGH);
    analogWrite(ena,(1-coeffMotorD+0.2)*64);
  }
  else if (coeffMotorD > 0.5){
    digitalWrite(in1,HIGH);
    digitalWrite(in2,LOW);
    analogWrite(ena,(coeffMotorD+0.2)*64);
  }
  
  delay(100); //à remplacer avec la library timer.h
}

void printDebug(int distance_cm1,int distance_cm2,int distance_cm3){ //Imprime les valeurs des distances des télémètres sur le port série pour debug
  
  Serial.print("Gauche :");
  Serial.print(distance_cm1);
  Serial.print(" cm |");
  Serial.print("Avant :");
  Serial.print(distance_cm2);
  Serial.print(" cm |");
  Serial.print("Droite :");
  Serial.print(distance_cm3);
  Serial.println(" cm");
}
