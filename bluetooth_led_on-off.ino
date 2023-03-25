char val;


void setup() {

pinMode(5,OUTPUT);
Serial.begin(9600);
 digitalWrite(5,HIGH);
  
}

void loop() {

  if(Serial.available()){
 val = Serial.read();
 Serial.println(val);
}
if(val=='1'){
  digitalWrite(5,LOW);
 
}
else if(val=='2'){
  digitalWrite(5,HIGH);

}


delay(100);

}
