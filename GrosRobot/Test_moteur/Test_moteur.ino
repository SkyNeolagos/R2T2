
#define ena 8
#define in1 9
#define in2 10
#define enb 3
#define in3 4
#define in4 5


void setup() {
  pinMode(ena,OUTPUT);
  pinMode(enb,OUTPUT);
  pinMode(in1,OUTPUT);
  pinMode(in2,OUTPUT);
  pinMode(in3,OUTPUT);
  pinMode(in4,OUTPUT);
}

void loop() {
  digitalWrite(in1,LOW);
  digitalWrite(in2,HIGH);
  digitalWrite(in3,HIGH);
  digitalWrite(in4,LOW);

  analogWrite(ena,0);
  analogWrite(enb,64);

  delay(1000);

  analogWrite(ena,64);
  analogWrite(enb,64);

  delay(1000);

  
  
}
