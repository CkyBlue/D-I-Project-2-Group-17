#include <WiFi.h>
#include <WiFiAP.h>

#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>

#include "M5Atom.h"
#include "sensor_data.h"
#include "text_scrolling.h"
#include "pause.h"

const char* ssid = "CkyBlue";
const char* password = "Electrolysis";

AsyncWebServer server(80);

String getTemperature() { return String(round_to_2dp(currentTemp)); }
String getAvgTemperature() { return String(round_to_2dp(getAverageTemperature())); }
String getHourlyAverages() { updateHourlyAverages(); return getHourlyAveragesStr(); }

bool Hflag = true;
float hVal = 11.2;
String getHumidity() { 
  Hflag = !Hflag;
  if (Hflag)
    hVal += 2.1f;
  else
    hVal -= 2.1f;
  return String(hVal); 
  }

void setup(){  
    if(!SPIFFS.begin()){ Serial.println("An Error has occurred while mounting SPIFFS"); return; }

    M5.begin(true, true, true);
    M5.IMU.Init();

    updateTemperatureData();
    enqueueTemperatureData();

    M5.dis.clear();
    delay(10);

    WiFi.softAP(ssid);
    IPAddress myIP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(myIP);

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(SPIFFS, "/index.html");
    });
    server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send_P(200, "text/plain", getTemperature().c_str());
    });
    server.on("/avg_temperature", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send_P(200, "text/plain", (getHourlyAverages() + ":" + getAverageTemperature()).c_str());
    });
    server.on("/humidity", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send_P(200, "text/plain", getHumidity().c_str());
    });

    server.on("/kelvin", HTTP_GET, [](AsyncWebServerRequest *request){
        currentUnit = Unit::K; request->send(204);
    });
    server.on("/fahrenheit", HTTP_GET, [](AsyncWebServerRequest *request){
        currentUnit = Unit::F; request->send(204);
    });
    server.on("/celsius", HTTP_GET, [](AsyncWebServerRequest *request){
        currentUnit = Unit::C; request->send(204);
    });

    server.on("/highcharts.js", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(SPIFFS, "/highcharts.js", "text/javascript");
    });

    server.begin();
}

unsigned int long lastSampled = 0;
bool textDisplay = false;

void loop(){
    if ((millis() - lastSampled) > (samplingDelay * 1000)) {
      updateTemperatureData();
      enqueueTemperatureData();

      lastSampled = millis();
   }

    if (M5.Btn.wasPressed()) {
        textDisplay = !textDisplay;

        // TODO- Switch to currentHumidity
        if (textDisplay) setText(currentTemp, hVal);
    }

    if (isPaused()) return;

    M5.dis.clear();
    if (textDisplay) { textDisplay = scrollText(); }    
}
