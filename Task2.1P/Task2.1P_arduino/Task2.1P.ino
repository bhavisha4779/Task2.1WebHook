#include "DHT.h"
#include <WiFiNINA.h>
#include <ArduinoHttpClient.h>

#define DHTPIN 2         // DHT22 data pin connected to D2
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

// WiFi credentials
char ssid[] = "MERALAPTOP 2717";      
char pass[] = "9317724941";  

// ThingSpeak settings
char server[] = "api.thingspeak.com";
String apiKey = "9PCGX40LBJOWF3RL"; // From ThingSpeak

WiFiClient wifi;
HttpClient client = HttpClient(wifi, server, 80);

void setup() {
  Serial.begin(9600);
  dht.begin();

  // Connect to WiFi
  while (WiFi.begin(ssid, pass) != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected!");
}

void loop() {
  float temp = dht.readTemperature();    // Celsius

  if (isnan(temp)) {
    Serial.println("Failed to read from DHT22");
    return;
  }

  // Prepare data
  String data = "field1=" + String(temp);

  // POST data to ThingSpeak
  client.post("/update?api_key=" + apiKey, "application/x-www-form-urlencoded", data);

  int statusCode = client.responseStatusCode();
  String response = client.responseBody();

  Serial.print("Status: ");
  Serial.println(statusCode);
  Serial.print("Response: ");
  Serial.println(response);

  delay(30000); // Wait 30 seconds
}
