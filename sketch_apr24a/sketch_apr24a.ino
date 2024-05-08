#include <Servo.h>
#include <LiquidCrystal_I2C.h>

#define SCL_PIN 8
#define SDO_PIN 9
#define SERVO_PIN 10

LiquidCrystal_I2C lcd(0x27, 16, 2);
Servo myServo;

int melody[] = { 
659, 698, 587, 659, 698, 587, 0, 659, 698, 587, 659, 587, 659, 698, 784, 698, 659, 587, 523, 587, 523, 494, 440, 440, 440, 494, 523, 587, 523, 494, 523, 440, 392, 349, 349, 349, 392, 440, 0, 494, 0, 440, 392, 440, 349, 330, 659, 659, 784, 784, 784, 784, 698, 587, 659, 659, 659, 659, 659, 659, 784, 784, 784, 784, 698, 587, 659, 659, 659, 659, 440, 440, 494, 523, 587, 523, 494, 523, 440, 392, 349, 349, 392, 440, 494, 0, 440, 392, 440, 349, 392, 349, 330, 330, 330, 330, 440, 440, 392, 440, 494, 392, 392, 392, 440, 494, 494, 440, 392, 349, 330, 349, 392, 330, 330, 330, 330, 330, 440, 440, 392, 440, 494, 392, 392, 392, 440, 494, 494, 440, 392, 349, 330, 349, 392, 330, 659, 698, 587, 659, 698, 587, 0, 659, 698, 587, 659, 587, 659, 698, 784, 659, 659, 698, 587, 659, 0 
};
int noteDurations[] = { 
504, 189, 126, 504, 189, 126, 378, 504, 378, 126, 1008, 504, 378, 126, 504, 378, 126, 504, 168, 168, 168, 1008, 336, 168, 336, 168, 336, 168, 504, 504, 504, 504, 1008, 336, 336, 336, 336, 336, 336, 168, 84, 504, 504, 504, 504, 1008, 1008, 1008, 504, 378, 126, 1008, 1008, 1008, 504, 378, 126, 1008, 1008, 1008, 504, 378, 126, 1008, 1008, 1008, 504, 378, 126, 1008, 504, 336, 168, 336, 168, 336, 168, 504, 504, 1008, 504, 336, 336, 336, 168, 84, 504, 504, 504, 168, 168, 168, 1008, 504, 378, 126, 1008, 378, 126, 378, 126, 1008, 504, 378, 126, 378, 126, 378, 126, 378, 126, 378, 126, 1008, 378, 126, 378, 126, 1008, 378, 126, 378, 126, 1008, 504, 378, 126, 378, 126, 378, 126, 378, 126, 378, 126, 1008, 504, 189, 126, 504, 378, 126, 189, 504, 378, 126, 1008, 504, 252, 252, 504, 378, 126, 504, 504, 1008, 0 
};

byte correctCode[4] = {1, 2, 3, 4}; // 4 haneli şifre
byte fenerCode[4] = {1, 9, 0, 7};
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

 //   for (int thisNote = 0; thisNote < sizeof(melody) / sizeof(int); thisNote++)
 // {    
 //   tone(11, melody[thisNote], noteDurations[thisNote] * .7);    
 //   delay(noteDurations[thisNote]);    
//    noTone(11);
//  }
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
          lcd.print("0");
          break;
        case 11:
          lcd.print("*");
          break;
        case 12:
          lcd.print("#");
          break;
        case 13:
          lcd.print("&");
          break;
        case 14:
          lcd.print("$");
          break;
        case 15:
          lcd.print("%");
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
        } 
        else if (checkfenerCode()) {
          lcd.clear();
          lcd.setCursor(4, 0);
          lcd.print("EN BUYUK");
          lcd.setCursor(3, 1);
          lcd.print("FENERBAHCE");
          delay(500);
              for (int thisNote = 0; thisNote < sizeof(melody) / sizeof(int); thisNote++)
               {    
                tone(11, melody[thisNote], noteDurations[thisNote] * .7);    
                delay(noteDurations[thisNote]);    
                noTone(11);
               }
        }
        else {
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

bool checkfenerCode() { // Girilen şifreyi doğru şifreyle karşılaştır
  for (int i = 0; i < 4; i++) {
    if (enteredCode[i] != fenerCode[i]) {
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
