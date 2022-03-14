//=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#
//
//TRANSPORTEUR AUTONOME DEMO :
//        CE CODE ORIENTE LE ROBOT DANS LA DIRECTION AVEC LE PLUS D'ESPACE PUIS LE FAIT AVANCER JUSQU'AU PROCHAIN OBSTACLE (PUIS RECOMENCE)
//
//=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#

//bluetooth
#define BlueT Serial2

//LIDAR
#include <RPLidar.h>
RPLidar lidar;//variable lidar
#define MotorCtrl 11//contrôle du moteur
#define COM Serial1// com rx tx
float distances[360]; //données du lidar



//Libraries
#include <EEPROM.h> //pour stocker les données du calibrage
#include <Wire.h>//https://www.arduino.cc/en/reference/wire
#include <Adafruit_MPU6050.h>//https://github.com/adafruit/Adafruit_MPU6050
#include <Adafruit_Sensor.h>//https://github.com/adafruit/Adafruit_Sensor

//magnetor library
#include <FreeSixIMU.h>
#include <FIMU_ADXL345.h>
#include <FIMU_ITG3200.h>

#define AD0 13 // pin de l'acceléromètre pour changer son adresse

//info du calibrage
#define TESTS 100
#define TESTDELAY 20
#define MINSPEED 0.1

//vitesse à partir de laquelle on utilise le magnétomètre au lieu de l'accéléromètre pour suivre la rotation
#define SPEEDLIMIT 0.7*PI

//decalage d'accélération 
//decalage du Moment
float decAcc[3]={0,0,0};
float decMom[3]={0,0,0};

//variables pour l'accéléromètre
sensors_event_t a, g, temp;


//variable des pins des moteurs
const int enableL = 9;
const int enableR = 10;
//IN1 et IN2 pour la roue droite
const int IN1 = 5;
const int IN2 = 4;
//IN3 et IN4 pour la roue gauche 
const int IN3 = 6;
const int IN4 = 7;

//rotations cibles par secondesx1000
int RightRPS = 700;
int LeftRPS = 700;

//les pins des encodeurs
//les pins dirR et dirL ne sont pas branché car non utilisé
const int interR = 3;
//const int dirR = 24;
const int interL = 2;
//const int dirL = 28;

//pour compter les rotation
volatile long RwheelPulse = 0;
volatile long LwheelPulse = 0;

//magnétomètre
float SEN0Dat[3]; // yaw pitch roll
float SEN0offset=1; // decalage du lacet (est calculer à partir de l'accéléromètre)

// magnetomètre + MPU6050
FreeSixIMU sixDOF = FreeSixIMU();
Adafruit_MPU6050 mpu;

//données de position
float veloc[3]={0,0,0};
float pos[3]={0,0,0};
float angle[3]={0,0,0};

float lastDist=0;

//lidar variables
boolean OK = false;
//on sort les variables de la fonction pour économiser des ressources
float LIdistance=0;
float LIangle=0;

int farAngle=0;

//moteurs
int Rmotor=1;
int Lmotor=1;
float RightRot=1;
float LeftRot=1;

void setup() {
  //SERIAL
  Serial.begin(9600);
   Serial.println(F("Initialize System"));

   //setup des composants.
   setupMPU();
   setupMotor();
   //init magnetometer
   Wire.begin();
   delay(5);
   sixDOF.init(); //begin the IMU
   delay(100);
   
   setupBluetooth();
   setupLidar();
}



//commande bluetooth
char data;
boolean OnOff= true;

//variable de temps
int deltaT;
long lastLoop;
long frame=0;

//distance parcourue
long distance=0;

float targetAngle=0;

boolean state = false;
// false = on rechechele le plus loin
// true = on fonce jusqu'au mur

