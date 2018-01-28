/*//////////////////////////////////////////////////////////
Astrotracker
V 1.0.0
Pavel Antsypovich, Jan 2017

Buttons:
left/right ---- increase/decrease tracking speed
up ---- change mod
down ---- adjust brightness
select ---- start/pause tracking

Mods:
manual -- you can adjust tracking speed by pressing left/right buttots
full -- tracking speed according to default speed
half -- tracking speed according to half default speed
//////////////////////////////////////////////////////////*/
////////////////////////SETTINGS////////////////////////////
  #define  del 100 //sensivity of right/left buttons
  #define set_spd  50.0 //default speed
  #define steps 200 //number of steps per revolution

#include <LiquidCrystal.h>
#include <Stepper.h>

byte play[8]= { 
  0b10000,
  0b11000,
  0b11100,
  0b11110,
  0b11110,
  0b11100,
  0b11000,
  0b10000
};

byte pause[8]= { 
  0b11011,
  0b11011,
  0b11011,
  0b11011,
  0b11011,
  0b11011,
  0b11011,
  0b11011
};

  bool pushed = 0;
  bool released = 0;
  bool key_flag_up = 0;
  bool key_flag_down = 0;
  bool state = 0;
  int bright = 30;
  double spd = 0;
  int key = 0;
  int mode = 0;
  unsigned long last_time_speed;
  
Stepper myStepper(steps, 0, 1, 2, 3);
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
    
void setup() {
  lcd.createChar(0, play);
  lcd.createChar(1, pause);
  lcd.begin(16,2);
  spd = set_spd;
  }

void loop() { 
  lcd.setCursor(0,0);
  lcd.print("Mode:");
  lcd.setCursor(0,1);
  lcd.print("Speed:");  
  analogWrite (10, bright);
  key = analogRead(0);
  
  if (key == 99 && key_flag_up == 0) {
  mode++;
  key_flag_up = 1;
  lcd.clear();
  }
  else if (key == 255 && key_flag_down == 0) {
  bright = bright +20;
  key_flag_down = 1;
  }
  else if (key == 1023) {
    key_flag_up = 0;
    key_flag_down = 0;
  }

  if (key == 639) {
    pushed = 1;
  }
  if (key == 1023 && pushed == 1) {
    released = 1; 
  }
  
 if (pushed == 1 && released == 1) {
  state = 1 - state;
  pushed = 0;
  released =0; 
  }
  
  
  

  if (mode == 3) {
    mode = 0;
  }
   
  if (mode == 0) {
   manual();  
  }
  else if (mode == 1) {
   half();  
  }
  else if (mode == 2) {
    full();
  }

   if (state == 1) {
   myStepper.setSpeed(spd);
   myStepper.step(steps/100);
   lcd.setCursor (15, 0);
   lcd.write((uint8_t)0);
   }
   else if (state == 0) {
   myStepper.setSpeed(0);
   lcd.setCursor (15, 0);
   lcd.write((uint8_t)1);   
   }
   
   lcd.setCursor(7,1);  
   lcd.print(spd);     
}

void manual() {
  lcd.setCursor(6,0);
  lcd.print("manual");
 if (key == 0 && millis() - last_time_speed > del) {
   last_time_speed = millis();
   spd = spd + 0.1;
   }
 else if (key == 407 && millis() - last_time_speed > del){
  last_time_speed = millis();
  spd = spd - 0.1;
    }    
 }

void full() {
  lcd.setCursor(6,0);
  lcd.print("full");  
  spd = set_spd;
}

void half () {
  lcd.setCursor(6,0);
  lcd.print("half");
  spd = set_spd/2;
}
