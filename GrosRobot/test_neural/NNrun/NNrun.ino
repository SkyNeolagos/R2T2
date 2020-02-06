#include <math.h>

const int PatternCount = 8;
const int InputNodes = 3;
const int HiddenNodes = 4;
const int OutputNodes = 2;

const float Input[PatternCount][InputNodes] = {
  { 0, 0, 0 },  // 0   {Gauche,devant,droite}
  { 0, 0, 1 },  // 1
  { 0, 1, 0 },  // 2
  { 0, 1, 1 },  // 3
  { 1, 0, 0 },  // 4
  { 1, 0, 1 },  // 5
  { 1, 1, 0 },  // 6
  { 1, 1, 1 }   // 7 
}; 

const float Target[PatternCount][OutputNodes] = {
  { 1, 1},  
  { 0.5, 1}, 
  { 0, 0}, 
  { 0.5, 0}, 
  { 1, 0.5}, 
  { 1, 1}, 
  { 0, 0.5}, 
  { 0, 0} 
};

const float HiddenWeights[InputNodes][HiddenNodes] = {
    {4.80420380032, -3.31803870729, 3.659635567, 4.48234257054},
    {-7.46613721969, -4.97507509531, 6.78329351675, 0.143357463605},
    {-4.35918584147, 2.70760798004, -4.88714897457, -4.47254443167}
};

const float OutputWeights[HiddenNodes+1][OutputNodes] = {
    {17.5152427029, -0.6313397373},
    {-0.129783857661, 9.9009805956},
    {0.142179588906, -10.3748590339},
    {-8.77141418999, 10.5129521087}
};

int i, j, p, q, r;
float Accum;

float Hidden[HiddenNodes];
float Output[OutputNodes];

void InputToOutput(float In1, float In2, float In3)
{
  float TestInput[] = {0, 0, 0};
  TestInput[0] = In1;
  TestInput[1] = In2;
  TestInput[2] = In3;

  /******************************************************************
    Compute hidden layer activations
  ******************************************************************/

  for ( i = 0 ; i < HiddenNodes ; i++ ) {
    Accum = HiddenWeights[InputNodes][i] ;
    for ( j = 0 ; j < InputNodes ; j++ ) {
      Accum += TestInput[j] * HiddenWeights[j][i] ;
    }
    Hidden[i] = 1.0 / (1.0 + exp(-Accum)) ;
  }

  /******************************************************************
    Compute output layer activations and calculate errors
  ******************************************************************/

  for ( i = 0 ; i < OutputNodes ; i++ ) {
    Accum = OutputWeights[HiddenNodes][i] ;
    for ( j = 0 ; j < HiddenNodes ; j++ ) {
      Accum += Hidden[j] * OutputWeights[j][i] ;
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

void setup(){
  Serial.begin(9600);
  ReportEvery1000 = 0;
}  

void loop(){

    if (ReportEvery1000==30000){
      ReportEvery1000=0;
      InputToOutput(1,1,1);
    }
    ReportEvery1000=ReportEvery1000+1;
}
