#include <Wire.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>

/* ---------- I2C ---------- */
#define SLAVE_ADRES 0x08

struct SensorData {
  int  ldr;
  int  sicaklik;
  long mesafe;
  int  nem;
  int  gaz;
  byte hareket;
};

SensorData data;

unsigned long lastRead = 0;
const unsigned long readInterval = 1000;

/* ---------- TFT ---------- */
#define TFT_CS   10
#define TFT_DC   9
#define TFT_RST  8

Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

/* ---------- YARDIMCI ---------- */
void drawLabel(int x, int y, const char* label) {
  tft.setCursor(x, y);
  tft.setTextColor(ST77XX_CYAN);
  tft.print(label);
}

void drawValue(int x, int y, String value) {
  tft.fillRect(x, y, 160, 22, ST77XX_BLACK);
  tft.setCursor(x, y);
  tft.setTextSize(2);
  tft.setTextColor(ST77XX_WHITE);
  tft.print(value);
}

/* ---------- DASHBOARD ---------- */
void drawDashboard() {
  tft.fillScreen(ST77XX_BLACK);

  tft.setTextSize(3);
  tft.setCursor(30, 10);
  tft.setTextColor(ST77XX_GREEN);
  tft.println("SENSOR PANEL");

  tft.setTextSize(2);
  drawLabel(10, 50,  "TEMP:");
  drawLabel(10, 75,  "HUM :");
  drawLabel(10, 100, "GAS :");
  drawLabel(10, 125, "LDR :");
  drawLabel(10, 150, "PIR :");
  drawLabel(10, 175, "DIST:");
}

/* ---------- TFT GUNCELLE ---------- */
void updateDashboardValues() {
  drawValue(90, 50,  String(data.sicaklik) + " C");
  drawValue(90, 75,  String(data.nem) + " %");
  drawValue(90, 100, String(data.gaz));
  drawValue(90, 125, String(data.ldr));
  drawValue(90, 150, data.hareket ? "ON" : "OFF");

  if (data.mesafe < 0)
    drawValue(90, 175, "ERR");
  else
    drawValue(90, 175, String(data.mesafe) + " cm");
}

/* ---------- I2C OKUMA ---------- */
void veriOku() {
  Wire.requestFrom(SLAVE_ADRES, sizeof(SensorData));

  byte* ptr = (byte*)&data;
  byte i = 0;

  while (Wire.available() && i < sizeof(SensorData)) {
    ptr[i++] = Wire.read();
  }
}

/* ---------- SETUP ---------- */
void setup() {
  Serial.begin(9600);
  Wire.begin();              // MASTER

  tft.init(240, 320);
  tft.setRotation(1);
  drawDashboard();
}

/* ---------- LOOP ---------- */
void loop() {
  if (millis() - lastRead >= readInterval) {
    lastRead = millis();
    veriOku();
    updateDashboardValues();
  }
}
