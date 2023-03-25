#define led 7
#define pot A0

void setup() {
pinMode(7,OUTPUT);
pinMode(A0,INPUT);
Serial.begin(9600);

}

void loop() {
int potval = analogRead(A0);
Serial.println (potval);
digitalWrite (led,HIGH);
delay(potval);
digitalWrite (led,LOW);
delay(potval);

}
