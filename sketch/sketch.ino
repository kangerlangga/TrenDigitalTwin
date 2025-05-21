#include <DHT.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// === PIN SETUP ===
const int trigPin = 9;
const int echoPin = 8;
const int lightSensorPin = 7; // D0 dari modul LDR (digital)
const int ledPin = 3;
const int relayPin = 4;
#define DHTPIN 2
#define DHTTYPE DHT11

// === SENSOR & MODULES ===
DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 16, 2); // alamat I2C umum

// === VARIABLES ===
long duration;
int distance;
const int distanceThreshold = 10; // cm

void setup() {
  // Setup pin mode
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(lightSensorPin, INPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(relayPin, OUTPUT);

  // Aktifkan internal pull-up untuk pin data DHT11
  pinMode(DHTPIN, INPUT_PULLUP);

  // Inisialisasi sensor & LCD
  dht.begin();
  lcd.init();
  lcd.backlight();

  Serial.begin(9600);
}

void loop() {
  // --- Sensor Ultrasonik ---
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH, 30000); // timeout 30ms (hindari blocking lama)
  if (duration == 0) {
    // Jika tidak ada echo, objek terlalu jauh atau error
    distance = -1;
  } else {
    distance = duration * 0.034 / 2;
  }

  // --- Sensor LDR (digital) ---
  int lightState = digitalRead(lightSensorPin); // HIGH = gelap
  bool gelap = (lightState == HIGH);

  // --- Sensor DHT11 ---
  float suhu = dht.readTemperature();
  float kelembapan = dht.readHumidity();

  // Cek data DHT valid atau tidak
  bool dhtValid = !isnan(suhu) && !isnan(kelembapan);

  // --- LCD ---
  lcd.clear();
  lcd.setCursor(0, 0);
  if (dhtValid) {
    lcd.print("T:");
    lcd.print(suhu, 1);
    lcd.print("C H:");
    lcd.print(kelembapan, 0);
    lcd.print("%");
  } else {
    lcd.print("Sensor DHT Error");
  }

  lcd.setCursor(0, 1);
  if (distance > 0 && distance <= distanceThreshold) {
    lcd.print("Status: ON ");
    digitalWrite(ledPin, HIGH);
    digitalWrite(relayPin, HIGH);
  } else {
    lcd.print("Status: OFF");
    digitalWrite(ledPin, LOW);
    digitalWrite(relayPin, LOW);
  }

  // --- Serial Debug ---
  Serial.print("Jarak: ");
  if (distance > 0) Serial.print(distance);
  else Serial.print("Out of Range");
  Serial.print(" cm | Gelap: ");
  Serial.print(gelap ? "YA" : "TIDAK");
  Serial.print(" | T: ");
  if (dhtValid) Serial.print(suhu);
  else Serial.print("Error");
  Serial.print(" C | H: ");
  if (dhtValid) Serial.println(kelembapan);
  else Serial.println("Error");

  delay(2000); // Delay untuk refresh data
}
