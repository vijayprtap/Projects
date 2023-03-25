/*    12v-----------------Vs
      5v------------------enable
      input1--------------d2
      input2--------------d3
      output1-------------motor
      output 2------------motor
      */
#define i1 2
#define i2 3
void setup() {
  // put your setup code here, to run once:
pinMode  (i1,OUTPUT);
pinMode (i2,OUTPUT);
Serial.begin(9600);
}

void loop() {
  digitalWrite(i1,HIGH);
digitalWrite(i2,LOW);
Serial.println("ANTI-CLOCKWISE");
delay(5000);
digitalWrite(i2,HIGH);
digitalWrite(i1,LOW);
Serial.println("CLOCKWISE");
delay(5000);
}
