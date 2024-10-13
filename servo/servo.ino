

#include <Servo.h>

Servo myservo;  

//int pos = 0;    // variable to store the servo position

void setup() {
  myservo.attach(9);  // attaches the servo on pin 9 to the Servo object
  myservo.write(0);
}

void loop() {
  myservo.write(45);
  delay(5000);
  myservo.write(0);
  delay(5000);
}
