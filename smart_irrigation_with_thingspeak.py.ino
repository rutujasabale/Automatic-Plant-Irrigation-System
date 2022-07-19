
const char* ssid     = "realme 7i";    //Name of your wifi network
const char* password = "123456789";  // Wifi/*

#include <ThingSpeak.h>  //To connect our thingspeak channel with the esp8266 through this code.
#include <ESP8266WiFi.h>  // To connect the esp with internet
#include <DHT.h>
#include <Wire.h>           
    
/* Change these values based on your calibration values */
int soilWet = 65;        // Define max value we consider soil 'wet'   
int soilDry = 35;        // Define min value we consider soil 'dry'

#define sensorPin A0     //connect the sensor to analog pin of esp8266
#define motorPin D8
float Humidity = 0.0 ;
float Temperature = 0.0 ;


//---------------------Channel Details--------------------//
unsigned long Channel_ID = 1780839; // Channel ID
const char *WriteAPIKey = "BSFRYTQ5ZM9THDSH"; // Your write API Key
WiFiClient  client;

#define DHTPIN 2              //D4 pin
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
long delayStart = millis();   // start delay

void setup() {
   pinMode(motorPin, OUTPUT);
   digitalWrite(motorPin, HIGH);
   Serial.begin(9600); 
   WiFi.begin(ssid, password);  // Connecting esp to wifi
   while (WiFi.status() != WL_CONNECTED)  //This loop will work untill esp connects to the wifi
   {   
   delay(500);
   }
   ThingSpeak.begin(client);       //The client here is esp8266 
  delay(1000);

  }


void loop() {
  Humidity = dht.readHumidity();
  Temperature = dht.readTemperature();
  delay(1000);
   //get the reading from the function below and print it
  int moisture = analogRead(sensorPin);    //Read the analog values
  moisture = map(moisture, 0, 1024, 100, 0);
  
  if ((millis() - delayStart) >= 15000) {
  ThingSpeak.writeField(Channel_ID,1,Humidity, WriteAPIKey);
  ThingSpeak.writeField(Channel_ID,2,Temperature, WriteAPIKey);
  ThingSpeak.writeField(Channel_ID,3,moisture, WriteAPIKey);
  // Determine status of our soil using the values already defined for wet and dry soil
  if (moisture > soilWet) 
  {
    digitalWrite(motorPin, HIGH);
    ThingSpeak.writeField(Channel_ID,4, false, WriteAPIKey);
  } 
  else if (moisture <= soilWet && moisture > soilDry) 
  {
    digitalWrite(motorPin, HIGH); 
    ThingSpeak.writeField(Channel_ID,4, false, WriteAPIKey);   
  } 
  else 
  {
    digitalWrite(motorPin, LOW);
    delay(5000);
    digitalWrite(motorPin, HIGH);
    ThingSpeak.writeField(Channel_ID,4, true, WriteAPIKey);
  }
} 
delay(500);                // Take a reading every  half a second for testing
}
