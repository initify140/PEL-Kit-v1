#include <Wire.h>

#define SLAVE_ADRES 0x08

#define ldrsensor A7
#define sicaklik  A6
#define trigPin   A3
#define echoPin   A2
#define nem       A1
#define mqgaz     A0
#define hareketpin   7

struct SensorData {
  int ldr;
  int sicaklik;
  long mesafe;
  int nem;
  int gaz;
  byte hareket;
};

SensorData data;

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(hareketpin, INPUT);

  Wire.begin(SLAVE_ADRES);
  Wire.onRequest(veriGonder);

  Serial.begin(9600);
}

void loop() {
  data.ldr = analogRead(ldrsensor);
  data.nem = analogRead(nem);
  data.gaz = analogRead(mqgaz);
  data.hareket = digitalRead(hareketpin);

  int raw = analogRead(sicaklik);
  data.sicaklik = (raw * 5.0 / 1023.0) * 100;

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long sure = pulseIn(echoPin, HIGH, 30000);
  if (sure == 0) {
    data.mesafe = -1;
  } else {
    data.mesafe = sure * 0.034 / 2;
  }

  delay(500);
}
void veriGonder() {
  Wire.write((byte*)&data, sizeof(data));
}