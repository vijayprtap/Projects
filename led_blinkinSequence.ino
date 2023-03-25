#define led 8
#define led2 9
#define led3 10


void setup() {
pinMode(led,OUTPUT);
pinMode(led2,OUTPUT);
pinMode(led3,OUTPUT);

}

void loop() {
digitalWrite(led3,HIGH);
delay(300);
digitalWrite(led2,HIGH);
delay(200);
digitalWrite(led,HIGH);
delay(100);

digitalWrite(led,LOW);
digitalWrite(led2,LOW);
digitalWrite(led3,LOW);

delay(100);
}
