#include <Servo.h>
#include <Arduino_USBHostMbed5.h>
#include <DigitalOut.h>
#include <FATFileSystem.h>
#include <Wire.h>
#include <RTClib.h>

nt valvePin1=2;//valve connections
int valvePin2=3;//valce connection

int targetLow=580;
int targetHigh=630;
int soilVal;
int pumpStatus;
int valveStatus;
int err;
int hour;
int minute;
int second; 
uint32_t start;
uint32_t end;


// Create an RTC object
RTC_DS3231 rtc;
DateTime now;
USBHostMSD msd;
mbed::FATFileSystem usb("usb");
mbed::DigitalOut otg(PB_8, 1);

Servo myservo;  

void setup() {
 // Serial.begin(9600);
  myservo.attach(9);  
  myservo.write(180);
  pinMode(5, OUTPUT);
  pinMode(valvePin1,OUTPUT);
  pinMode(valvePin2,OUTPUT);
  pinMode(PA_15, OUTPUT); //enable the USB-A port
  digitalWrite(PA_15, HIGH);
  //while (!Serial);
    
    msd.connect();

    while (!msd.connected()) {
      msd.connect();
      //Serial.println("Trying");
      delay(1000);
    }
  err =  usb.mount(&msd);
  mbed::fs_file_t file;
  struct dirent *ent;
  int dirIndex = 0;
  int res = 0;
  writeUsbStr("Start Here");
  writeUsbSoil(00000);
  if (!rtc.begin()) {
      //Serial.println("Couldn't find RTC");
      rtc.begin();
      while (1); // Halt if RTC isn't found
      
    }
    //Serial.println("RTC found.");

}

void writeUsbSoil(int soil){
  FILE *f = fopen("/usb/soil.txt", "a+");
  fflush(stdout);
  err = fprintf(f, "%d\n", soil);
  //Serial.println(soil);
  fflush(stdout);
  err = fclose(f);

}
void writeUsbStr(char* text){
  FILE *f = fopen("/usb/data.txt", "a+");
  fflush(stdout);
  err = fprintf(f, "%s\n", text);
  //Serial.println(text);
  fflush(stdout);
  err = fclose(f);
}

int readSoil(){
  
  soilVal = analogRead(0);
  //Serial.println(soilVal);
  writeUsbSoil(soilVal);
}
void togglePump() {
  myservo.write(130);
  delay(500);
  myservo.write(180);
  if (pumpStatus==1){
    pumpStatus=0;
  }
  else {
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
  start = rtc.now().unixtime(); 

  //Serial.println(start); 
}

void closeValve(){
  digitalWrite(valvePin1, LOW);
  digitalWrite(valvePin2, HIGH);
  delay(800);
  digitalWrite(valvePin1, LOW);
  digitalWrite(valvePin2, LOW);
  digitalWrite(5,LOW);
  valveStatus=0;
  end = rtc.now().unixtime(); 
  //Serial.println(start);
  //Serial.println(end);
  uint32_t elapsed = end-start;
  

  int day=rtc.now().day();
  char dest[30];
  char timeArr[4];
  char dayArr[2];
  sprintf(timeArr,"%d",elapsed);
  sprintf(dayArr,"%d",day);
  strcpy(dest,dayArr);
  strcat(dest,",");
  strcat(dest,timeArr);
  writeUsbStr(dest);
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
    
  }
}

void loop() {
  readSoil();
  //Serial.println(soilVal);
  if (soilVal>targetHigh){
    waterOn();
  }
  else if (soilVal<=targetLow){
    waterOff();
  }
  delay(2000);
}