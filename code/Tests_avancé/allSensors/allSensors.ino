// code de test récuperé ici https://wiki.dfrobot.com/10_DOF_Sensor__SKU_SEN0140

//il y a peu être des librairies inutiles 
// penser à les suprimmer
#include <FreeSixIMU.h>
#include <Wire.h>
#include <FIMU_ADXL345.h>
#include <FIMU_ITG3200.h>

#include <Adafruit_MPU6050.h>//https://github.com/adafruit/Adafruit_MPU6050
#include <Adafruit_Sensor.h>//https://github.com/adafruit/Adafruit_Sensor

// magnetometer data
float angles[3]; // yaw pitch roll
float angleInit;
// Set the FreeSixIMU object
FreeSixIMU sixDOF = FreeSixIMU();

//accelerometer data
#define AD0 13 // pin de l'acceléromètre pour changer son adresse
Adafruit_MPU6050 mpu;

void setup() {
  Serial.begin(9600);
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
}

void loop() {

  //magnetometer
  sixDOF.getEuler(angles);

  Serial.print("Magnetometer : ");
  Serial.print(angles[0]);
  Serial.print(" | ");
  Serial.print(angles[1]);
  Serial.print(" | ");
  Serial.print(angles[2]);
  Serial.print("     || Accelerometer : ");
  /***
  Serial.print(" | orientation : ");
  Serial.print(angles[0]-angleInit);**/

  //accelerometer
  readMPU();
  delay(100);
}


void readMPU( ) { /* function readMPU */
 sensors_event_t a, g, temp;
 mpu.getEvent(&a, &g, &temp);
 /* Print out the values */
 Serial.print(" X: ");
 Serial.print(a.acceleration.x);
 Serial.print(", Y: ");
 Serial.print(a.acceleration.y);
 Serial.print(", Z: ");
 Serial.print(a.acceleration.z);
 Serial.print(" m/s^2 || ");
 Serial.print(" X: ");
 Serial.print(g.gyro.x);
 Serial.print(", Y: ");
 Serial.print(g.gyro.y);
 Serial.print(", Z: ");
 Serial.print(g.gyro.z);
 Serial.println(" rad/s");
}
