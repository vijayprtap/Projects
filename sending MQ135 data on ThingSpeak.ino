#include <ThingSpeak.h>
#include <ESP8266WiFi.h>

WiFiClient client;

long mychnlnumber = 1666110;
const char mywriteapi[] = "ZFFBMA25JSTLSAYU";

void setup() {
Serial.begin(9600);
pinMode (A0,INPUT);
WiFi.begin("esp","naruto900");
while (WiFi.status() != WL_CONNECTED)
{
  Serial.print("..");
  delay(1000);
  }
 Serial.print("Connected To The Network");
 ThingSpeak.begin (client);

}

void loop() {
 int sensorval = analogRead(A0);
Serial.println (sensorval);
ThingSpeak.writeField(mychnlnumber,1,sensorval,mywriteapi);
delay(1000);

  }
