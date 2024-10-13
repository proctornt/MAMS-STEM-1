 int water=275;
  int dry=480;
  int range=dry-water;//205
  int intervals=int(range/5);//41
  int eightyPct =water+intervals;//246
  int sixtyPct=water+2*(intervals);//287
  int fourtyPct=water+3*(intervals);//428
  int twentyPct=water+4*(intervals);//469

#include <Servo.h>

Servo myservo;  

void setup() {
Serial.begin(9600);
myservo.attach(9);  
myservo.write(0);
}
int soilRead(){
int val;
  val = analogRead(1); 
  Serial.println(val);
  delay(2000);
  if (val <= water){

    Serial.println("Soaked");
  }
  else if (val>=dry){

    Serial.println("Dry");
  }
  else if (val > water && val < eightyPct){
    Serial.println("Between 80 and 100%");
  }
  else if (val > eightyPct && val <=sixtyPct){
    Serial.println("Between 60 and 80%");
  }
  else if (val> sixtyPct && val <=fourtyPct)
    Serial.println("Between 40 and 60%");
  else if (val>fourtyPct && val <=twentyPct)
    Serial.println("Bewten 20 and 40%");
  else if (val>twentyPct &&  val <= dry)
    Serial.println("Less than 20%");
  return val;

  
}

void fourtyFive(){
myservo.write(45);

}
void zero(){
  myservo.write(0);
}
void loop() {
  int moistureVal=soilRead();
  if (moistureVal>sixtyPct)
  fourtyFive();
  if (moistureVal<sixtyPct)
  zero();  

}
