#include <Wire.h>
#include <EEPROM.h>
#include <RTClib.h>
#include <LiquidCrystal_I2C.h>  // Use LiquidCrystal_I2C instead of LiquidCrystal
#include <Servo.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);  // Set the I2C address and LCD dimensions
RTC_DS1307 RTC;
int temp, inc, hours1, minut, add = 11;
int next = A0;
int INC = 2;
int set_mad = 3;
Servo myservo;

#define buzzer 4

int HOUR, MINUT, SECOND;

void setup() {
  myservo.attach(9);
  Wire.begin();
  RTC.begin();
  lcd.init();  // Initialize LCD
  lcd.backlight();  // Turn on backlight
  pinMode(INC, INPUT);
  pinMode(next, INPUT);
  pinMode(set_mad, INPUT);
  pinMode(buzzer, OUTPUT);

  lcd.setCursor(0, 0);
  lcd.print("Medicin reminder");
  lcd.setCursor(0, 1);
  lcd.print(" Using Arduino  ");
  delay(2000);
  if (!RTC.isrunning()) {
    RTC.adjust(DateTime(__DATE__, __TIME__));
  }
}

void loop() {
  int temp = 0, val = 1, temp4;
  DateTime now = RTC.now();
  if (digitalRead(set_mad) == 0)  
  {
    lcd.setCursor(0, 0);
    lcd.print("  Set Medicine  ");
    lcd.setCursor(0, 1);
    lcd.print("  Reminder time ");
    delay(2000);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Enter Time 1");
    defualt();
    time(1);
    delay(1000);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Enter Time 2");
    defualt();
    delay(1000);
    time(2);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Enter Time 3");
    defualt();
    time(3);
    lcd.setCursor(0, 0);
    lcd.print("Medicin reminder");
    lcd.setCursor(0, 1);
    lcd.print("  time has set  ");
    delay(2000);
  }
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Time:");
  lcd.setCursor(6, 0);
  lcd.print(HOUR = now.hour(), DEC);
  lcd.print(":");
  lcd.print(MINUT = now.minute(), DEC);
  lcd.print(":");
  lcd.print(SECOND = now.second(), DEC);
  lcd.setCursor(0, 1);
  lcd.print("Date: ");
  lcd.print(now.day(), DEC);
  lcd.print("/");
  lcd.print(now.month(), DEC);
  lcd.print("/");
  lcd.print(now.year(), DEC);
  match();
  delay(200);
}

void defualt() {
  lcd.setCursor(0, 1);
  lcd.print(HOUR);
  lcd.print(":");
  lcd.print(MINUT);
  lcd.print(":");
  lcd.print(SECOND);
}
void time(int x) {
  int temp = 1, minuts = 0, hours = 0, seconds = 0;
  while (temp == 1) {
    if (digitalRead(INC) == 0) {
      HOUR++;
      if (HOUR == 24) {
        HOUR = 0;
      }
      while (digitalRead(INC) == 0)
        ;
    }
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Enter Time ");
    lcd.print(x);
    lcd.setCursor(0, 1);
    lcd.print(HOUR);
    lcd.print(":");
    lcd.print(MINUT);
    lcd.print(":");
    lcd.print(SECOND);
    delay(100);
    if (digitalRead(next) == 0) {
      hours1 = HOUR;
      EEPROM.write(add++, hours1);
      temp = 2;
      while (digitalRead(next) == 0)
        ;
    }
  }
  while (temp == 2) {
    if (digitalRead(INC) == 0) {
      MINUT++;
      if (MINUT == 60) { MINUT = 0; }
      while (digitalRead(INC) == 0)
        ;
    }
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Enter Time ");
    lcd.print(x);
    lcd.setCursor(0, 1);
    lcd.print(HOUR);
    lcd.print(":");
    lcd.print(MINUT);
    lcd.print(":");
    lcd.print(SECOND);
    delay(100);
    if (digitalRead(next) == 0) {
      minut = MINUT;
      EEPROM.write(add++, minut);
      temp = 0;
      while (digitalRead(next) == 0)
        ;
    }
  }
  delay(1000);
}
void match() {
  int tem[17];
  for (int i = 11; i < 17; i++) {
    tem[i] = EEPROM.read(i);
  }
  if (HOUR == tem[11] && MINUT == tem[12]) {
    beep();
    beep();
    beep();
    beep();
    rotateServo(-90);
    lcd.setCursor(0, 0);
    lcd.print("  Take Group One  ");
    lcd.setCursor(0, 1);
    lcd.print("     Medicine     ");
    beep();
    beep();
    beep();
    beep();
    myservo.write(0);
  }

  if (HOUR == tem[13] && MINUT == tem[14]) {
    beep();
    beep();
    beep();
    beep();
    rotateServo(0);
    lcd.setCursor(0, 0);
    lcd.print("  Take Group Two  ");
    lcd.setCursor(0, 1);
    lcd.print("     Medicine     ");
    beep();
    beep();
    beep();
    beep();
  }


  if (HOUR == tem[15] && MINUT == tem[16]) {
    beep();
    beep();
    beep();
    beep();
    rotateServo(-30);
    lcd.setCursor(0, 0);
    lcd.print("Take Group Three ");
    lcd.setCursor(0, 1);
    lcd.print("     Medicine    ");
    beep();
    beep();
    beep();
    beep();
  }
}

void beep() {
  digitalWrite(buzzer, HIGH);
  delay(500);
  digitalWrite(buzzer, LOW);
  delay(500);
}
void rotateServo(int d) {
  
  myservo.write(d);// Rotate the servo 90 degrees clockwise
}
  
