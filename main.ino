 // Import required libraries
#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <LiquidCrystal_I2C.h>
#include "FS.h"
#include "LittleFS.h"
#include "Secrets.h"

const char* ssid = SSID;
const char* password = PASSWORD;

#define DHTPIN 2
#define DHTTYPE DHT11    

LiquidCrystal_I2C lcd(0x27, 16, 2);
DHT dht(DHTPIN, DHTTYPE);
AsyncWebServer server(80);

String readDHTTemperature() {
  float t = dht.readTemperature();

  if (isnan(t)) {    
    Serial.println("Failed to read from DHT sensor!");
    return "--";
  }
  else {
    Serial.println(t);
    return String(t);
  }
}

String readDHTHumidity() {
  float h = dht.readHumidity();

  if (isnan(h)) {
    Serial.println("Failed to read from DHT sensor!");
    return "--";
  }
  else {
    Serial.println(h);
    return String(h);
  }
}

// Replaces placeholder with DHT values
String processor(const String& var){
  //Serial.println(var);
  if(var == "TEMPERATURE"){
    return readDHTTemperature();
  }
  else if(var == "HUMIDITY"){
    return readDHTHumidity();
  }
  return String();
}

void serverRoute(){
  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS, "/index.html", String(), false, processor);
  });
  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS, "/style.css", "text/css");
  });
  server.on("/script.js", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS, "/script.js", "application/javascript");
  });
  server.on("/img/weather1.png", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS, "/img/weather1.png", "image/png");
  });
  server.on("/img/weather.png", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS, "/img/weather.png", "image/png");
  });
  server.on("/img/wind.png", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS, "/img/wind.png", "image/png");
  });
  server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/plain", readDHTTemperature().c_str());
  });
  server.on("/humidity", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/plain", readDHTHumidity().c_str());
  });

  server.begin();
}

void wifiConnection(){
    // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  // Print ESP32 IP Address
  Serial.println(WiFi.localIP());
}
void setup(){
  Serial.begin(115200);

  dht.begin();
  lcd.begin();
  lcd.backlight();

  if(!LittleFS.begin(true)){
    Serial.println("An Error has occurred while mounting LittleFS");
    return;
  }
  Serial.println("LittleFS mounted successfully");

  wifiConnection();

  serverRoute();
}
 
void loop(){
  float humi  = dht.readHumidity();
  float tempC = dht.readTemperature();

  lcd.clear();
  
  if (isnan(tempC) || isnan(humi)) {
    lcd.setCursor(0, 0);
    lcd.print("Failed");
  } else {
    lcd.setCursor(0, 0);  // display position
    lcd.print("Temp: ");
    lcd.print(tempC);     // display the temperature
    lcd.print("°C");

    lcd.setCursor(0, 1);  // display position
    lcd.print("Humi: ");
    lcd.print(humi);      // display the humidity
    lcd.print("%");
  }

  delay(10000);

  lcd.clear();
  lcd.print(WiFi.localIP());

  delay(2000);
}
