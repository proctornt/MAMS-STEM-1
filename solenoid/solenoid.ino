int valvePin1=2;
int valvePin2=3;

void setup() {
pinMode(valvePin1,OUTPUT);
pinMode(valvePin2,OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
digitalWrite(valvePin1, HIGH);
digitalWrite(valvePin2, LOW);
delay(200);
digitalWrite(valvePin1, LOW);
digitalWrite(valvePin2, LOW);
delay(5000);
digitalWrite(valvePin1, LOW);
digitalWrite(valvePin2, HIGH);
delay(200);
digitalWrite(valvePin1, LOW);
digitalWrite(valvePin2, LOW);
delay(5000);


}