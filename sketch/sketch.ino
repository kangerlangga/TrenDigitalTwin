#include <DHT.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// === PIN SETUP ===
const int trigPin = 9;
const int echoPin = 8;
const int lightSensorPin = 7;
const int ledPin = 3;
const int relayPin = 4;
#define DHTPIN 2
#define DHTTYPE DHT22

// === SENSOR & MODULES ===
DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 16, 2);

// === VARIABLES ===
long duration;
int distance;
const int distanceThreshold = 10;

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(lightSensorPin, INPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(relayPin, OUTPUT);

  dht.begin();
  lcd.init();
  lcd.backlight();

  Serial.begin(9600);
}

void loop() {
  // === Sensor Ultrasonik ===
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH, 30000);
  if (duration > 100 && duration < 30000) {
    distance = duration * 0.034 / 2;
  } else {
    distance = -1;
  }

  // === Sensor LDR (digital) ===
  int lightState = digitalRead(lightSensorPin);
  bool gelap = (lightState == HIGH);

  // === Sensor DHT22 ===
  float suhu = dht.readTemperature();
  float kelembapan = dht.readHumidity();
  bool dhtValid = !isnan(suhu) && !isnan(kelembapan);

  // === Aksi Berdasarkan Sensor ===
  // LED hanya menyala jika gelap
  digitalWrite(ledPin, gelap ? HIGH : LOW);

  // Relay hanya menyala jika ada objek dekat
  if (distance > 0 && distance <= distanceThreshold) {
    digitalWrite(relayPin, HIGH);
  } else {
    digitalWrite(relayPin, LOW);
  }

  // === LCD Display ===
  lcd.clear();
  lcd.setCursor(0, 0);
  if (dhtValid) {
    lcd.print("T:");
    lcd.print(suhu, 1);
    lcd.print("C H:");
    lcd.print(kelembapan, 0);
    lcd.print("%");
  } else {
    lcd.print("DHT Error");
  }

  lcd.setCursor(0, 1);
  lcd.print("L:");
  lcd.print(gelap ? "Gelap" : "Terang");
  lcd.print(" D:");
  if (distance > 0) lcd.print(distance);
  else lcd.print("X");

  // === Serial to Unity (pakai Serial.write/print) ===
  if (dhtValid) {
    Serial.print("<");
    Serial.print("T:"); Serial.print(suhu, 1);
    Serial.print(",H:"); Serial.print(kelembapan, 0);
    Serial.print(",J:"); Serial.print(distance);
    Serial.print(",G:"); Serial.print(gelap ? "1" : "0");
    Serial.println(">");
  } else {
    Serial.println("<DHT:ERROR>");
  }
  delay(2000); // Refresh interval
}
