#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

// Inicializace servo motoru
Servo Myservo;

// Definice pinů pro ultrazvukový senzor
#define trigPin 10
#define echoPin 9

// Inicializace LCD displeje (I2C adresa 0x27, 20 znaků na 4 řádky)
LiquidCrystal_I2C lcd(0x27, 20, 4);

// Proměnné pro servo pozici, dobu odezvy a vzdálenost
int pos;
long odezva, vzdalenost;

// Konstanta pro refresh rate displeje
const long interval = 500;

// Stavová proměnná pro dávkování
bool dispensing = false;

void setup() {
  // Inicializace sériové komunikace pro debugování
  Serial.begin(9600);

  // Připojení servo motoru k pinu 3
  Myservo.attach(3);

  // Inicializace LCD displeje
  lcd.init();
  lcd.backlight();
  lcd.setCursor(3, 0);
  lcd.print("Ocistete se");

  // Nastavení pinů pro ultrazvukový senzor
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop() {
  // Spuštění trig pinu pro vyslání ultrazvukového pulsu
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);   
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Měření doby trvání echa
  odezva = pulseIn(echoPin, HIGH);

  // Výpočet vzdálenosti v centimetrech
  vzdalenost = odezva / 58.2;

  // Kontrola, zda je vzdálenost menší než 15 cm a větší než 0
  if (vzdalenost < 15 && vzdalenost > 0) {
    if (!dispensing) {
      // Pokud není právě dávkováno, zobrazí zprávu na LCD
      lcd.clear();
      lcd.setCursor(4, 0);
      lcd.print("Davkuji!");
      dispensing = true;
    }

    // Pohyb servo motoru na pozici 40 stupňů a zpět
    for(pos = 0; pos <= 40; pos++) {
      Myservo.write(pos);
      delay(15);
    }
    for(pos = 40; pos >= 0; pos--) {
      Myservo.write(pos);
      delay(15);                                                 
    }
    delay(500); // Krátká pauza po dávkování
  } else {
    if (dispensing) {
      // Pokud je ruka mimo dosah, zobrazí výchozí zprávu na LCD
      lcd.clear();
      lcd.setCursor(3, 0);
      lcd.print("Ocistete se");
      dispensing = false;
    }
  }
  delay(10); // Krátké zpoždění před dalším měřením
}