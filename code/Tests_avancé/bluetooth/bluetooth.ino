#define BlueT Serial2

void setup() {  
  Serial.begin(9600);
  delay(500);
  setupBluetooth();
  delay(500);
}

void loop() {
  bluetooth();
  delay(500);
}

char data;
boolean OnOff= true;

void setupBluetooth(){
  BlueT.begin(9600);
  Serial.println("Pret pour les commandes bluetooth");
}

void bluetooth(){
while (BlueT.available()){
    data= char(BlueT.read());
      if(data=='C'){
           Serial.println("calibrage");
      }
      if(data=='R'){
        OnOff=true;
        Serial.print("state : ");
        Serial.println(OnOff);
      }
      if(data=='S'){
        OnOff=false;
        Serial.print("state : ");
        Serial.println(OnOff);
      }
  }
  while (Serial.available()) {
  BlueT.write(char(Serial.read())); }
}
