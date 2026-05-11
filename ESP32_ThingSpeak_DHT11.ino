#include "WiFi.h"
#include <Adafruit_Sensor.h>
#include "DHT.h"

#define DHT11_PIN 23
#define DHTTYPE DHT11

// 1. Enter your Wi-Fi network name and password here
const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";

// 2. Enter your Channel ID and Write API Key here
unsigned long myChannelNumber = 0000000;         // Replace with your Channel ID
String APIKEY = "YOUR_THINGSPEAK_API_KEY";       // Replace with your Write API Key

const char* host = "api.thingspeak.com";

int T, H;
DHT dht(DHT11_PIN, DHTTYPE);
WiFiClient client;

void Connect_WiFi() {
  WiFi.begin(ssid, password);
  while(WiFi.status() != WL_CONNECTED) {
    delay(1000);
  }
}

void TempHum() {
  H = dht.readHumidity();       // Read humidity
  T = dht.readTemperature();    // Read temperature
}

void setup() {
  Serial.begin(9600);
  dht.begin();
  Connect_WiFi();
  client.connect(host, 80);
}

void loop() {
  client.connect(host, 80);
  TempHum();
  
  // Send data to ThingSpeak
  client.print("GET /update?api_key=" + APIKEY + "&field1=");
  client.print(String(T));
  client.print("&field2=");
  client.print(String(H));
  client.print(" HTTP/1.0\r\nHost: api.thingspeak.com\r\n\r\n");
  
  // Wait 30 seconds before sending the next reading
  delay(30000);
}