void loop() {

  //time update
  frame++;
  deltaT=(int)millis()-lastLoop;
  lastLoop=millis();

  //on met à jour le lidar
  while(!OK){
    updateLidar();
  }
  for(int i=0;i<360;i++){
    updateLidar();
  }


  //mise à jour du bluetooth
  bluetooth();

  //mise à jour des accéléromètres
  readMPU();
  readSEN0();
  updatePos(deltaT);
  
  //mise à jour des moteurs
  updateMotor(deltaT);

  //verification des conditions d'arrêt
  if(!OnOff){
     digitalWrite(IN1,0);
     digitalWrite(IN2,0);
     digitalWrite(IN3,0);
     digitalWrite(IN4,0);
     state=true;
  }else{
    //mis à jour en fonction de l'état :
    if(state){
      for(int i=-2;i<3;i++){
        if((distances[180+i]<700 && distances[180+i]>10)){
         Serial.print(distances[i]);
         Serial.print(" ");
         Serial.print(distances[259-i]);
         Serial.print(" ");
         Serial.println("contact. Retour en phase de recherche");
         int out[2];maxDistance(4,out);
          //moteur en rotation
          digitalWrite(IN1,1);
          digitalWrite(IN2,0);
          digitalWrite(IN3,1);
          digitalWrite(IN4,0);

          //angle cible
          targetAngle = (PI*(1-farAngle/180)+angle[2]);          
          while(targetAngle>2*PI){
            targetAngle-=2*PI;
          }
          while(targetAngle<-2*PI){
            targetAngle+=2*PI;
          }
          //on tourne lentement pour une meilleure précision
          RightRPS=700;
          LeftRPS=700;
          state=false;
          break;
        }
      }
    }else{
      //version multiscan : plus coûteuse en calcul
      /** on rescane la distance maximale à chaque fois pour voir si on est en face.
      int out[2];maxDistance(4,out);
      if(abs(180-out[1])<3){
         Serial.print(out[0]);
         Serial.println(" cible trouvé. Retour en phase route");
          digitalWrite(IN1,1);
          digitalWrite(IN2,0);
          digitalWrite(IN3,0);
          digitalWrite(IN4,1);
          RightRPS=1000;
          LeftRPS=1000;
          state=true;
      }
      //**/
      //version accéléromètre
      if(abs(angle[2]-targetAngle)<3){
          Serial.println(" cible trouvé. Retour en phase route");
          digitalWrite(IN1,1);
          digitalWrite(IN2,0);
          digitalWrite(IN3,0);
          digitalWrite(IN4,1);
          RightRPS=1000;
          LeftRPS=1000;
          state=true;
      }
    }
  }
  if(frame%10==0){
    frame-=10;
    Serial.print(state);
  }
}

//=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=
//
//             LIDAR
//
//=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=


void setupLidar(){
  lidar.begin(COM);
  COM.begin(115200);
  // set pin modes
  pinMode(MotorCtrl, OUTPUT);
}

void updateLidar(){
  if (IS_OK(lidar.waitPoint())) {
    //perform data processing here... 
    LIdistance = lidar.getCurrentPoint().distance;
    LIangle = lidar.getCurrentPoint().angle;
    distances[round(LIangle)%360]=LIdistance;
  } else {
    OK=false;
    analogWrite(MotorCtrl, 0); //stop the rplidar motor
    Serial.println("lidar not working");
    // try to detect RPLIDAR... 
    rplidar_response_device_info_t info;
    if (IS_OK(lidar.getDeviceInfo(info, 100))) {
       OK=true;
       Serial.println("lidar detected");
       //detected...
       lidar.startScan();
       analogWrite(MotorCtrl, 255);
       delay(100);
    }
  }
}

//pour éviter que le robot tente de se faufiller dans un espace trop petit, je considère que la distance maximale est la distance moyenne sur une plage de x degré.
void maxDistance(int rad,int out[2]){
  int maxDist=0;
  int angle=0;
  int Sum = distances[0]; 
  for(int i=0;i<rad;i++){
    Sum+=distances[359-i]+distances[i+1];
  }
  for(int i=0;i<360;i++){
      if(Sum>maxDist){
        maxDist = Sum;
        angle=i;
      }
      Sum-=distances[(360+i-rad)%360];
      Sum+=distances[(i+1+rad)%360];
    }
  farAngle=angle;
  out[0] = maxDist/(2*rad+1);out[1]=angle;
  return(out);
}

//=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=
//
//   ACCELEROMETRE ET MAGNETOMETRE
//
//=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=

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
   //on charge le calibrage : 
   loadCaliMPU();
}

int GET;

void loadCaliMPU(){
 for(int i=0;i<3;i++){
  EEPROM.get(i*4,GET);
  decAcc[i]=(float)(GET)/1000.0;
  EEPROM.get(12+i*4,GET);
  decMom[i]=(float)(GET)/1000.0;
 }
}

