#include <Wire.h>
#include <DHT.h>

/* ---------- I2C ---------- */
#define SLAVE_ADRES 0x08

/* ---------- PIN TANIMI — Harita v2 ---------- */
#define LDR_PIN      A2
#define MQ_AO_PIN    A0
#define MQ_DO_PIN     7
#define SES_AO_PIN   A1
#define SES_DO_PIN    6
#define HC_TRIG_PIN   4
#define HC_ECHO_PIN   5
#define PIR_PIN       2
#define DHT_PIN       3

/* ---------- DHT ---------- */
#define DHT_TIP DHT11   // DHT22 kullanıyorsan DHT22 yap
DHT dht(DHT_PIN, DHT_TIP);

/* ---------- STRUCT ---------- */
struct __attribute__((packed)) SensorData {
  int   ldr;       // 2 byte
  int   sicaklik;  // 2 byte
  long  mesafe;    // 4 byte
  int   nem;       // 2 byte
  int   gaz;       // 2 byte
  byte  hareket;   // 1 byte
  byte  ses;       // 1 byte
};                 // Toplam: 14 byte, padding yok

SensorData data;      // loop buraya yazar
SensorData dataReady; // veriGonder buradan okur (double buffer)

/* ---------- ZAMANLAMA ---------- */
unsigned long lastSensor   = 0;
unsigned long lastDHT      = 0;
const unsigned long sensorInterval = 100;  // analog/digital: 100ms
const unsigned long dhtInterval    = 2000; // DHT min 2 saniye

/* ---------- SETUP ---------- */
void setup() {
  pinMode(HC_TRIG_PIN, OUTPUT);
  pinMode(HC_ECHO_PIN, INPUT);
  pinMode(PIR_PIN,     INPUT);
  pinMode(MQ_DO_PIN,   INPUT);
  pinMode(SES_DO_PIN,  INPUT);

  dht.begin();

  Wire.begin(SLAVE_ADRES);
  Wire.onRequest(veriGonder);

  Serial.begin(9600);
  Serial.println(F("PEL-Kit NANO baslatildi."));
  Serial.print(F("SensorData boyutu: "));
  Serial.println(sizeof(SensorData)); // 14 olmali
}

/* ---------- LOOP ---------- */
void loop() {
  unsigned long simdi = millis();

  // DHT — 2 saniyede bir (minimum okuma süresi)
  if (simdi - lastDHT >= dhtInterval) {
    lastDHT = simdi;

    float t = dht.readTemperature();
    float h = dht.readHumidity();

    // NaN kontrolü — sensör okuyamazsa ERR gönder
    data.sicaklik = isnan(t) ? -999 : (int)t;
    data.nem      = isnan(h) ? -999 : (int)h;
  }

  // Diğer sensörler — 100ms'de bir
  if (simdi - lastSensor >= sensorInterval) {
    lastSensor = simdi;

    data.ldr     = analogRead(LDR_PIN);
    data.gaz     = analogRead(MQ_AO_PIN);
    data.hareket = digitalRead(PIR_PIN);
    data.ses     = digitalRead(SES_DO_PIN);

    // HC-SR04 mesafe ölçümü
    digitalWrite(HC_TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(HC_TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(HC_TRIG_PIN, LOW);

    long sure = pulseIn(HC_ECHO_PIN, HIGH, 30000);

    data.mesafe = (sure == 0) ? -1 : (sure * 0.034 / 2);

    // Double buffer — atomik kopyala
    noInterrupts();
    dataReady = data;
    interrupts();
  }
}
/* ---------- I2C CALLBACK ---------- */
// Uno istediğinde bu çalışır — dataReady'den okur
void veriGonder() {
  Wire.write((byte*)&dataReady, sizeof(dataReady));
}
