#include<ESP8266WiFi.h>
#include<ESP8266HTTPClient.h>

String host = "h2c.in";
int httpport = 80;
String url = "/ecoair/dev_api/gdt.php";
WiFiClient client;
HTTPClient http;

void setup() {
pinMode (A0,INPUT);
  WiFi.begin("esp","naruto900");
Serial.begin(9600);
while (WiFi.status() != WL_CONNECTED)
{
  Serial.print("..");
  delay(1000);
  }

}

void loop() {

String link = url;
http.begin(client,host,httpport,link);
int httpcode = http.GET();
String data = http.getString();
Serial.println( data );
Serial.println( "Http Code : " + (String) httpcode);
delay(2000);

}
