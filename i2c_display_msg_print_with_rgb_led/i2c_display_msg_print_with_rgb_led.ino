#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#define Red_led  6    
#define Green_led  5      
#define Blue_led  3      
#define DELAY  5  //delay in ms

 
LiquidCrystal_I2C lcd(0x27,16,2); // set the LCD address to 0x27 for a 16 chars and 2 line display
void setup() {
  
  lcd.begin(16,2);
  
  lcd.backlight();
  Serial.begin(9600);
  lcd.print("RAINBOW PYRAMID");
  pinMode(Red_led, OUTPUT);
  pinMode(Green_led, OUTPUT);
  pinMode(Blue_led, OUTPUT);

  
}
void loop() 
{

 //Blue to Red
  for(int i=0; i<255; i++) {
    analogWrite(Red_led, 255-i); // Red_led initially OFF
    analogWrite(Green_led, 255); // Green_led OFF
    analogWrite(Blue_led, i); // Blue_led initially ON
    delay(DELAY);
  }
  //Red to Green
  for(int i=0; i<255; i++) {
    analogWrite(Red_led, i); // Red_led initially ON
    analogWrite(Green_led, 255-i); // Green_led initially OFF
    analogWrite(Blue_led, 255); // Blue_led OFF
    delay(DELAY); // wait before next transition
  }

  //Green to Blue
  for(int i=0; i<255; i++) {
    analogWrite(Red_led, 255); // Red_led OFF
    analogWrite(Green_led, i); // Green_led initially ON
    analogWrite(Blue_led, 255-i); // Blue_led initially OFF
    delay(DELAY);
  }


 

 
}
