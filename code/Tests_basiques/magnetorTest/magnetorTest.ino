// code de test récuperé ici https://wiki.dfrobot.com/10_DOF_Sensor__SKU_SEN0140


#include <FreeSixIMU.h>
#include <FIMU_ADXL345.h>
#include <FIMU_ITG3200.h>

#include <Wire.h>

float angles[3]; // yaw pitch roll
float angleInit; // lacet initial (direction vers laquelle pointe la voiture au début

// Set the FreeSixIMU object
FreeSixIMU sixDOF = FreeSixIMU();

void setup() {
  Serial.begin(9600);
  Wire.begin();

  delay(5);
  sixDOF.init(); //begin the IMU
  delay(5);
}

void loop() {

  sixDOF.getEuler(angles);

  Serial.print(angles[0]);
  Serial.print(" | ");
  Serial.print(angles[1]);
  Serial.print(" | ");
  Serial.print(angles[2]);

  Serial.print(" | orientation : ");
  Serial.println(angle[0]-angleInit);

  delay(100);
}
