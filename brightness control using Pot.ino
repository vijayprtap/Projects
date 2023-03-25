int pot= A0;
int led= 5;

int potval;
int brightness;

void setup() {
pinMode (5,OUTPUT);
Serial.begin(9600);

}

void loop() {
potval = analogRead(pot);
Serial.println(potval);
brightness= map(potval,0,1023,0,255);
analogWrite(led,brightness);
delay(200);
}
