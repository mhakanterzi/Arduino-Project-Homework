#include <Servo.h>
#include <LiquidCrystal_I2C.h>

#define SCL_PIN 8
#define SDO_PIN 9
#define SERVO_PIN 10

LiquidCrystal_I2C lcd(0x27, 16, 2);
Servo myServo;

byte correctCode[4] = {1, 2, 3, 4}; // 4 haneli şifre
byte enteredCode[4]; // Girilen şifre
int codeIndex = 0; // Şifredeki şu anki pozisyon

void setup() {
  Serial.begin(9600);
  pinMode(SCL_PIN, OUTPUT);
  pinMode(SDO_PIN, INPUT);

  lcd.begin();
  lcd.clear();
  lcd.print("Sifreyi Girin");

  myServo.attach(SERVO_PIN); // Servo'yu 10 numaralı pine bağla
}

void loop() {
  byte button = readbutton();

  if (button) {
    Serial.println(button);

    if (button == 16) { // 16 "geri silme" anlamına gelir
      if (codeIndex > 0) {
        codeIndex--; // Şifreyi bir adım geri al
        lcd.setCursor(codeIndex, 1);
        lcd.print(" "); // Ekrandaki karakteri sil
      }
    } else if (button >= 10 && button <= 15) { // 11-15 semboller
      lcd.setCursor(codeIndex, 1);

      switch (button) {
        case 10:
          lcd.print("*");
          break;
        case 11:
          lcd.print("#");
          break;
        case 12:
          lcd.print("&");
          break;
        case 13:
          lcd.print("$");
          break;
        case 14:
          lcd.print("%");
          break;
        case 15:
          lcd.print("@");
          break;
      }

      enteredCode[codeIndex] = 0; // Sembol olduğunda şifreye eklenmez
      codeIndex++; // Sonraki pozisyona geç
    } else {
      enteredCode[codeIndex] = button; // Basılan sayıyı şifreye ekle
      codeIndex++; // Şifredeki sonraki pozisyona geç

      // LCD ekranda girilen tuşları göster
      lcd.setCursor(codeIndex - 1, 1);
      lcd.print(button);

      if (codeIndex == 4) { // 4 haneli şifre dolduğunda
      delay(500);
        if (checkCode()) { // Doğru şifre kontrolü
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Dogru Sifre!");
          
          myServo.write(90); // Servo'yu 90 dereceye getir
          delay(3000); // 2 saniye bekle
          myServo.write(0); // Servo'yu geri getir
        } else {
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Yanlis Sifre!");
          delay(2000);
        }

        // Şifreyi sıfırla
        codeIndex = 0;
        lcd.setCursor(0, 1);
        lcd.print("                "); // Alt satırı temizle
        lcd.setCursor(0, 0);
        lcd.print("Sifreyi Girin");
      }
    }

    delay(500); // Hatalı algılamayı önlemek için kısa gecikme
  }

  delay(100); // Döngü gecikmesi
}

bool checkCode() { // Girilen şifreyi doğru şifreyle karşılaştır
  for (int i = 0; i < 4; i++) {
    if (enteredCode[i] != correctCode[i]) {
      return false; // Yanlış şifre
    }
  }
  return true; // Doğru şifre
}

byte readbutton() {
  byte number;
  byte buttondurum = 0;

  for (number = 1; number <= 16; number++) {
    digitalWrite(SCL_PIN, LOW);

    if (!digitalRead(SDO_PIN)) {
      buttondurum = number; // Basılan tuş numarası
    }

    digitalWrite(SCL_PIN, HIGH);
  }

  return buttondurum;
}
