//le robot avance de X mètre puis fait demi tour

//Libraries

#include <EEPROM.h>
#include <Wire.h>//https://www.arduino.cc/en/reference/wire
#include <Adafruit_MPU6050.h>//https://github.com/adafruit/Adafruit_MPU6050
#include <Adafruit_Sensor.h>//https://github.com/adafruit/Adafruit_Sensor
//Objects


//magnetor library
#include <FreeSixIMU.h>
#include <FIMU_ADXL345.h>
#include <FIMU_ITG3200.h>


#define AD0 13 // pin de l'acceléromètre pour changer son adresse

#define TESTS 100
#define TESTDELAY 20
#define MINSPEED 0.1


//vitesse à partir de laquelle on utilise le magnétomètre au lieu de l'accéléromètre
#define SPEEDLIMIT 0.7*PI

//decalage d'accélération 
//decalage du Moment
float decAcc[3]={0,0,0};
float decMom[3]={0,0,0};


sensors_event_t a, g, temp;


//ENCODEUR ET MOTEUR
#define DIST 4000

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
int RightRPS = 1000;
int LeftRPS = 1000;

//les pins des encodeurs
//les pins dirL et dirL ne sont pas branché car non utilisé
const int interR = 3;
const int dirR = 24;
const int interL = 2;
const int dirL = 28;

volatile long RwheelPulse = 0;
volatile long LwheelPulse = 0;


//magnétomètre
float SEN0Dat[3]; // yaw pitch roll
float SEN0offset=1; // decalage du lacet

// Set the FreeSixIMU object
FreeSixIMU sixDOF = FreeSixIMU();

Adafruit_MPU6050 mpu;
void setup() {
  //Init Serial USB
   Serial.begin(9600);
   Serial.println(F("Initialize System"));
   setupMPU();
   setupMotor();

   
   //init magnetometer
   Wire.begin();
   delay(5);
   sixDOF.init(); //begin the IMU
   delay(100);
 
}

void setupMotor(){
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

void setupMPU(){
    pinMode(AD0,OUTPUT);
    digitalWrite(AD0,HIGH);
    if (!mpu.begin(0x69,&Wire,1)) {
     //si ça marche pas
     Serial.println("Failed to find MPU6050 chip");
   }
   mpu.setAccelerometerRange(MPU6050_RANGE_16_G);
   mpu.setGyroRange(MPU6050_RANGE_250_DEG);
   mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  
   delay(200);
   //calibrage : 
   calibrageMPU();
}

int deltaT;
long lastLoop;

float veloc[3]={0,0,0};
float pos[3]={0,0,0};
float angle[3]={0,0,0};

long frame=0;
long distance=0;

bool state=true;

float RightRot=1;
float LeftRot=1;
int Rmotor=255;
int Lmotor=255;
float startAngle=0;
void loop() {

  //accelerometer
 frame++;
 deltaT=(int)millis()-lastLoop;
 lastLoop=millis();
 readMPU();
 readSEN0();
 updatePos(deltaT);
 updateMotor(deltaT);
 if(distance>DIST && state){
    digitalWrite(IN1,0);
    digitalWrite(IN2,1);
    digitalWrite(IN3,0);
    digitalWrite(IN4,1);
    RightRPS = 500;
    LeftRPS=500;
    distance=0;
    startAngle=angle[2];
    //angle[2]=0;
    state=false;
 }
 if(abs(angle[2]-startAngle)>PI/2 && !state){
    digitalWrite(IN1,1);
    digitalWrite(IN2,0);
    digitalWrite(IN3,0);
    digitalWrite(IN4,1);
    RightRPS = 1000;
    LeftRPS=1000;
    distance=0;
    state=true;
 }

 if(frame%20==0){
    Serial.print(lastLoop);
    Serial.print(" | ");
    Serial.println(angle[2]*180/PI);
    
 }
}




void updateMotor(float deltaT){
  RightRot=1000*float(RwheelPulse)/deltaT;
  LeftRot=1000*float(LwheelPulse)/deltaT;
  distance+=(RwheelPulse+LwheelPulse)/2;
  LwheelPulse=0;
  RwheelPulse=0;
  if(RightRot>RightRPS){
    Rmotor-=2;
  }else{
    Rmotor+=2;
  }
  if(LeftRot>LeftRPS){
    Lmotor-=2;
  }else{
    Lmotor+=2;
  }
  Rmotor= constrain(Rmotor,0,255);
  Lmotor= constrain(Lmotor,0,255);
  
  analogWrite(enableR,Rmotor);
  analogWrite(enableL,Lmotor);
}


void updatePos(float deltaT){
  if(abs(g.gyro.z-decMom[2])>SPEEDLIMIT){
    angle[2]=(SEN0Dat[0]-SEN0offset)*PI/180;
  }else{
    angle[2]+=1.0/1000.0*(g.gyro.z-decMom[2])*(float)deltaT;
    SEN0offset=SEN0Dat[0]-(angle[2])*180/PI;
  }
}

void calibrageMPU(){
  //calibrage : 
 Serial.println("calibrage de l'acceleromètre ...");
 for(int i=0;i<TESTS;i++){
  if(i*100/TESTS%10==0){
    Serial.print("progress : ");
    Serial.print((i*10/TESTS)*10);
    Serial.println(" %");
  }
   //get value
   mpu.getEvent(&a, &g, &temp);
   decAcc[0]=(a.acceleration.x+decAcc[0]*i)/(i+1);
   decAcc[1]=(a.acceleration.y+decAcc[1]*i)/(i+1);
   decAcc[2]=(a.acceleration.z+decAcc[2]*i)/(i+1);
   decMom[0]=(g.gyro.x+decMom[0]*i)/(i+1);
   decMom[1]=(g.gyro.y+decMom[1]*i)/(i+1);
   decMom[2]=(g.gyro.z+decMom[2]*i)/(i+1);
   delay(TESTDELAY);
 }
 Serial.println("calibrage terminé");
 for(int i=0;i<3;i++){
  EEPROM.write((int)(decAcc[i]*1000),i*4);
  EEPROM.write((int)(decMom[i]*1000),12+i*4);
 }
}


void readSEN0(){
  sixDOF.getEuler(SEN0Dat);

  //rend les données plus precises pour une raison inconue
  SEN0Dat[0]+=1;
  SEN0Dat[0]*=-1;
  SEN0Dat[1]+=1;
  SEN0Dat[2]+=1;
  
}

void printSEN0(){
  Serial.print(SEN0Dat[0]);
  Serial.print(" | ");
  Serial.print(SEN0Dat[1]);
  Serial.print(" | ");
  Serial.print(SEN0Dat[2]);

  delay(100);
}


void readMPU( ) { /* function readMPU */
 mpu.getEvent(&a, &g, &temp);
}

void printMPU(){
   /* Print out the values */
   Serial.print("Acceleration X: ");
   Serial.print(a.acceleration.x-decAcc[0]);
   Serial.print(", Y: ");
   Serial.print(a.acceleration.y-decAcc[1]);
   Serial.print(", Z: ");
   Serial.print(a.acceleration.z-decAcc[2]);
   Serial.print(" m/s^2 || ");
   Serial.print("Rotation X: ");
   Serial.print(g.gyro.x-decMom[0]);
   Serial.print(", Y: ");
   Serial.print(g.gyro.y-decMom[1]);
   Serial.print(", Z: ");
   Serial.print(g.gyro.z-decMom[2]);
   Serial.println(" rad/s");
}



void Rpulse() {
  RwheelPulse++;
}
void Lpulse() {
  LwheelPulse++;
}
