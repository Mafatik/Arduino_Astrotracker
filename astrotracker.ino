/*//////////////////////////////////////////////////////////
Astrotracker
V 1.1.0
Pavel Antsypovich, Aug 2018

Buttons:
  left/right ---- increase/decrease tracking speed
  up ---- change mod
  down ---- adjust brightness
  select ---- select/pause tracking

Mods:
  manual -- you can adjust tracking speed by pressing left/right buttots
  full -- tracking speed according to default speed
  half -- tracking speed according to half default speed
//////////////////////////////////////////////////////////*/
#include <LiquidCrystal.h>
#include <Stepper.h>
////////////////////////SETTINGS////////////////////////////
  #define del 100 //sensivity of right/left buttons
  #define set_spd  50.0 //default speed
  #define set_br 50 //default brightness
  #define steps 360 //number of steps per revolution
  #define stop_button 13 //stop button pin
  #define buzzer 12 //buzzer pin
////////////////////////////////////////////////////////////
byte play[8]= //play sign
{ 
  0b10000,
  0b11000,
  0b11100,
  0b11110,
  0b11110,
  0b11100,
  0b11000,
  0b10000
};
byte pause[8]= //stop sign
{ 
  0b11011,
  0b11011,
  0b11011,
  0b11011,
  0b11011,
  0b11011,
  0b11011,
  0b11011
};

//Button flags
bool select_button_flag = 0;
bool up_button_flag = 0;
bool down_button_flag = 0;
//Buzzer flags
bool beep_r = 1;
bool beep_s = 1;
//Values
int state = 0;
int brightness = 0;
double spd = 0; 
int button = 0;
int mode = 0;
unsigned long last_time;
  
Stepper myStepper(steps, 0, 1, 2, 3);
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
    
void setup() 
{
  pinMode(buzzer, OUTPUT);
  pinMode(stop_button, INPUT);
  spd = set_spd;
  brightness = set_br;
  analogWrite (10, brightness);
  lcd.createChar(0, play);
  lcd.createChar(1, pause);
  lcd.begin(16,2);
  lcd.setCursor(0,0);
  lcd.print("  Astrotracker");
  lcd.setCursor(0,1);
  lcd.print("    v 1.1.0.");
  beep_hello();
  delay(2100);
  lcd.clear();
}

void loop() 
{ 
  lcd.setCursor(0,0);
  lcd.print("Mode:");
  lcd.setCursor(0,1);
  lcd.print("Speed:");  
  button = analogRead(0);
  //Up button
  if (button == 99 && up_button_flag == 0) 
  {
    mode++;
    up_button_flag = 1;
    lcd.clear();
  }
  //Down button
  else if (button == 255 && down_button_flag == 0) 
  {
    brightness = brightness + 20;
    down_button_flag = 1;
    if (brightness == 250)
    {
      beep_screen();
    }
    if (brightness == 270)
    {
      brightness = 10;
    }
    analogWrite (10, brightness);
  }
  //All buttons released
  else if (button == 1023) 
  {
    up_button_flag = 0;
    down_button_flag = 0;
  }
  //Select button
  if (button == 639) 
  {
    select_button_flag = 1;
  }
  if (select_button_flag == 1 && button == 1023)
  {
    state++;
    select_button_flag = 0;
  }
  if (digitalRead(stop_button) == HIGH)
  {
    state = 3;
  }
/////////////////////////////////////////////////
  if (mode == 3)
  {
    mode = 0;
  }
   
  if (mode == 0) 
  {
    manual();  
  }
  else if (mode == 1) 
  {
    half();  
  }
  else if (mode == 2) 
  {
    full();
  }
////////////////////////////////////////////////
  if (state == 0) 
  { 
    myStepper.setSpeed(0);
    lcd.setCursor (15, 0);
    lcd.write((uint8_t)1);
  }
  else if (state == 1) 
  {
    beep_run();
    state++;
  }
  else if (state == 2)
  {
    myStepper.setSpeed(spd);
    myStepper.step(steps/100);
    lcd.setCursor (15, 0);
    lcd.write((uint8_t)0);
  }
  else if (state == 3)
  {
    beep_stop();
    state = 0;
  }
////////////////////////////////////////////////  
  lcd.setCursor(7,1);  
  lcd.print(spd);     
}

void manual() 
{
  lcd.setCursor(6,0);
  lcd.print("manual");
 if (button == 0 && millis() - last_time > del) 
 {
    last_time = millis();
    spd = spd + 0.1;
 }
 else if (button == 407 && millis() - last_time > del)
 {
    last_time = millis();
    spd = spd - 0.1;
 }    
}

void full() 
{
  lcd.setCursor(6,0);
  lcd.print("full");  
  spd = set_spd;
}

void half() 
{
  lcd.setCursor(6,0);
  lcd.print("half");
  spd = set_spd/2;
}

void beep_hello()
{
  tone (buzzer, 300);
  delay(100);
  tone (buzzer, 600);
  delay(100);
  tone (buzzer, 900);
  delay(100);
  tone (buzzer, 1200);
  delay(100);
  noTone (buzzer);
}

void beep_run()
{
  tone (buzzer, 500);
  delay(100);
  tone (buzzer, 1000);
  delay(100);
  noTone (buzzer);
}

void beep_stop()
{
  tone (buzzer, 1000);
  delay(100);
  tone (buzzer, 500);
  delay(100);
  noTone (buzzer);
}

void beep_screen()
{
  tone (buzzer, 500);
  delay(100);
  noTone (buzzer);
}