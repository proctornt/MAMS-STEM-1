#include <Wire.h>
#include <RTClib.h>

// Create an RTC object
RTC_DS3231 rtc;

void setup() {
  // Setup Serial connection
  Serial.begin(115200);
  delay(1000);  // Wait for Serial Monitor to open
  Serial.println("Starting RTC setup...");

  // Check if the RTC is connected properly
  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1); // Halt if RTC isn't found
  }
  Serial.println("RTC found.");

  // Check if the RTC lost power and set the time
  if (rtc.lostPower()) {
    Serial.println("RTC lost power, setting time to default.");
    // Set the RTC to a known time (compile time of the sketch)
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  } else {
    Serial.println("RTC time is running correctly.");
  }

  Serial.println("RTC setup complete.");
}

void loop() {
  DateTime now = rtc.now();
                                                   

  Serial.print("Date: ");
  Serial.print(now.month(), DEC);  
  Serial.print('/');
  Serial.print(now.day(), DEC);    
  Serial.print('/');
  Serial.print(now.year(), DEC);   
  Serial.print(" -- ");

  Serial.print("Time: ");
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.println(now.second(), DEC);

  delay(1000);
}
