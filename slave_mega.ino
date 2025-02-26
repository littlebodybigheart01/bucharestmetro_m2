#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

// Lista numelor stațiilor
const char* stationNames[] = {
  "TUDOR ARGHEZI",
  "BERCENI",
  "DIMITRIE LEONIDA",
  "AP. PATRIEI",
  "PIATA SUDULUI",
  "C. BRANCOVEANU",
  "EROII REVOLUTIEI",
  "TINERETULUI",
  "PIATA UNIRII",
  "UNIVERSITATE",
  "PIATA ROMANA",
  "PIATA VICTORIEI",
  "AVIATORILOR",
  "AUREL VLAICU",
  "PIPERA"
};

// Săgeți pentru peron (stânga și dreapta)
const char directionArrows[] = {
  0, // Săgeată stânga
  1  // Săgeată dreapta
};

// Definim caracterele personalizate pentru săgeți
byte arrowLeft[8] = {
  0b00000,
  0b00100,
  0b01100,
  0b11111,
  0b01100,
  0b00100,
  0b00000,
  0b00000
};

byte arrowRight[8] = {
  0b00000,
  0b00100,
  0b00110,
  0b11111,
  0b00110,
  0b00100,
  0b00000,
  0b00000
};

void setup() {
  Serial.begin(9600);        // Pentru monitorul serial
  Serial1.begin(9600);       // Pentru comunicarea cu Uno

  // Inițializează LCD-ul
  lcd.begin(16, 2);
  lcd.backlight();
  lcd.createChar(0, arrowLeft);
  lcd.createChar(1, arrowRight);

  lcd.setCursor(0, 0);
  lcd.print("Metrorex S.A.");
  lcd.setCursor(0, 1);
  lcd.print("MAGISTRALA 2");

  Serial.println("Mega ready. LCD initialized.");
}

void loop() {
  // Verifică dacă există date disponibile de la Uno
  if (Serial1.available()) {
    int index = Serial1.read();
    Serial.print("Data received: ");
    Serial.println(index);

    // Afișează pe LCD în funcție de index
    lcd.clear();

    if (index == 0) {
      lcd.setCursor(0, 0);
      lcd.print("    Atentie!");
      lcd.setCursor(0, 1);
      lcd.print("Se inchid usile!");
    } else if (index >= 1 && index <= 15) {
      lcd.setCursor(0, 0);
      lcd.print("Urmeaza statia:");

      // Afișează numele stației pe al doilea rând
      lcd.setCursor(0, 1);
      lcd.print(stationNames[index - 1]);

      // Afișează săgeata corespunzătoare în colțul din dreapta sus
      lcd.setCursor(15, 0);
      if (index == 1 || index == 5 || index == 6 || index == 7 || index == 8 || index == 9 || index == 10 || index == 13 || index == 14) {
        lcd.write(directionArrows[0]); // Săgeată stânga
      } else {
        lcd.write(directionArrows[1]); // Săgeată dreapta
      }
    } else {
      lcd.setCursor(0, 0);
      lcd.print("Index invalid");
    }
  }
}
