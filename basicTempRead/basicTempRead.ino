//gnd->ground
//VCC->5v
//Sig->A0
//NC-> NOTHING, EMPTY, NO CURRENT
// humidity is [0]
// temperatur is [1]

#include "Grove_Temperature_And_Humidity_Sensor.h"
#define DHTTYPE DHT22 
#define DHTPIN A0    // what pin we're connected to（DHT10 and DHT20 don't need define it）
#define ledpin 4
DHT dht(DHTPIN, DHTTYPE);
double humidity=0.0;
double temperature=0.0;

void setup() {
Serial.begin(9600);
delay(500);
Serial.println("Temperature Sensor");
delay(1000);
Wire.begin();
dht.begin();
pinMode(ledpin, OUTPUT);

}
void readTempSensor(float* data){
  float temp_hums[2] = {0};
int success=dht.readTempAndHumidity(temp_hums);

    if (!success) {
        Serial.print("Humidity: ");
        double humidity=temp_hums[0];
        Serial.print(humidity);
        Serial.print(" %\t");
        double temperature=temp_hums[1];
        Serial.print("Temperature: ");
        Serial.print(temperature);
        Serial.println(" *C");

        data[0] = humidity;
        data[1] = temperature;
    } else {
        Serial.println("Failed to get temprature and humidity value.");
    }
}


void loop() {
  // put your main code here, to run repeatedly:
    // Reading temperature or humidity takes about 250 milliseconds!
    // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
    float tempData[2] = {0};
    readTempSensor(tempData);
    

    // if (sizeof(temp_hums)>0) {
        double humidity=tempData[0];
        double temperature=tempData[1];
        if (temperature>=30){
          Serial.println("Hot");
          digitalWrite(ledpin, HIGH);

        }
        else{

          digitalWrite(ledpin, LOW);
        }
    

    delay(1500);
    //}
}



