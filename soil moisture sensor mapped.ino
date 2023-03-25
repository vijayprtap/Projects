/*AOUT--------->A0
  VCC----------> 5V
  Gnd---------->GND
*/
#include <Wire.h> // Library for I2C communication
#include <LiquidCrystal_I2C.h> // Library for LCD
LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 16, 2); // Change to (0x27,20,4) for 20x4 LCD.

#include "DHT.h"
#define DHTPIN 16  //D0  
#define DHTTYPE DHT11 
DHT dht(DHTPIN, DHTTYPE);

#define buzzer  14//D5
#define motorpin 12//D6
int valinair=885;// 500 in ardiuno 680 in esp8266
int valinwater=420;// 190 in ardiuno 290 in esp8266
int val;

void setup() {
Serial.begin(9600);
 lcd.init();
  lcd.backlight();
 dht.begin();
                                                                      
pinMode(A0,INPUT);
pinMode(buzzer,OUTPUT);
pinMode(motorpin,OUTPUT);
}

void loop() {
float h = dht.readHumidity();
  float t = dht.readTemperature();
  Serial.print(" Humidity: ");
  Serial.print(h); 
  Serial.print((" Temperature: "));
  Serial.print(t);  
    Serial.println(" ");
lcd.setCursor(0, 0); // Set the cursor on the third column and first row.
  lcd.print("TEMP -> "+ String(t)+("C")); // Print the string "Hello World!"
  lcd.setCursor(0, 1); //Set the cursor on the third column and the second row (counting starts at 0!).
  lcd.print("HUMI -> "+ String(h));

  

val = analogRead(A0);
//Serial.print("     Sensor Value:");
//Serial.println(val);
 int moisturepercentage= map(val,valinair,valinwater,0,100);
//lcd.clear();
//lcd.print("Moisture:"+String(moisturepercentage)+("%"));


Serial.print(" Percentage:");
Serial.println(moisturepercentage);
if (moisturepercentage < 40) {
    digitalWrite(motorpin, HIGH);
    digitalWrite(buzzer, HIGH);
Serial.println("loop 1");
    
    
  }
   if (moisturepercentage > 40 && moisturepercentage < 50) {
    digitalWrite(motorpin, HIGH);
    digitalWrite(buzzer, HIGH);
    Serial.println("loop 2");

  }
 if (moisturepercentage >= 55) {
    digitalWrite(motorpin,LOW);
    digitalWrite(buzzer, LOW);
    Serial.println("loop 3");
  }
delay(1000);
}