#include "PMS.h"
#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <DHT.h>
#include <DHT_U.h>
#include "DHT.h"
#define DHTPIN D3     // what digital pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
DHT dht(DHTPIN, DHTTYPE);
WiFiClient client;
SoftwareSerial pmsSerial(D4, D5);
PMS pms(pmsSerial);
PMS::DATA data;
float pm10 = 0;
float pm25 = 0;
// replace with your channel’s thingspeak API key and your SSID and password
String apiKey = "E9T10WNQAO6OR5SE";
const char* ssid = "wang2";
const char* password = "bc2858bc2858@";
const char* server = "api.thingspeak.com";

 
void setup()
{
  Serial.begin(115200);
  pmsSerial.begin(9600);
  dht.begin();
  WiFi.begin(ssid, password);
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) 
  {
   delay(500);
   Serial.print(".");
  }
   Serial.println("");
   Serial.println("WiFi connected");
}
void loop()
{
 if (pms.read(data))
{
    pm10 =   data.PM_AE_UG_1_0;
    pm25 =   data.PM_AE_UG_2_5;
 //
    float h = dht.readHumidity();
    float t = dht.readTemperature();
    float f = dht.readTemperature(true);
  // Compute heat index in Fahrenheit (the default)
    float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
    float hic = dht.computeHeatIndex(t, h, false);
 // 
     if (client.connect(server,80)) {
     String postStr = apiKey;
     postStr +="&field1=";
     postStr += String(pm10);
     postStr +="&field2=";
     postStr += String(pm25);
     postStr +="&field3=";
     postStr += String(h);
     postStr +="&field4=";
     postStr += String(t);
     postStr += "\r\n\r\n\r\n\r\n";

     client.print("POST /update HTTP/1.1\n");
     client.print("Host: api.thingspeak.com\n");
     client.print("Connection: close\n");
     client.print("X-THINGSPEAKAPIKEY: "+apiKey+"\n");
     client.print("Content-Type: application/x-www-form-urlencoded\n");
     client.print("Content-Length: ");
     client.print(postStr.length());
     client.print("\n\n\n\n");
     client.print(postStr);
     Serial.print("PM1.0: ");
     Serial.print(pm10);
     Serial.print("PM2.5: ");
     Serial.print(pm25); 
     Serial.print("Temperature: ");
     Serial.print(t);
     Serial.print(" degrees Celsius Humidity: ");
     Serial.print(h);
     Serial.println("Sending data to Thingspeak"); 
    }
     client.stop();
     Serial.println("Waiting 20 secs");
     delay(20000); 
}

}
// Do
