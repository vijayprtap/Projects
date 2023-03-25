
#include "DHT.h"
#define DHTPIN 15    
#define DHTTYPE DHT11 
DHT dht(DHTPIN, DHTTYPE);
void setup() {
 dht.begin();
 Serial.begin(9600);
 Serial.println("DHTxx test!");
}
void loop() {
  // Wait a few seconds between measurements.
  delay(2000);
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  Serial.print(" Humidity: ");
  Serial.print(h); 
  Serial.print((" Temperature: "));
  Serial.print(t);
  Serial.println(" ");
  
}

