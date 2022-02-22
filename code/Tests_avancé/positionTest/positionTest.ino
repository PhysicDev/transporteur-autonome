//ce code calcule la position angulaire de robot
//l'accéléromètre est efficace pour mesurer les rotations à faible vitesse angulaire
// mais pas le magnètomètre car ses données dérivent avec le temps

// cependant le magnètomètre est utilisé pour les grandes vitesses angulaire, quand l'accéléromètre ne peut plus suivre
// le reste du temps, on utilise les données de l'accéléromètre pour calculer le décalage qu'accumule le magnétomètre

//ce système permet d'avoir une idée relativment précise de l'orientation relative du robot
//une fois cette information combiné avec les encodeurs, on peut retracer le mouvement du robot si on suppose qu'il
//les roues ne subissent pas de frotement et qu'il se déplace sur une surface relativement plate.

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

#include <Wire.h>


#define AD0 13 // pin de l'acceléromètre pour changer son adresse

#define TESTS 100
#define TESTDELAY 20
#define MINSPEED 0.1

//vitesse à partir de laquelle on utilise le magnétomètre au lieur 
#define SPEEDLIMIT PI

//decalage d'accélération 
//decalage du Moment
float decAcc[3]={0,0,0};
float decMom[3]={0,0,0};

//magnetor value
float SEN0Dat[3]; // yaw pitch roll
float SEN0offset=1; // decalage du lacet
// Set the FreeSixIMU object
FreeSixIMU sixDOF = FreeSixIMU();


sensors_event_t a, g, temp;

Adafruit_MPU6050 mpu;
void setup() {
   //Init Serial USB
   Serial.begin(9600);
   Serial.println(F("Initialize System"));


   //init accelerometer
    pinMode(AD0,OUTPUT);
    digitalWrite(AD0,HIGH);
    if (!mpu.begin(0x69,&Wire,1)) {
     //si ça marche pas
     Serial.println("Failed to find MPU6050 chip");
   }
   mpu.setAccelerometerRange(MPU6050_RANGE_16_G);
   mpu.setGyroRange(MPU6050_RANGE_250_DEG);
   mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

   //init magnetometer
   Wire.begin();
   delay(5);
   sixDOF.init(); //begin the IMU
  
   delay(200);
   //calibrage : mesure le décalage moyen par rapport à 0 
   calibrageMPU();
 
}


int deltaT;
long lastLoop;

float veloc[3]={0,0,0};
float pos[3]={0,0,0};
float angle[3]={0,0,0};

long frame=0;
void loop() {
 frame++;
 deltaT=(int)millis()-lastLoop;
 lastLoop=millis();
 readMPU();
 readSEN0();
 updatePos(deltaT);
 if(frame%20==0){
  //Serial.println(1.0/1000.0*angVeloc[2]*(float)deltaT);
   Serial.print("angular : ");
   Serial.print(angle[2]*180/PI);
   Serial.print(" speedX : ");
   Serial.print(SEN0Dat[0]);
   Serial.print(" speedX : ");
   Serial.print(abs(g.gyro.z-decMom[2]));
   Serial.print(" speed Y : ");
   Serial.println(SEN0offset);
   
   //printMPU();
 }
 delay(5);
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
   //on récupère les données et on fait une moyenne pondéré
   mpu.getEvent(&a, &g, &temp);
   decAcc[0]=(a.acceleration.x+decAcc[0]*i)/(i+1);
   decAcc[1]=(a.acceleration.y+decAcc[1]*i)/(i+1);
   decAcc[2]=(a.acceleration.z+decAcc[2]*i)/(i+1);
   decMom[0]=(g.gyro.x+decMom[0]*i)/(i+1);
   decMom[1]=(g.gyro.y+decMom[1]*i)/(i+1);
   decMom[2]=(g.gyro.z+decMom[2]*i)/(i+1);
   //on attends un peu pour faire plusieur mesure
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
