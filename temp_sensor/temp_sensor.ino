#include "DHT.h"        //DHT Library
#define DHTPIN 3       //Data Pin Connected to Arduino Uno Board
#define DHTTYPE DHT11
  // DHT 22  (AM2302)

DHT dht(DHTPIN, DHTTYPE);
float h; //stores humidity value
float t; //stores humidity value

void setup() 
{
  Serial.begin(9600); 
  Serial.println("DHT Test");
  Serial.println();
  Serial.println("Enter number for checking option:");
  Serial.println();
  Serial.println("1. Humidity");
  Serial.println("2. Temperature"); 
  dht.begin(); //begin the sensor 
}

void loop() 
{
//  char user_input; //allow user to key-in input
//  if (isnan(t) || isnan(h)) //if they are not a number (nan)
//  {
//  Serial.println("Failed to read from DHT");
//  }
//    {
//      while(Serial.available())
//      {
//        user_input = Serial.read();
//        if (user_input == '1')
//        {
//          Humidity();
//        }
//        else if (user_input == '2')
//        {
//          Temperature();
//        }
//        else
//        {
//          Serial.println("Failed. Invalid option entered");
//        }
//      }
//    }
  Temperature();
  delay(50);
}

void Humidity()
{
  h = dht.readHumidity();
  Serial.println();
  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.println("%\t");
}

void Temperature()
{
  t = dht.readTemperature(false);
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.println("*C");
}
