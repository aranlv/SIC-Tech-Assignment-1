#include <WiFi.h>
#include <HTTPClient.h>
#include <DHT.h>

#define DHTPIN 26      
#define DHTTYPE DHT11 

const char* ssid = "cestlavie";           
const char* password = "eggtarts!";      
const char* serverName = "http://192.168.101.70:5000/data"; 

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  dht.begin();
  
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  
  Serial.println("Connected to WiFi");
}

void loop() {
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  Serial.print("Temp: ");
  Serial.print(temperature);
  Serial.print(" C ");
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println(" % ");
  delay(2000);
  
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    
    if (isnan(humidity) || isnan(temperature)) {
      Serial.println("Failed to read from DHT sensor!");
      return;
    }

    String postData = "{\"temperature\":" + String(temperature) + ",\"humidity\":" + String(humidity) + "}";

    http.begin(serverName);
    http.addHeader("Content-Type", "application/json");
    
    int httpResponseCode = http.POST(postData);

    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println(httpResponseCode);
      Serial.println(response);
    } else {
      Serial.print("Error on sending POST: ");
      Serial.println(httpResponseCode);
    }
    
    http.end();
  } else {
    Serial.println("WiFi Disconnected");
  }
  
  delay(60000); 
}
