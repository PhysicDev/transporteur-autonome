//un code très rudimentaire pour verifier que les roues et les encodeurs fonctionnent
//programme qui active les roues alternativement en marche avant et marche arriere

//variable des pins
const int enableL = 9;
const int enableR = 10;
//IN1 et IN2 pour la roue droite
const int IN1 = 5;
const int IN2 = 4;
//IN3 et IN4 pour la roue gauche 
const int IN3 = 6;
const int IN4 = 7;

//les pins des encodeurs
const int interR = 3;//pin déplacé pour laisser place au lidar
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
  digitalWrite(IN1,0);
  digitalWrite(IN2,0);
  digitalWrite(IN3,0);
  digitalWrite(IN4,0);
}

void loop() {
  //affiche les données des encodeurs
  Serial.print("Right :");Serial.print(RwheelPulse);Serial.print("; D:");Serial.print(RwheelPulse/1000.0*23.56);
  Serial.print("cm || left :");Serial.print(LwheelPulse);Serial.print("; D:");Serial.print(LwheelPulse/1000.0*23.56);Serial.println("cm");
  
  //condition pour ne pas utiliser de delais de 0,5 secondes
  if(millis()%1000<500){
    //marche avant pendant 0.5s
    digitalWrite(IN1,0);
    digitalWrite(IN2,1);
    digitalWrite(IN3,1);
    digitalWrite(IN4,0);
  }else{
    //marche arriere pendant 0.5s
    digitalWrite(IN1,1);
    digitalWrite(IN2,0);
    digitalWrite(IN3,0);
    digitalWrite(IN4,1);
  }
  delay(10);
}

void Rpulse() {
  RwheelPulse++;
}
void Lpulse() {
  LwheelPulse++;
}
