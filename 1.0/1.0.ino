int valvePin1=2;
int valvePin2=3;

 int water=385;
  int dry=780;
  int range=dry-water;//205
  int intervals=int(range/5);//41
  int eightyPct =water+intervals;//246
  int sixtyPct=water+2*(intervals);//287
  int fourtyPct=water+3*(intervals);//428
  int twentyPct=water+4*(intervals);//469
int val;
int pumpStatus;
int valveStatus;

#include <Servo.h>
Servo myservo;  

void setup() {
Serial.begin(9600);
myservo.attach(9);  
myservo.write(180);
pinMode(5, OUTPUT);
pinMode(valvePin1,OUTPUT);
pinMode(valvePin2,OUTPUT);

}


int readSoil(){
val = analogRead(0); 
}
void togglePump() {
myservo.write(130);
delay(500);
myservo.write(180);
Serial.println("Pump toggled");
if (pumpStatus==1){
   pumpStatus=0;
}else {
  pumpStatus=1;
}

}
void openValve(){
digitalWrite(valvePin1, HIGH);
digitalWrite(valvePin2, LOW);
delay(800);
digitalWrite(valvePin1, LOW);
digitalWrite(valvePin2, LOW);
digitalWrite(5,HIGH);
valveStatus=1;
}

void closeValve(){
digitalWrite(valvePin1, LOW);
digitalWrite(valvePin2, HIGH);
delay(800);
digitalWrite(valvePin1, LOW);
digitalWrite(valvePin2, LOW);
digitalWrite(5,LOW);
valveStatus=0;
}
void waterOn(){
if (pumpStatus==0){
  togglePump();
  delay(3000);
  openValve();
}
}
void waterOff(){
  if (pumpStatus==1){
    togglePump();
    delay(3000);//do I need to do thus
    closeValve();
    Serial.println("Water off"); 
    
  }
}

void loop() {
readSoil();
Serial.println(val);
Serial.println(pumpStatus);
Serial.println("Complete");
if (val>sixtyPct && pumpStatus==0){
  waterOn();
}
else if (val<twentyPct && pumpStatus==1){
  waterOff();
}
delay(2000);
}


