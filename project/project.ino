#include "DHT.h"        //DHT Library#include <BlynkSimpleWiFiLink.h>
//#include <SoftwareSerial.h>
///#include <WiFiLink.h>
//#include <BlynkSimpleWiFiLink.h>

#define DHTPIN 4       //Data Pin Connected to Arduino Uno Board
#define DHTTYPE DHT11
//#define FLOW_SENSOR_INTERRUPT 1
//#define FLOW_SENSOR_PIN 3

#define BLYNK_PRINT Serial // Comment this out to disable prints and save space
byte FLOW_SENSOR_INTERRUPT = 2;
byte FLOW_SENSOR_PIN = 3;
// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "80a41c375609480a91a22f773c04772c";
char ssid[] = "omerlevko";
char pass[] = "";

DHT dht(DHTPIN, DHTTYPE);
float t; //stores humidity value
float m; // Stores moisture value


// Flow sensor variables initializations
// The hall-effect flow sensor outputs approximately 4.5 pulses per second per

// litre/minute of flow.

float calibrationFactor = 4.5;

volatile byte pulseCount = 0;
float flowRate = 0.0;
unsigned int flowMilliLitres = 0;
unsigned long totalMilliLitres = 0;
unsigned long oldTime = 0;



bool isDebug = true;

void setup() {
  Serial.begin(9600);
  delay(10);
  //Blynk.begin(auth, ssid, pass);

  dht.begin(); // Begins the sensor

  pinMode(FLOW_SENSOR_PIN, INPUT); // flow sensor
  digitalWrite(FLOW_SENSOR_PIN, HIGH);

  // The Hall-effect sensor is connected to pin 3(originally-2) which uses interrupt 1 (originally-0).
  // Configured to trigger on a FALLING state change (transition from HIGH
  // state to LOW state)
  attachInterrupt(FLOW_SENSOR_INTERRUPT, pulseCount, FALLING);
}

void loop() {
  //Blynk.run();
  // put your main code here, to run repeatedly:
  //t = GetTemp();
  //m = GetMoisture();
  GetFlow();
  delay(100);
}

float GetTemp() {
  float val = dht.readTemperature(false);
  if (isDebug) {
    Serial.print("Temperature: ");
    Serial.print(val);
    Serial.println("*C");
  }
  return val;
}

float GetMoisture() {
  // read the input on analog pin 0:
  int sensorValue = analogRead(A0);
  // print out the value you read:
  if (isDebug) {
    Serial.println(sensorValue);
  }
}

float GetFlow() {

  if ((millis() - oldTime) > 1000)   // Only process counters once per second
  {
    // Disable the interrupt while calculating flow rate and sending the value to
    // the host
    detachInterrupt(FLOW_SENSOR_INTERRUPT);

    // Because this loop may not complete in exactly 1 second intervals we calculate
    // the number of milliseconds that have passed since the last execution and use
    // that to scale the output. We also apply the calibrationFactor to scale the output
    // based on the number of pulses per second per units of measure (litres/minute in
    // this case) coming from the sensor.
    flowRate = ((1000.0 / (millis() - oldTime)) * pulseCount) / calibrationFactor;

    // Note the time this processing pass was executed. Note that because we've
    // disabled interrupts the millis() function won't actually be incrementing right
    // at this point, but it will still return the value it was set to just before
    // interrupts went away.
    oldTime = millis();

    // Divide the flow rate in litres/minute by 60 to determine how many litres have
    // passed through the sensor in this 1 second interval, then multiply by 1000 to
    // convert to millilitres.
    flowMilliLitres = (flowRate / 60) * 1000;

    // Add the millilitres passed in this second to the cumulative total
    totalMilliLitres += flowMilliLitres;

    unsigned int frac;

    if (isDebug) {
      // Print the flow rate for this second in litres / minute
      Serial.print("Flow rate: ");
      Serial.print(int(flowRate));  // Print the integer part of the variable
      Serial.print(".");             // Print the decimal point

      // Determine the fractional part. The 10 multiplier gives us 1 decimal place.
      frac = (flowRate - int(flowRate)) * 10;

      Serial.print(frac, DEC) ;      // Print the fractional part of the variable
      Serial.print("L/min");

      // Print the number of litres flowed in this second
      Serial.print("  Current Liquid Flowing: ");             // Output separator
      Serial.print(flowMilliLitres);
      Serial.print("mL/Sec");

      // Print the cumulative total of litres flowed since starting
      Serial.print("  Output Liquid Quantity: ");             // Output separator
      Serial.print(totalMilliLitres);
      Serial.println("mL");
    }

    // Reset the pulse counter so we can start incrementing again
    pulseCount = 0;

    // Enable the interrupt again now that we've finished sending output
    attachInterrupt(FLOW_SENSOR_INTERRUPT, pulseCount, FALLING);
  }
}
