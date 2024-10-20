#include <ESP8266WiFi.h>
#include <LiquidCrystal_I2C.h>
#include<ESP8266HTTPClient.h>
#include <Arduino.h>

String host = "139.144.5.107";
int httpport = 80;
String url = "http://139.144.5.107/api/bin_level";

WiFiClient client;
HTTPClient http;

LiquidCrystal_I2C lcd(0x27, 16, 2);

//----------- Enter you Wi-Fi Details---------//
char ssid[] = "esp"; //SSID
char pass[] = "naruto900"; // Password
//-------------------------------------------//

// ---------- garbage bin details ------------//
const int total_height = 27; // garbage bin height in CM
//-------------------------------------------//

//----------- Channel Details -------------//
//unsigned long Channel_ID = 2036906; // Channel ID
//const char * mywriteapi= "LLDIQK2Z8A9UE6CR"; // Your write API Key
// ----------------------------------------//

// defines pins numbers
const int trigPin = 14;//D5
const int echoPin = 12;//D6
int buzzerpin = 0;//D3
// defines variables
unsigned int duration;
int distance;
void setup() {
lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("IoT Garbage lvl");
  lcd.setCursor(0, 1);
  lcd.print("Monitoring Sys.");
  
  WiFi.begin("esp","naruto900");
while (WiFi.status() != WL_CONNECTED)
{
  Serial.print("..");
  delay(1000);
  }
  
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  pinMode(buzzerpin, OUTPUT); // Sets the buzzerpin as an Output
  
 Serial.begin(9600); // Starts the serial communication
 Serial.print("Connected To The Network");
  
}
void loop() {


  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  distance = duration * 0.034 / 2;
  // Prints the distance on the Serial Monitor
  Serial.print(" Distance: ");
  Serial.println(distance);


int garbagelvl=map(distance,total_height,0,0,100);
lcd.setCursor(0, 0);
lcd.print("Garbage Monitor");
lcd.setCursor(0, 1);
Serial.print(" Garbage lvl:" + String(garbagelvl)+"%");
lcd.print("Garbage lvl:" + String(garbagelvl)+"%");



http.begin(client,host,httpport,url);

http.addHeader("Content-Type", "application/x-www-form-urlencoded");

String requestbody ="?id=sdf&level="+ String(garbagelvl)+"%";
requestbody = url+requestbody;
int httpcode = http.POST(requestbody);
Serial.println(requestbody);
String payload = http.getString();
Serial.println("payload: " + payload);


Serial.println( "  Http Code : " + (String) httpcode);


  delay(1500);
lcd.clear();
}