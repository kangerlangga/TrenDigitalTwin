#include <DHT.h>

#define DHTPIN 2       // OUT dari DHT11 ke pin digital 12
#define DHTTYPE DHT11     // Jenis sensor: DHT11

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  dht.begin();
  Serial.println("📡 Cek Sensor DHT11 (modul 3 pin)...");
}

void loop() {
  float suhu = dht.readTemperature();
  float kelembapan = dht.readHumidity();

  if (isnan(suhu) || isnan(kelembapan)) {
    Serial.println("⚠️  Gagal membaca dari sensor DHT!");
  } else {
    Serial.print("🌡️  Suhu: ");
    Serial.print(suhu, 1);
    Serial.print(" °C  |  💧 Kelembapan: ");
    Serial.print(kelembapan, 1);
    Serial.println(" %");
  }

  delay(2000); // Refresh setiap 2 detik
}
