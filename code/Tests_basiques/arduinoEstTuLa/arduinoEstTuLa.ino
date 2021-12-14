//pour verifier que l'aduino est fonctionelle et correctement connectée

void setup() {
  Serial.begin(9600);
  Serial.println("Fin du setup");
}

void loop() {
  Serial.println("Recu 5/5");
}
