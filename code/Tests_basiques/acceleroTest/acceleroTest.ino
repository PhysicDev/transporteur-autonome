// code de test récuperé ici https://www.aranacorp.com/fr/utilisation-dun-module-mpu6050-avec-arduino/


//Libraries
#include <Wire.h>//https://www.arduino.cc/en/reference/wire
#include <Adafruit_MPU6050.h>//https://github.com/adafruit/Adafruit_MPU6050
#include <Adafruit_Sensor.h>//https://github.com/adafruit/Adafruit_Sensor
//Objects
Adafruit_MPU6050 mpu;
void setup() {
 //Init Serial USB
 Serial.begin(9600);
 Serial.println(F("Initialize System"));
if (!mpu.begin(0x69)) { // Change address if needed
   Serial.println("Failed to find MPU6050 chip");
   while (1) {
     delay(10);
   }
 }
 mpu.setAccelerometerRange(MPU6050_RANGE_16_G);
 mpu.setGyroRange(MPU6050_RANGE_250_DEG);
 mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
}
void loop() {
 readMPU();
 delay(100);
}
void readMPU( ) { /* function readMPU */
 ////Read acceleromter data
 sensors_event_t a, g, temp;
 mpu.getEvent(&a, &g, &temp);
 /* Print out the values */
 Serial.print("Acceleration X: ");
 Serial.print(a.acceleration.x);
 Serial.print(", Y: ");
 Serial.print(a.acceleration.y);
 Serial.print(", Z: ");
 Serial.print(a.acceleration.z);
 Serial.println(" m/s^2");
 Serial.print("Rotation X: ");
 Serial.print(g.gyro.x);
 Serial.print(", Y: ");
 Serial.print(g.gyro.y);
 Serial.print(", Z: ");
 Serial.print(g.gyro.z);
 Serial.println(" rad/s");
 Serial.print("Temperature: ");
 Serial.print(temp.temperature);
 Serial.println("°C");
}
