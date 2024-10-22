int pumpPin1=2;
int pumpPin2=3;

void setup() {
pinMode(pumpPin1,OUTPUT);
pinMode(pumpPin2,OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
digitalWrite(pumpPin1, LOW);
digitalWrite(pumpPin2, HIGH);
delay(15000);
digitalWrite(pumpPin1, LOW);
digitalWrite(pumpPin2, LOW);
delay(10000);


}