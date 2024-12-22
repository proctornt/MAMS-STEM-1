#include <Servo.h>
#include <Arduino_USBHostMbed5.h>
#include <DigitalOut.h>
#include <FATFileSystem.h>
#include <Wire.h>
#include <RTClib.h>

#include <SPI.h>
#include <WiFi.h>
#include <ArduinoJson.h>

int valvePin1=2;//valve connections
int valvePin2=3;//valce connection

int targetLow=580;
int targetHigh=630;
int targetRainVal=15;
int soilVal;
int pumpStatus;
int valveStatus;
int err;
int hour;
int minute;
int second; 
int todayRainPCT;
uint32_t start;
uint32_t end;
char ssid[] = "Proctor";        // your network SSID (name)
char pass[] = "Proctor#2003!";    // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;            // your network key index number (needed only for WEP)

int status = WL_IDLE_STATUS;
// if you don't want to use DNS (and reduce your sketch size)
// use the numeric IP instead of the name for the server:
//IPAddress server(74,125,232,128);  // numeric IP for Google (no DNS)
char server[] = "api.weather.gov";    // name address for Google (using DNS)

// Initialize the Ethernet client library
// with the IP address and port of the server
// that you want to connect to (port 80 is default for HTTP):
WiFiSSLClient client;

//#define max_json 1



// Create an RTC object
RTC_DS3231 rtc;
DateTime now;
USBHostMSD msd;
mbed::FATFileSystem usb("usb");
mbed::DigitalOut otg(PB_8, 1);

Servo myservo;  

void setup() {
  Serial.begin(9600);
  myservo.attach(9);  
  myservo.write(180);
  pinMode(5, OUTPUT);
  pinMode(valvePin1,OUTPUT);
  pinMode(valvePin2,OUTPUT);
  pinMode(PA_15, OUTPUT); //enable the USB-A port
  digitalWrite(PA_15, HIGH);
  while (!Serial);
    
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
      Serial.println("Couldn't find RTC");
      rtc.begin();
      while (1); // Halt if RTC isn't found
      
    }
    Serial.println("RTC found.");

    if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    while (true);
  }
while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);

    // wait 5 seconds for connection:
    delay(5000);
  }
Serial.println("Connected to WiFi");


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

int readRain(){
  Serial.println("\nStarting connection to server...");
  // if you get a connection, report back via serial:
  if (client.connect(server, 443)) {
    Serial.println("connected to server");

    // Make a HTTP request:
    // client.println("GET /gridpoints/BOX/46,74/forecast HTTP/1.0"); // Oxford, MA
    //client.println("GET /gridpoints/OTX/47,122/forecast HTTP/1.0"); // Spokane, WA
    client.println("GET /gridpoints/ILN/39,82/forecast HTTP/1.0"); // Columbus, OH
    client.println("User-Agent: arduino, proctornt@gmail.com");
    client.println("Host: api.weather.gov");
    client.println("Accept: application/geo+json,*/*");
    client.println("Connection: close");
    client.println();
    delay(1000);
  }

  while (client.available()) {
    String line = client.readStringUntil('\r');
    // Serial.println(line);
    if (line.indexOf("{") != -1) {  // Simple check to make sure it’s a valid JSON response
      DynamicJsonDocument doc(1024);
      
      // Parse the JSON from the server response
      DeserializationError error = deserializeJson(doc, line);

      if (error) {
        Serial.print("Failed to parse JSON: ");
        Serial.println(error.f_str());
      }
      else {
        // Extract specific data from JSON
          todayRainPCT= doc["properties"]["periods"][0]["probabilityOfPrecipitation"]["value"].as<long>();
      }
    }
  }
  
  if (!client.connected()) {
    Serial.println();
    Serial.println("disconnecting from server.");
    client.stop();

  }
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
  readRain();
  if (todayRainPCT<targetRainVal){
    Serial.println("it wont rain");
    if (pumpStatus==0){
      togglePump();
      delay(3000);
      openValve();
    }
  }
}
void waterOff(){
  if (pumpStatus==1){
    togglePump();
    delay(3000);//do I need to do thus
    closeValve();
    
  }
}

/*
**********************
 Main Loop
**********************
*/
void loop() {
  // read chance of rain from NWS
  readRain();
  Serial.print("Rain percentage: ");
  Serial.println(todayRainPCT);
  // read soil moisture
  readSoil();
  //Serial.println(soilVal);
  if (soilVal>targetHigh){
    waterOn();
  }
  else if (soilVal<=targetLow){
    waterOff();
  }
  // wait two seconds before polling soil and NWS.
  delay(2000);
  }
