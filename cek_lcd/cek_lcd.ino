#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2); // Ganti alamat sesuai hasil scan

void setup() {
  lcd.init();       // Beberapa library pakai init() bukan begin()
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Hello World");
}

void loop() {
  // kosong
}
