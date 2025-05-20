#include <DHT.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// === PIN SETUP ===
const int trigPin = 9;
const int echoPin = 8;
const int lightSensorPin = 7; // D0 dari modul LDR
const int ledPin = 3;
const int relayPin = 4;
#define DHTPIN 2
#define DHTTYPE DHT22

// === SENSOR & MODULES ===
DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 16, 2); // alamat I2C umum

// === VARIABLES ===
long duration;
int distance;
const int distanceThreshold = 100; // cm

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(lightSensorPin, INPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(relayPin, OUTPUT);

  dht.begin();
  lcd.begin(16, 2);
  lcd.backlight();

  Serial.begin(9600);
}

void loop() {
  // === Ultrasonik ===
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;

  // === LDR ===
  int lightState = digitalRead(lightSensorPin); // HIGH = gelap
  bool gelap = (lightState == HIGH);

  // === DHT22 ===
  float suhu = dht.readTemperature();
  float kelembapan = dht.readHumidity();

  // === LCD ===
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("T:");
  lcd.print(suhu, 1);
  lcd.print("C H:");
  lcd.print(kelembapan, 0);
  lcd.print("%");

  lcd.setCursor(0, 1);
  if (distance > 0 && distance <= distanceThreshold && gelap) {
    lcd.print("Status: ON ");
    digitalWrite(ledPin, HIGH);
    digitalWrite(relayPin, HIGH);
  } else {
    lcd.print("Status: OFF");
    digitalWrite(ledPin, LOW);
    digitalWrite(relayPin, LOW);
  }

  // === Serial Debug ===
  Serial.print("Jarak: ");
  Serial.print(distance);
  Serial.print(" cm | Gelap: ");
  Serial.print(gelap ? "YA" : "TIDAK");
  Serial.print(" | T: ");
  Serial.print(suhu);
  Serial.print(" C | H: ");
  Serial.println(kelembapan);

  delay(2000); // Delay untuk refresh
}
