#include <Servo.h>

// Pin setup
const int trigPin = 9;
const int echoPin = 8;
const int ldrPin = A3;
const int ledPin = 3;

Servo myServo;

long duration;
int distance;
int lightValue;

const int distanceThreshold = 100;
const int lightThreshold = 500;

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(ledPin, OUTPUT);

  myServo.attach(6); // Servo pada pin 6
  myServo.write(0);  // Tutup pintu (awal)
  
  Serial.begin(9600);
}

void loop() {
  // Baca sensor ultrasonik
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;

  // Baca nilai LDR
  lightValue = analogRead(ldrPin);

  Serial.print("Jarak: ");
  Serial.print(distance);
  Serial.print(" cm, Cahaya: ");
  Serial.println(lightValue);

  if (distance > 0 && distance <= distanceThreshold) {
    // Orang terdeteksi
    myServo.write(90); // Buka pintu

    if (lightValue < lightThreshold) {
      digitalWrite(ledPin, HIGH); // Menyala hanya saat malam
    } else {
      digitalWrite(ledPin, LOW); // Tidak menyala saat terang
    }

    delay(3000); // Tahan posisi terbuka 3 detik
  } else {
    // Tidak ada orang
    myServo.write(0); // Tutup pintu
    digitalWrite(ledPin, LOW); // Matikan LED
  }

  delay(500);
}
