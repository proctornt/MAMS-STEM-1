

#include <Servo.h>

Servo myservo;  

//int pos = 0;    // variable to store the servo position

void setup() {
  myservo.attach(9);  // attaches the servo on pin 9 to the Servo object
  myservo.write(180);
}

void loop() {
  myservo.write(130);
  delay(500);
  myservo.write(180);
  delay(10000);//would stop there, would write another function to turn off 
}
