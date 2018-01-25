#include <LiquidCrystal.h>
//ubrat drebezg
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

  int  del = 100; //sensivity
  int speed = 0;
  int key = 0;
  unsigned long last_time;
  
  
void setup() {
  lcd.begin(16,2);
  
}

void loop() {
 if (millis() - last_time > del) {
  last_time = millis();
  key = analogRead(0);
  if (key < 60) {
   speed++;
   }
   else if (key < 200) {
     //up
   }
   else if (key < 400){
     //down
   }
   else if (key < 600){
      speed--;
    }
   else if (key < 800){
    
    } 
   lcd.setCursor(0,0);
   lcd.print(speed);
  }
}

