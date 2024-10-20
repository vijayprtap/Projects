#include <Servo.h> //includes the servo library
#include <LiquidCrystal_I2C.h>

#include "DHT.h"
#define DHTPIN 8
   // 
#define DHTTYPE DHT11 
#define ldr1 A0 // set ldr 1 Analog input pin of East ldr as an integer
#define ldr2 A3 // set ldr 2 Analog input pin of West ldr as an integer

DHT dht(DHTPIN, DHTTYPE);

const int voltageSensor = A5;
float vOUT = 0.0;
float vIN = 0.0;
float R1 = 30000.0;
float R2 = 7500.0;
int value = 0;

Servo myservo;
int pos = 90; // initial position of the Horizontal movement controlling servo motor
int tolerance = 20; // allowable tolerance setting - so solar servo motor isn't constantly in motion

LiquidCrystal_I2C lcd(0x27,16,2);// set the LCD address to 0x27 for a 16 chars and 2 line display

void setup(){
  Serial.begin(9600);

 dht.begin();
  
lcd.init();
lcd.backlight();  

myservo.attach(11); // attaches the servo on digital pin 2 to the horizontal movement servo motor 
pinMode(ldr1, INPUT); //set East ldr pin as an input
pinMode(ldr2, INPUT); //set West ldr pin as an input
myservo.write(pos); // write the starting position of the horizontal movement servo motor 
    
delay(1000); // 1 second delay to allow the solar panel to move to its staring position before comencing solar tracking
}

void loop(){   

int val1 = analogRead(ldr1); // read the value of ldr 1
int val2 = analogRead(ldr2); // read the value of ldr 2
int LightIntensity= map(val1,0,1024,0,100);

  Serial.println("ldr 1 >"+ String (val1)+" ldr 2 >"+ String (val2));
  Serial.println();
value = analogRead(voltageSensor);
  vOUT = (value * 5.0) / 1024.0;
  vIN = vOUT / (R2/(R1+R2));
float current = 1.3/vIN;
float watt= current*vIN;

float h = dht.readHumidity();
  float t = dht.readTemperature();
  
  //Serial.print("Input = ");
  //Serial.println(vIN);
lcd.setCursor(0,0);
lcd.print("V:" + String(vIN)+ String("v"));
lcd.setCursor(0,1);
lcd.print("I:" + String(LightIntensity)+ String("%"));
lcd.setCursor(8,0);
lcd.print("T:" + String(t)+ String("C"));   
lcd.setCursor(8,1);
lcd.print("H:" + String(h)+ String(""));


if((abs(val1 - val2) <= tolerance) || (abs(val2 - val1) <= tolerance)) {
//no servo motor horizontal movement will take place if the ldr value is within the allowable tolerance
}else {
if(val1 > val2) // if ldr1 senses more light than ldr2 
{
  
pos = pos+1; // decrement the 90 degree poistion of the horizontal servo motor - this will move the panel position Eastward
myservo.write(pos); 
}
if(val1 < val2) // if ldr2 senses more light than ldr1
{

pos = pos-1; // increment the 90 degree position of the horizontal motor - this will move the panel position Westward
myservo.write(pos); 
 }
}
     
if(pos > 180) {pos = 180;} // reset the horizontal postion of the motor to 180 if it tries to move past this point
if(pos < 0) {pos = 0;} // reset the horizontal position of the motor to 0 if it tries to move past this point
myservo.write(pos); // write the starting position to the horizontal motor
delay(500);

}