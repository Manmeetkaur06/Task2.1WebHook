#include <WiFiNINA.h>  // Library to connect to WiFi networks
#include <ThingSpeak.h>  // Library to send data to ThingSpeak platform
#include <DHT.h>  // Library to interface with DHT sensors
#include "secrets.h"  // Contains WiFi and ThingSpeak credentials

// DHT Sensor settings
#define DHTPIN 2     // Pin where the DHT sensor is connected
#define DHTTYPE DHT11   // Define the type of DHT sensor (DHT11)
DHT dht(DHTPIN, DHTTYPE);  // Initialize the DHT sensor

WiFiClient client;  // Create a WiFi client for ThingSpeak

void setup() {
  Serial.begin(9600);  // Start the Serial Monitor for debugging
  dht.begin();         // Initialize the DHT sensor
  WiFi.begin(SSID, PASSWORD);  // Connect to WiFi using credentials from secrets.h

  // Wait until the WiFi is connected
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");  // Print dots while connecting
  }
  Serial.println("\nConnected to WiFi");  // Confirmation of successful WiFi connection

  ThingSpeak.begin(client);  // Initialize the ThingSpeak connection
}

void loop() {
  // Read temperature and humidity from DHT11 sensor
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  // Check if any readings failed and exit early (to try again)
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");  // Error message if reading fails
    return;
  }

  // Print the sensor values to the Serial Monitor
  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.println(" *C");

  // Write data to ThingSpeak fields
  ThingSpeak.setField(1, t);  // Set temperature to field 1
  ThingSpeak.setField(2, h);  // Set humidity to field 2

  int x = ThingSpeak.writeFields(CHANNEL_NUMBER, WRITE_API_KEY);  // Send the data to ThingSpeak

  // Check if the data was successfully sent to ThingSpeak
  if (x == 200) {
    Serial.println("Channel update successful.");  // Success message
  } else {
    Serial.println("Problem updating channel. HTTP error code " + String(x));  // Error message with code
  }

  // Wait for 60 seconds before the next update
  delay(60000);
}
