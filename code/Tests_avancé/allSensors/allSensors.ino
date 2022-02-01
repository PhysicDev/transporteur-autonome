//il y a peu être des librairies inutiles 
// penser à les suprimmer
#include <FreeSixIMU.h>
#include <Wire.h>
#include <FIMU_ADXL345.h>
#include <FIMU_ITG3200.h>

#include <Adafruit_MPU6050.h>//https://github.com/adafruit/Adafruit_MPU6050
#include <Adafruit_Sensor.h>//https://github.com/adafruit/Adafruit_Sensor

//pour corriger les defaut de l'accéléromètre
#define CorectX 0.32;
#define CorectY -0.1;
#define AxeZ -0.02;

// magnetometer data
float angles[3]; // yaw pitch roll
float angleInit;
int data[6];
// Set the FreeSixIMU object
FreeSixIMU sixDOF = FreeSixIMU();

//accelerometer data
#define AD0 13 // pin de l'acceléromètre pour changer son adresse
Adafruit_MPU6050 mpu;

//MOTOR + ENCODER
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
const int interR = 3;
const int dirR = 24;
const int interL = 2;
const int dirL = 28;

volatile long RwheelPulse = 0;
volatile long LwheelPulse = 0;

void setup() {
  Serial.begin(115200);
  Wire.begin();

  //accelerometer
  pinMode(AD0,OUTPUT);
  digitalWrite(AD0,HIGH);
  if (!mpu.begin(0x69,&Wire,1)) {
    //si ça marche pas
    Serial.println("Failed to find MPU6050 chip");
  }
  mpu.setAccelerometerRange(MPU6050_RANGE_16_G);
  mpu.setGyroRange(MPU6050_RANGE_250_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

  //magnetometer
  delay(5);
  sixDOF.init(); //begin the IMU
  delay(5);

  //encoder+ Moteur
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
int iter=0;
void loop() {
  iter++;
  //magnetometer
  readMagn();
  

  //accelerometer
  readMPU();
  delay(100);

  if(iter%10==0){
    printMagn();
    printMPU();
    Serial.println("");
  }
}


// pour ne pas surcharger l'affichage, je n'affiche que les données les plus utiles
void readMagn(){
  sixDOF.getYawPitchRoll(angles);
  sixDOF.getRawValues(data);
  angles[0]+=100;
}
void printMagn(){
  Serial.print("Magnetometer : ");
    Serial.print(angles[0]-100);//rotation axe Z
    
  /**Serial.print(" ");
    Serial.print(millis());
  Serial.print(" | ");
  Serial.print(angles[1]);
  Serial.print(" | ");
  Serial.print(angles[2]);*/
}

void readMPU( ) { 
 sensors_event_t a, g, temp;
 mpu.getEvent(&a, &g, &temp);
 /* Print out the values */

 a.acceleration.x-=CorectX;
 a.acceleration.y-=CorectY;
 g.gyro.z-=AxeZ;
}

void printMPU(){
 sensors_event_t a, g, temp;
 mpu.getEvent(&a, &g, &temp);
 a.acceleration.x-=CorectX;
 a.acceleration.y-=CorectY;
 g.gyro.z-=AxeZ;
 Serial.print("     || Accelerometer : ");
 Serial.print(" X: ");
 Serial.print(a.acceleration.x);
 Serial.print(", Y: ");
 Serial.print(a.acceleration.y);
 Serial.print(" || ");
 /**
 Serial.print(", Z: ");
 Serial.print(a.acceleration.z);
 Serial.print(" m/s^2 || ");
 Serial.print(" X: ");
 Serial.print(g.gyro.x);
 Serial.print(", Y: ");
 Serial.print(g.gyro.y);**/
 Serial.print(", Z: ");
 Serial.print(g.gyro.z);
 Serial.print(" rad/s");
}


void Rpulse() {
  RwheelPulse++;
}
void Lpulse() {
  LwheelPulse++;
}
