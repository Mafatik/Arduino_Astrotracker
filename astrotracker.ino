#include <LiquidCrystal.h>
//bag s neravnomernum izmeneniem skorosti (vinovat ekran, v serial vse pravilno)
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

  bool key_flag_up = 0;
  bool key_flag_down = 0;
  int bright = 30;
  int  del = 100; //sensivity
  int set_speed = 225; //speed
  unsigned int speed = 0;
  int key = 0;
  int mode = 0;
  unsigned long last_time;
  
  
void setup() {
  lcd.begin(16,2);
  pinMode(0, INPUT_PULLUP);
  }

void loop() {
  analogWrite (10, bright);
  key = analogRead(0);
  lcd.setCursor(0,0);
  lcd.print("Mode:");
  lcd.setCursor(0,1);
  lcd.print("Speed:");

  if (key == 99 && key_flag_up == 0) {
  mode++;
  key_flag_up = 1;
  lcd.clear();
  }
  if (key == 255 && key_flag_down == 0) {
  bright = bright +20;
  key_flag_down = 1;
  }
  else if (key == 1023) {
    key_flag_up = 0;
    key_flag_down = 0;
  }

  if (mode == 3) {
    mode = 0;
  }
   
  if (mode == 0) {
   manual();  
  }
  else if (mode == 1) {
   full();  
  }
  else if (mode == 2) {
    half();
  }
   lcd.setCursor(7,1);
   lcd.print(speed);    
}

void manual() {
  lcd.setCursor(6,0);
  lcd.print("manual");
 if (key == 0 && millis() - last_time > del) {
   last_time = millis();
   speed = speed + 10;
   lcd.clear();
   }
 else if (key == 407 && millis() - last_time > del){
  last_time = millis();
  speed = speed - 10;
  lcd.clear();
    }    
 }

void full() {
  lcd.setCursor(6,0);
  lcd.print("full");  
  speed = set_speed;
}

void half () {
  lcd.setCursor(6,0);
  lcd.print("half");
  speed = set_speed/2;
}



