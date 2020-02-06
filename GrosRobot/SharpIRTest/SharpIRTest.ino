
#include <SharpIR.h>

#define IRPin1 A0
#define model1 430
#define IRPin2 A1
#define model2 1080
#define IRPin3 A2
#define model3 1080
int distance_cm1;
int distance_cm2;
int distance_cm3;


/* Model :
  GP2Y0A02YK0F --> 20150
  GP2Y0A21YK0F --> 1080
  GP2Y0A710K0F --> 100500
  GP2YA41SK0F --> 430
*/


SharpIR sensorAvant = SharpIR(IRPin3, model3);
SharpIR sensorGauche = SharpIR(IRPin2, model2);
SharpIR sensorDroite = SharpIR(IRPin1, model1);


void setup() {
  Serial.begin(9600);
}
void loop() {

  distance_cm3 = sensorAvant.distance();
  distance_cm2 = sensorGauche.distance();
  distance_cm1 = sensorDroite.distance();

  
  Serial.print("Gauche :");
  Serial.print(distance_cm2);
  Serial.print(" cm |");
  Serial.print("Avant :");
  Serial.print(distance_cm3);
  Serial.print(" cm |");
  Serial.print("Droite :");
  Serial.print(distance_cm1);
  Serial.println(" cm");
  delay(100);
}
