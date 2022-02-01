#include <RPLidar.h>

RPLidar lidar;

#define MotorCtrl 11

#define COM Serial1

//code de test retourne la distance la plus petite (et son angle corespondant)

void setup() {
  // bind the RPLIDAR driver to the arduino hardware serial
  lidar.begin(COM);
  COM.begin(115200);
  Serial.begin(9600);
  Serial.print("BRUH");
  // set pin modes
  pinMode(MotorCtrl, OUTPUT);
}

float minDistance = 100000;
float angleAtMinDist = 0;
float distances[360];

void loop() {
  //analogWrite(MotorCtrl,255);
  //**
  if (IS_OK(lidar.waitPoint())) {
       Serial.print("working : ");
    //perform data processing here... 
    float distance = lidar.getCurrentPoint().distance;
    float angle = lidar.getCurrentPoint().angle;
    Serial.print(round(angle));
    Serial.print(" || ");
    Serial.print(round(distance));
    Serial.println("");
    distances[round(angle)]=distance;
    if (lidar.getCurrentPoint().startBit) {
       minDistance = 100000;
       angleAtMinDist = 0;
    } else {
       if ( distance > 0 &&  distance < minDistance) {
          minDistance = distance;
          angleAtMinDist = angle;
       }
    }
  } else {
    analogWrite(MotorCtrl, 128); //stop the rplidar motor
    
       Serial.println("NOT working");
    // try to detect RPLIDAR... 
    rplidar_response_device_info_t info;
    if (IS_OK(lidar.getDeviceInfo(info, 100))) {
       //detected...
       Serial.print("BRUH");
       lidar.startScan();
       analogWrite(MotorCtrl, 255);
       delay(1000);
    }
  }//**/
}
