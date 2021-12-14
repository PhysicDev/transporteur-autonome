//un code très rudimentaire pour verifier que les roues fonctionnent
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


void setup() {
  Serial.begin(9600);
  //setup des pins
  pinMode(enableL,OUTPUT);
  pinMode(enableR,OUTPUT);
  pinMode(IN1,OUTPUT);
  pinMode(IN2,OUTPUT);
  pinMode(IN3,OUTPUT);
  pinMode(IN4,OUTPUT);

  //on allume les moteurs et on met toutes les inputs à 0V tout par sécurité
  analogWrite(enableL,255);
  analogWrite(enableR,255);
  digitalWrite(IN1,0);
  digitalWrite(IN2,0);
  digitalWrite(IN3,0);
  digitalWrite(IN4,0);
}

void loop() {
  //marche avant pendant 0.5s
  digitalWrite(IN1,0);
  digitalWrite(IN2,1);
  digitalWrite(IN3,1);
  digitalWrite(IN4,0);
  delay(500);
  //marche arriere pendant 0.5s
  digitalWrite(IN1,1);
  digitalWrite(IN2,0);
  digitalWrite(IN3,0);
  digitalWrite(IN4,1);
  delay(500);
}