void updatePos(float deltaT){

  //angle = 0;
  if(abs(g.gyro.z-decMom[2])>SPEEDLIMIT){
    angle[2]=(SEN0Dat[0]-SEN0offset)*PI/180;
  }else{
    angle[2]+=1.0/1000.0*(g.gyro.z-decMom[2])*(float)deltaT;
    SEN0offset=SEN0Dat[0]-(angle[2])*180/PI;
  }

  while(angle[2]>2*PI){
    angle[2]-=2*PI;
    SEN0offset-=360;
  }
  while(angle[2]<-2*PI){
    angle[2]+=2*PI;
    SEN0offset+=360;
  }

  pos[0]=cos(angle[2])*(distance-lastDist);
  pos[1]=sin(angle[2])*(distance-lastDist);
  lastDist=distance;
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
  EEPROM.put(i*4,(int)(decAcc[i]*1000));
  EEPROM.put(12+i*4,(int)(decMom[i]*1000));
 }
}

void readSEN0(){
  sixDOF.getEuler(SEN0Dat);
  //rend les données plus precises pour une raison inconue
  SEN0Dat[0]+=1;
  SEN0Dat[1]+=1;
  SEN0Dat[2]+=1;
}

void readMPU( ) { /* function readMPU */
 mpu.getEvent(&a, &g, &temp);
}


//=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=
//
//             MOTEURS
//
//=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=

void setupMotor(){
  //setup des pins
  pinMode(enableL,OUTPUT);
  pinMode(enableR,OUTPUT);
  pinMode(IN1,OUTPUT);
  pinMode(IN2,OUTPUT);
  pinMode(IN3,OUTPUT);
  pinMode(IN4,OUTPUT);

  pinMode(interR,INPUT_PULLUP);
  //pinMode(dirR,INPUT);
  pinMode(interL,INPUT_PULLUP);
  //pinMode(dirL,INPUT);

  //exécute la fonction à chaque fois que le signal passe de 0 à 1
  attachInterrupt(digitalPinToInterrupt(interR), Rpulse, RISING);
  attachInterrupt(digitalPinToInterrupt(interL), Lpulse, RISING);

  //on allume les moteurs
  analogWrite(enableL,255);
  analogWrite(enableR,255);
  digitalWrite(IN1,1);
  digitalWrite(IN2,0);
  digitalWrite(IN3,0);
  digitalWrite(IN4,1);
}

void updateMotor(float deltaT){
  //on recupère la vitesse actuelle des moteurs
  RightRot=1000.0*float(RwheelPulse)/deltaT;
  LeftRot=1000.0*float(LwheelPulse)/deltaT;
  distance+=(RwheelPulse+LwheelPulse)/2;
  LwheelPulse=0;
  RwheelPulse=0;
  //on corrige :
  if(RightRot>RightRPS){
    Rmotor-=4;
  }else{
    Rmotor+=4;
  }
  if(LeftRot>LeftRPS){
    Lmotor-=4;
  }else{
    Lmotor+=4;
  }
  //on met à jour
  Rmotor= constrain(Rmotor,0,255);
  Lmotor= constrain(Lmotor,0,255);
  analogWrite(enableR,Rmotor);
  analogWrite(enableL,Lmotor);
}

void Rpulse() {
  RwheelPulse++;
}
void Lpulse() {
  LwheelPulse++;
}

//=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=
//
//           BLUETOOTH
//
//=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=

void setupBluetooth(){
  BlueT.begin(9600);
  Serial.println("Pret pour les commandes bluetooth");
}

void bluetooth(){
  while (BlueT.available()){
      data= char(BlueT.read());
        if(data=='C'){
             Serial.println("calibrage ...");
             digitalWrite(IN1,0);
             digitalWrite(IN2,0);
             digitalWrite(IN3,0);
             digitalWrite(IN4,0);
             delay(100);//pour que le robot aie le temps de s'arrêter
             calibrageMPU();
             if(state){
              digitalWrite(IN1,1);
              digitalWrite(IN2,0);
              digitalWrite(IN3,0);
              digitalWrite(IN4,1);
             }else{
              digitalWrite(IN1,1);
              digitalWrite(IN2,0);
              digitalWrite(IN3,1);
              digitalWrite(IN4,0);
             }
        }
        if(data=='R'){
          OnOff=true;
          Serial.print("state : ");
          Serial.println(OnOff);
          //on rallume les moteurs
            digitalWrite(IN1,1);
            digitalWrite(IN2,0);
            digitalWrite(IN3,0);
            digitalWrite(IN4,1);
            RightRPS=700;
            LeftRPS=700;
        }
        if(data=='S'){
          OnOff=false;
          Serial.print("state : ");
          Serial.println(OnOff);
        }
        
      if(data=='B'){
        //reset le robot
        //non utilisé car inutile pour les tests
      }
    }
    while (Serial.available()) {
    BlueT.write(char(Serial.read())); }
}
