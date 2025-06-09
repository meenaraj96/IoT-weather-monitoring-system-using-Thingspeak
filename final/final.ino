//#include <WiFi.h>
#include <ESP8266WiFi.h>
#include "ThingSpeak.h"
#include "DHT.h"

#define DHTPIN 4  //GPIO4 - D2- SDA   // Digital pin connected to the DHT sensor  
#define DHTTYPE DHT11   // DHT 22  (AM2302), AM2321
DHT dht(DHTPIN, DHTTYPE);

 char* ssid = "";   // your network SSID (name) 
 char* password = "";   // your network password
WiFiClient  client;

unsigned long myChannelNumber = ; //ChannelNumber
const char * myWriteAPIKey = ""; //myWriteAPIKey

// Timer variables
unsigned long lastTime = 0;
unsigned long timerDelay = 30000;
int stat;

// Variable to hold temperature readings
float temperatureC;
float Humidity;

void setup() {
  Serial.begin(115200);  //Initialize serial
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo native USB port only
  }
  //initBME();
  WiFi.mode(WIFI_STA);   
  ThingSpeak.begin(client);  // Initialize ThingSpeak
  dht.begin();
}

void loop() {
  if ((millis() - lastTime) > timerDelay) {
    
    // Connect or reconnect to WiFi
    if(WiFi.status() != WL_CONNECTED){
      Serial.print("Attempting to connect");
      while(WiFi.status() != WL_CONNECTED){
          //stat = WiFi.begin(ssid, password);
          //Serial.print(stat);
        WiFi.begin(ssid, password); 
        delay(5000);     
        Serial.print(".");
      } 
      Serial.println("\nConnected.");
    }

      delay(2000); //Gives Sensor Reading for every 2 Seconds
    
      // Reading temperature or humidity takes about 250 milliseconds!
      // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
      float h = dht.readHumidity();
      // Read temperature as Celsius (the default)
      float t = dht.readTemperature();
      // Read temperature as Fahrenheit (isFahrenheit = true)
      float f = dht.readTemperature(true);
    
      // Check if any reads failed and exit early (to try again).
      if (isnan(h) || isnan(t) || isnan(f)) 
      {
        Serial.println(F("Failed to read from DHT sensor!"));
        return;
      }
      else
      {
        ThingSpeak.setField(1, h);
        ThingSpeak.setField(2, t);
        int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
        if(x == 200){
          Serial.println("Channel update successful.");
        }
        else{
          Serial.println("Problem updating channel. HTTP error code " + String(x));
        }
        lastTime = millis();
      }
    
      Serial.print(F("Humidity: "));
      Serial.print(h);
      Serial.print(F("%  Temperature: "));
      Serial.print(t);
      Serial.print(F("°C "));
      Serial.print(f);
      Serial.println(F("°F"));    
    
  }
}
