// ce code fait des meusure avec l'accélérométre lorsque le véhicule est à l'arrêt pour trouver les décalage à appliquer aux accélération pour avoit des résultats correct
// le code stocke ensuite les résultat dans la mémoire EEPROM de l'arduino puis reprends des mesures normales

//ce calibrage permetra d'éviter de donner au véhicule l'impression qu'il dérive dans une direction ce qui ferai également dériver la destination

//Libraries

#include <EEPROM.h>
#include <Wire.h>//https://www.arduino.cc/en/reference/wire
#include <Adafruit_MPU6050.h>//https://github.com/adafruit/Adafruit_MPU6050
#include <Adafruit_Sensor.h>//https://github.com/adafruit/Adafruit_Sensor
//Objects


#define AD0 13 // pin de l'acceléromètre pour changer son adresse

#define TESTS 100
#define TESTDELAY 20

//decalage d'accélération 
//decalage du Moment
float decAcc[3]={0,0,0};
float decMom[3]={0,0,0};


sensors_event_t a, g, temp;

Adafruit_MPU6050 mpu;
void setup() {
 //Init Serial USB
 Serial.begin(9600);
 Serial.println(F("Initialize System"));
 
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
void loop() {
 readMPU();
 delay(100);
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

void readMPU( ) { /* function readMPU */
 mpu.getEvent(&a, &g, &temp);
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
