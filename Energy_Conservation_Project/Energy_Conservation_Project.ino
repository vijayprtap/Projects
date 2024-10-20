// connect gnd and signal only .
#include <LiquidCrystal_I2C.h>

#include<ESP8266WiFi.h>
#include<ESP8266HTTPClient.h>
String host = "api.thingspeak.com";
int httpport = 80;
String url = "/update?api_key=M9NPXI1R314EJEL2&field1=0";
WiFiClient client;
HTTPClient http;

int IR1 = 14;//D5
int IR2 = 12;//D6
int LED1 = 13;//D7
int LED2 = 15;//D8
const int voltageSensor = A0;

float vOUT = 0.0;
float vIN = 0.0;
float R1 = 30000.0;
float R2 = 7500.0;
int value = 0;

LiquidCrystal_I2C lcd(0x27,16,2);// set the LCD address to 0x27 for a 16 chars and 2 line display

//////////////////////////////////////////////////////////////////////////////
unsigned long previousMillis = 0;  //will store last time LED was blinked
const long period = 40000;         // period at which to blink in ms
///////////////////////////////////////////////////////////////////////////////




void setup() {
lcd.init();
lcd.backlight();

pinMode(IR1, INPUT);
pinMode(IR2, INPUT); 
pinMode(LED1, OUTPUT); 
pinMode(LED2, OUTPUT);  
pinMode (A0,INPUT);

lcd.setCursor(5,0);
lcd.print("ENERGY");
lcd.setCursor(0,1);
lcd.print("CONSERVATION SYS");


 
  WiFi.begin("esp","naruto900");
Serial.begin(9600);
while (WiFi.status() != WL_CONNECTED)
{
  Serial.print("..");
  delay(1000);
  }
lcd.clear();
}

void loop() {
 

value = analogRead(voltageSensor);
  vOUT = (value * 5.0) / 1024.0;
  vIN = vOUT / (R2/(R1+R2));

 lcd.setCursor(0,0);
lcd.print("VOLTAGE:" + String(vIN)+ String("v"));
 

  
    int sensorStatus = digitalRead(IR1); // Set the GPIO as Input
  if (sensorStatus == 1) // Check if the pin high or not
  {
  // if the pin is high turn off the onboard Led
    digitalWrite(LED1, LOW); // LED LOW
        digitalWrite(LED2, LOW); // LED LOW
lcd.setCursor(0,1);
lcd.print("LIGHT STATUS:OFF");
        

    Serial.println("Motion Ended!"); // print Motion Detected! on the serial monitor window
  }
  else
  {
    //else turn on the onboard LED
    digitalWrite(LED1, HIGH); // LED High
        digitalWrite(LED2, HIGH); 
         lcd.clear();

lcd.setCursor(0,1);
lcd.print("LIGHT STATUS:ON");
 lcd.setCursor(0,0);
lcd.print("VOLTAGE:" + String(vIN)+ String("v"));
    Serial.println("Motion Detected!"); // print Motion Ended! on the serial monitor window
delay(2500); 
 }


///////////////////////////////////////////////////////////////////////////
unsigned long currentMillis = millis(); // store the current time
  if (currentMillis - previousMillis >= period) { // check if 1000ms passed
   previousMillis = currentMillis;   // save the last time you blinked the LED
   

   Serial.println("sensor data : " + (String) vIN);
String link = url +(String) vIN;
http.begin(client,host,httpport,link);
int httpcode = http.GET();
Serial.println( "Http Code : " + (String) httpcode);
 lcd.clear();
 
lcd.setCursor(3,0);
lcd.print("SOLAR DATA"); 
lcd.setCursor(5,1);
lcd.print("UPLOADING");

} 

///////////////////////////////////////////////////////////////////////////////////
delay(200);

}