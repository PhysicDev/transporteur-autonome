//use driver ch340

//programme qui fait avancer le robot en ligne droite (pour tester la correction avec les encodeurs

//variable des pins
const int enableL = 9;
const int enableR = 10;
//IN1 et IN2 pour la roue droite
const int IN1 = 5;
const int IN2 = 4;
//IN3 et IN4 pour la roue gauche 
const int IN3 = 6;
const int IN4 = 7;

//rotation par secondesx1000
#define RightRPS 900
#define LeftRPS 900

//les pins des encodeurs
//les pins dirL et dirL ne sont pas branché car non utilisé
const int interR = 3;
const int dirR = 24;
const int interL = 2;
const int dirL = 28;

volatile long RwheelPulse = 0;
volatile long LwheelPulse = 0;


void setup() {
  Serial.begin(9600);
  
  //setup des pins
  pinMode(enableL,OUTPUT);
  pinMode(enableR,OUTPUT);
  pinMode(IN1,OUTPUT);
  pinMode(IN2,OUTPUT);
  pinMode(IN3,OUTPUT);
  pinMode(IN4,OUTPUT);

  pinMode(interR,INPUT_PULLUP);
  pinMode(dirR,INPUT);
  pinMode(interL,INPUT_PULLUP);
  pinMode(dirL,INPUT);

  //exécute la fonction à chaque fois que le signal passe de 0 à 1
  attachInterrupt(digitalPinToInterrupt(interR), Rpulse, RISING);
  attachInterrupt(digitalPinToInterrupt(interL), Lpulse, RISING);

  //on allume les moteurs et on met toutes les inputs à 0V tout par sécurité
  analogWrite(enableL,255);
  analogWrite(enableR,255);
  digitalWrite(IN1,1);
  digitalWrite(IN2,0);
  digitalWrite(IN3,0);
  digitalWrite(IN4,1);
}

long lastFrame=0;
float deltaT=0;
float RightRot=1;
float LeftRot=1;
int Rmotor=255;
int Lmotor=255;

long Loop=15;

void loop() {
  
  deltaT=millis()-lastFrame;
  lastFrame=millis();
  
  RightRot=1000*float(RwheelPulse)/deltaT;
  LeftRot=1000*float(LwheelPulse)/deltaT;
  
  LwheelPulse=0;
  RwheelPulse=0;
  if(RightRot>RightRPS){
    Rmotor-=5;
  }else{
    Rmotor+=5;
  }
  if(LeftRot>LeftRPS){
    Lmotor-=5;
  }else{
    Lmotor+=5;
  }
  Rmotor= constrain(Rmotor,0,255);
  Lmotor= constrain(Lmotor,0,255);
  analogWrite(enableR,Rmotor);
  analogWrite(enableL,Lmotor);
  delay(5);
}

void Rpulse() {
  RwheelPulse++;
}
void Lpulse() {
  LwheelPulse++;
}
