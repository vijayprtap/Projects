/*    SOIL MOISTURE SENSOR
  AOUT--------->A0 
  VCC----------> 5V
  Gnd---------->GND
      DHT11 CONNECTIONS
OUT-------7
      LCD
SCL-------SCL
SDA-------SDA
GND-------GND
VCC-------VCC
RELAY MODULE ---->D0
  
*/
#include <Wire.h>
#include<LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);// set the LCD address to 0x27 for a 16 chars and 2 line display

#include "DHT.h"
#define DHTPIN 7    
#define DHTTYPE DHT11 
DHT dht(DHTPIN, DHTTYPE);

int buzzer = 5;
int motorpin =6;
int valinair=800;// 500 in ardiuno 680 in esp8266
int valinwater=270;// 190 in ardiuno 290 in esp8266
int val;
void setup() {
lcd.init();
lcd.backlight();
  
Serial.begin(9600);
 dht.begin();
pinMode(A0,INPUT);
pinMode(motorpin,OUTPUT);
  

}

void loop() {

float h = dht.readHumidity();
  float t = dht.readTemperature();
 Serial.println(" Humidity:"+String(h) + "Temp:" + String(t));
 
  
  
val = analogRead(A0);
Serial.print("     Sensor Value:");
Serial.println(val);
 int moisturepercentage= map(val,valinair,valinwater,0,100);

lcd.setCursor(0,0);
lcd.print("MOISTURE: " + String(moisturepercentage) + "%");
lcd.setCursor(0,1);
lcd.print("T:"+ String(t)+"C");
lcd.setCursor(9,1);
lcd.print("H:"+ String(h));




Serial.print(" Percentage:");
Serial.println(moisturepercentage);


if (moisturepercentage < 50) {

    digitalWrite(motorpin, LOW);
    digitalWrite(buzzer, HIGH);  
  }
   if (moisturepercentage > 50 && moisturepercentage < 55) {
    digitalWrite(motorpin, LOW);
    digitalWrite(buzzer, HIGH);


  }
 if (moisturepercentage > 56) {
    digitalWrite(motorpin, HIGH);
    digitalWrite(buzzer, LOW);

  }
 delay(1000);
}